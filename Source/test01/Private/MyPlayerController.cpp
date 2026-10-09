// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "SAdvancedTransformInputBox.h"
#include "ToolContextInterfaces.h"
#include "InputAction.h"
#include "InputMappingContext.h"

AMyPlayerController::AMyPlayerController()
{
	bReplicates = true;

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMC(TEXT("/Game/BP/Input/IMC_Mapping.IMC_Mapping"));
	static ConstructorHelpers::FObjectFinder<UInputAction> IAMove(TEXT("/Game/BP/Input/IA_Move.IA_Move"));
	if (IMC.Succeeded())
	{
		MyInputMappingContext = IMC.Object;
	}
	if (IAMove.Succeeded())
	{
		MoveAction = IAMove.Object;
	}
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	check(MyInputMappingContext);
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = 
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);
	
	Subsystem->AddMappingContext(MyInputMappingContext,0);
	
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	
	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetHideCursorDuringCapture(false);
	
	SetInputMode(InputMode);
		
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyPlayerController::Move);
}

void AMyPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator InputAxisRotation = GetControlRotation();
	const FRotator YawRotation(0.f,InputAxisRotation.Yaw,0.f);
	
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	if(APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection,InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection,InputAxisVector.X);
	}
	
}


