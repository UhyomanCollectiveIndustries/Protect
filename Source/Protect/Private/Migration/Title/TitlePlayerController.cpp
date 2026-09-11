#include "Migration/Title/TitlePlayerController.h"

#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>
#include <Kismet\GameplayStatics.h>


void ATitlePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LP = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem =
			LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			SubSystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ATitlePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EIC->BindAction(StartAction, ETriggerEvent::Started, this, &ATitlePlayerController::HandleStart);
	}
}

void ATitlePlayerController::HandleStart(const FInputActionValue& Value)
{
	UGameplayStatics::OpenLevel(this, PlayLevelName);
}