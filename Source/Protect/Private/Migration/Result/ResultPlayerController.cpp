#include "Migration/Result/ResultPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "Migration/Result/ResultWidget.h"
#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>
#include <Kismet\GameplayStatics.h>


void AResultPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LP = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem = 
			LP -> GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			SubSystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	if (UResultWidget* Widget = CreateWidget<UResultWidget>(this, ResultWidgetClass))
	{
		Widget->AddToViewport();
	}
}


void AResultPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EIC->BindAction(StartAction, ETriggerEvent::Started, this, &AResultPlayerController::HandeReturn);
	}
}

void AResultPlayerController::HandeReturn(const FInputActionValue& Value)
{
	UGameplayStatics::OpenLevel(this, TitleLevelName);
}