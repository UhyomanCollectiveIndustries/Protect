#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include <Components/TextBlock.h>

#include "ScoreWidget.generated.h"

UCLASS()
class PROTECT_API UScoreWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void HandleScoreChanged(int32 NewScore);

	UPROPERTY(meta=(BindWidget))
	UTextBlock* ScoreText;	/** UMGデザイナーで同名のTextBlockを配置 */
};