#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"

#include "TitlePlayerController.generated.h"


/**
 * 
 */
UCLASS()
class PROTECT_API ATitlePlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* StartAction;

	UPROPERTY(EditDefaultsOnly, Category = "GameFlow")
	FName PlayLevelName = "Lvl_Play";

	void HandleStart(const FInputActionValue& Value);
};