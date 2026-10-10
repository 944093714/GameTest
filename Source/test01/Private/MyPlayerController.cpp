// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Character/MyEnemy.h"

// 构造函数：bReplicates 允许网络复制，并从内容资源加载增强输入配置
AMyPlayerController::AMyPlayerController()
{
	bReplicates = true;   // 允许控制器复制（当前基本为单机用法）

	// 从 /Game/BP/Input 路径静态加载资源（FObjectFinder 在构造期查找资产）
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMC(TEXT("/Game/BP/Input/IMC_Mapping.IMC_Mapping"));
	static ConstructorHelpers::FObjectFinder<UInputAction> IAMove(TEXT("/Game/BP/Input/IA_Move.IA_Move"));
	if (IMC.Succeeded())
	{
		MyInputMappingContext = IMC.Object;   // 输入上下文资产
	}
	if (IAMove.Succeeded())
	{
		MoveAction = IAMove.Object;           // 移动动作资产
	}
}

// 每帧执行：驱动鼠标追踪以更新高亮
void AMyPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();   // 核心鼠标高亮逻辑逐帧运行
}

// 鼠标追踪：从鼠标位置向下发射可见性射线，命中实现 IMyInterface 的 Actor 时切换高亮
void AMyPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);   // 可见性通道射线
	if (!CursorHit.bBlockingHit) return;   // 未命中任何物体 → 直接结束

	LastActor = ThisActor;   // 上一帧结果归档
	ThisActor = Cast<IMyInterface>(CursorHit.GetActor());   // 当前帧命中者是否实现了接口

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

// 游戏开始：注册输入映射上下文、显示鼠标光标、设置"游戏+UI"混合输入模式
void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(MyInputMappingContext);   // 校验输入上下文已加载成功

	// 获取本地玩家子系统并注册映射上下文（优先级 0）
	UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);

	Subsystem->AddMappingContext(MyInputMappingContext, 0);

	bShowMouseCursor = true;                    // 显示鼠标(RPG 俯视角需要)
	DefaultMouseCursor = EMouseCursor::Default;

	// 游戏与 UI 同时接收输入的混合模式
	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock); // 不锁在视口内
	InputMode.SetHideCursorDuringCapture(false);                         // 不被遮挡光标
	SetInputMode(InputMode);
}

// 绑定输入：把 MoveAction 触发事件绑定到 Move() 回调
void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// PlayerController 的 InputComponent 是增强输入组件
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	// 动作每次数值变化(Triggered)时调用 Move
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyPlayerController::Move);
}

// 移动处理：将 2D 输入轴量转为相对视角(仅 Yaw)的前/右方向输入
void AMyPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();   // WASD 得到的二维轴量
	const FRotator InputAxisRotation = GetControlRotation();               // 当前相机旋转
	const FRotator YawRotation(0.f, InputAxisRotation.Yaw, 0.f);           // 只取 Yaw，保证水平移动

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



