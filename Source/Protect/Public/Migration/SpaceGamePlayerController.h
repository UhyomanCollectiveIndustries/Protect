#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"

#include "Migration/GameManager.h"

#include "SpaceGamePlayerController.generated.h"


/**
 * 
 */
UCLASS()
class PROTECT_API ASpaceGamePlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	UPROPERTY(EditDefaultsOnly,Category="Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* FireAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* SprintAction;

	void HandleMove(const FInputActionValue& Value);
	void HandleMoveCompleted(const FInputActionValue& Value);

	void HandleSprint(const FInputActionValue& Value);
	void HandleSprintCompleted(const FInputActionValue& Value);

	void HandleFire(const FInputActionValue& Value);

	UPROPERTY()
	AGameManager* GameManager;
};
