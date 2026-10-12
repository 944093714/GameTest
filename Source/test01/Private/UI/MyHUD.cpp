// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MyHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/WidgetController/OverlayMyWidgetController.h"
#include "UI/Widget/MyUserWidget.h"

UOverlayMyWidgetController* AMyHUD::GetOverlayWidgetController(const FWidgetControllerParams& Params)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayMyWidgetController>(this, OverlayWidgetControllerClass);

		OverlayWidgetController->SetWidgetControllerParams(Params);
		OverlayWidgetController->BindCallbacksToDependancies();
		
		return OverlayWidgetController;
	}

	return OverlayWidgetController;
}

void AMyHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("Overlay widget"))
	checkf(OverlayWidgetControllerClass, TEXT("Overlay widget Controller"));

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UMyUserWidget>(Widget);

	FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UOverlayMyWidgetController* Controller = GetOverlayWidgetController(WidgetControllerParams);

	OverlayWidget->SetWidgetController(Controller);
	Controller->BroadcastInitialValues();


	Widget->AddToViewport();
}

void AMyHUD::BeginPlay()
{
	Super::BeginPlay();
}
