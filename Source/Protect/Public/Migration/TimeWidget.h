#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/TextBlock.h>

#include "TimeWidget.generated.h"


/**
*	
*/
UCLASS()
class PROTECT_API UTimeWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	void ApplyTime(float ElapsedTime);

protected:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* TimeText;

};
