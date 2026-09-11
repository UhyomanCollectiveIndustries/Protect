#include "Migration/ScoreWidget.h"

#include "Migration/GameManager.h"
#include <Kismet\GameplayStatics.h>


void UScoreWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (AGameManager* GM = Cast<AGameManager>(
		UGameplayStatics::GetActorOfClass(GetWorld(), AGameManager::StaticClass())))
	{
		if (GM->GetScoreSytem())
		{
			GM->GetScoreSytem()->OnScoreChanged.AddUObject(this, &UScoreWidget::HandleScoreChanged);
		}
	}
}

void UScoreWidget::HandleScoreChanged(int32 NewScore)
{
	if (ScoreText)
	{
		FString ScoreString = FString::Printf(TEXT("Score:%2d"), NewScore);
		ScoreText->SetText(FText::FromString(ScoreString));
	}
}