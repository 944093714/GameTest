// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"

#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "Character/MyEnemy.h"
#include "Input/MyEnhancedInputComponent.h"
#include "AbilitySystem/MyAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "Input/MyGameplayTags.h"
#include "Kismet/GameplayStatics.h"


// 构造函数：bReplicates 允许网络复制，并从内容资源加载增强输入配置
AMyPlayerController::AMyPlayerController()
{
	bReplicates = true; // 允许控制器复制（当前基本为单机用法）

	Spline = CreateDefaultSubobject<USplineComponent>("Spline");

	// // 从 /Game/BP/Input 路径静态加载资源（FObjectFinder 在构造期查找资产）
	// static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMC(TEXT("/Game/BP/Input/IMC_Mapping.IMC_Mapping"));
	// static ConstructorHelpers::FObjectFinder<UInputAction> IAMove(TEXT("/Game/BP/Input/IA_Move.IA_Move"));
	// if (IMC.Succeeded())
	// {
	// 	MyInputMappingContext = IMC.Object;   // 输入上下文资产
	// }
	// if (IAMove.Succeeded())
	// {
	// 	MoveAction = IAMove.Object;           // 移动动作资产
	// }
}

// 每帧执行：驱动鼠标追踪以更新高亮
void AMyPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace(); // 核心鼠标高亮逻辑逐帧运行

	AutoRun();
}

void AMyPlayerController::AutoRun()
{
	if (!bAutoRunning)return;
	if (APawn* ControlledPawn = GetPawn())
	{
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(
			ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(
			LocationOnSpline, ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(Direction);

		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if (DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}

// 鼠标追踪：从鼠标位置向下发射可见性射线，命中实现 IMyInterface 的 Actor 时切换高亮
void AMyPlayerController::CursorTrace()
{
	
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit); // 可见性通道射线
	if (!CursorHit.bBlockingHit) return; // 未命中任何物体 → 直接结束

	LastActor = ThisActor; // 上一帧结果归档
	ThisActor = Cast<IMyInterface>(CursorHit.GetActor()); // 当前帧命中者是否实现了接口

	// 三态切换：处理"进入 / 离开 / 更换目标"的高亮变化
	if (LastActor == nullptr)
	{
		// 之前没有悬停对象 → 现在命中了  → 高亮它
		if (ThisActor != nullptr)
		{
			ThisActor->HighlightActor();
		}
	}
	else
	{
		if (ThisActor == nullptr)
		{
			// 之前有、现在没有 → 取消旧高亮
			LastActor->UnHighlightActor();
		}
		else
		{
			if (ThisActor != LastActor)
			{
				// 悬停目标发生了切换 → 先取消旧的，再高亮新的
				LastActor->UnHighlightActor();
				ThisActor->HighlightActor();
			}
			// ThisActor == LastActor：仍是同一目标，保持高亮，什么都不做
		}
	}
}

// 能力标签按键「按下」回调：打印被按下的标签用于调试，后续可在此触发对应技能
void AMyPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(InputTag_LMB)) //我并没有像教程一样创建结构体，使用是全局的标签变量，直接使用就可以
	{
		bTargeting = ThisActor ? true : false;
		bAutoRunning = false;
	}
}

// 能力标签按键「松开」回调：打印被松开的标签用于调试，后续可在此处理技能释放
void AMyPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	// UE_LOG(LogTemp, Warning, TEXT("Released %s"), *InputTag.ToString());
	if (InputTag.MatchesTagExact(InputTag_LMB)) //我并没有像教程一样创建结构体，使用是全局的标签变量，直接使用就可以
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagReleased(InputTag);
		}
		if (bTargeting)
		{
			if (GetASC())
			{
				GetASC()->AbilityInputTagHeld(InputTag);
			}
		}
		else
		{
			APawn* ControlledPawn = GetPawn();
			if (FollowTime <= ShortPressThreshold)
			{
				if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(
					this, ControlledPawn->GetActorLocation(), CachedDestination))
				{
					Spline->ClearSplinePoints();
					for (const FVector& SplinePoint : NavPath->PathPoints)
					{
						Spline->AddSplinePoint(SplinePoint, ESplineCoordinateSpace::World);
						DrawDebugSphere(GetWorld(), SplinePoint, 8.f, 8, FColor::Green, false, 5.f);
					}
					bAutoRunning = true;
				}
			}
			FollowTime = 0.f;
			bTargeting = false;
		}
	}
}

// 能力标签按键「持续按住」回调：打印持续按住的标签用于调试，后续可用于蓄力/持续技能
void AMyPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	// UE_LOG(LogTemp, Warning, TEXT("Held %s"), *InputTag.ToString());

	if (InputTag.MatchesTagExact(InputTag_LMB)) //我并没有像教程一样创建结构体，使用是全局的标签变量，直接使用就可以
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);
		}
		if (bTargeting)
		{
			if (GetASC())
			{
				GetASC()->AbilityInputTagHeld(InputTag);
			}
		}
		else
		{
			FollowTime += GetWorld()->GetDeltaSeconds();
			FHitResult Hit;
			// if (GetHitResultUnderCursor(ECC_Visibility, false, Hit))
			// {
			// 	CachedDestination = Hit.ImpactPoint;
			// }
			if (CursorHit.bBlockingHit)
			{
				CachedDestination = CursorHit.ImpactPoint;
			}
			if (APawn* ControlledPawn = GetPawn())
			{
				const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
				ControlledPawn->AddMovementInput(WorldDirection);
			}
		}
	}
}

UMyAbilitySystemComponent* AMyPlayerController::GetASC()
{
	if (MyAbilitySystemComponent == nullptr)
	{
		MyAbilitySystemComponent = Cast<UMyAbilitySystemComponent>(
			UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return MyAbilitySystemComponent;
}


// 游戏开始：注册输入映射上下文、显示鼠标光标、设置"游戏+UI"混合输入模式
void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(MyInputMappingContext); // 校验输入上下文已加载成功

	// 获取本地玩家子系统并注册映射上下文（优先级 0）
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		GetLocalPlayer());

	if (Subsystem != nullptr)
	{
		Subsystem->AddMappingContext(MyInputMappingContext, 0);
	}


	bShowMouseCursor = true; // 显示鼠标(RPG 俯视角需要)
	DefaultMouseCursor = EMouseCursor::Default;

	// 游戏与 UI 同时接收输入的混合模式
	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock); // 不锁在视口内
	InputMode.SetHideCursorDuringCapture(false); // 不被遮挡光标
	SetInputMode(InputMode);
}

// 绑定输入：把 MoveAction 触发事件绑定到 Move() 回调
void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// PlayerController 的 InputComponent 是增强输入组件
	//UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);改为使用自己的增强输入系统
	UMyEnhancedInputComponent* EnhancedInputComponent = CastChecked<UMyEnhancedInputComponent>(InputComponent);

	// 动作每次数值变化(Triggered)时调用 Move 旧版的
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyPlayerController::Move);

	//新版的绑定
	// 依据 MyInputConfig 中配置好的「输入动作→能力标签」映射,把按下/松开/按住三类回调统一绑定到能力标签上
	EnhancedInputComponent->BindAbilityActions(MyInputConfig, this, &ThisClass::AbilityInputTagPressed,
	                                           &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

// 移动处理：将 2D 输入轴量转为相对视角(仅 Yaw)的前/右方向输入
void AMyPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>(); // WASD 得到的二维轴量
	const FRotator InputAxisRotation = GetControlRotation(); // 当前相机旋转
	const FRotator YawRotation(0.f, InputAxisRotation.Yaw, 0.f); // 只取 Yaw，保证水平移动

	// 由仅含 Yaw 的旋转矩阵取单位轴：X=前方向，Y=右方向
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// 若存在被操控的 Pawn，则按 前/右 叠加输入向量(X 对应前后, Y 对应左右)
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.X);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.Y);
	}
}
