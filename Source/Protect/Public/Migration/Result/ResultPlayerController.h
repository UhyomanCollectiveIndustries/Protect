#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"

#include "ResultPlayerController.generated.h"

class RsultWidget;


/**
 * 
 */
UCLASS()
class PROTECT_API AResultPlayerController : public APlayerController
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
	FName TitleLevelName = "Lvl_Title";

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UResultWidget> ResultWidgetClass;

	void HandeReturn(const FInputActionValue& Value);
};
