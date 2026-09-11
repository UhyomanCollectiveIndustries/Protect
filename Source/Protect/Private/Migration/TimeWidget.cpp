#include "Migration/TimeWidget.h"

#include "Migration/GameManager.h"
#include <Kismet\GameplayStatics.h>


void UTimeWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UTimeWidget::ApplyTime(float ElapsedTime)
{
	if (!TimeText) return;

	int32 TotalSeconds = FMath::FloorToInt(ElapsedTime);
	int32 Minutes = TotalSeconds / 60;
	int32 Seconds = TotalSeconds % 60;

	if (Minutes < 0 || Seconds < 0)
	{
		Minutes, Seconds = 0;
	}

	FString TimeString = FString::Printf(TEXT("Time:\n%02d:%02d"), Minutes, Seconds);
	TimeText->SetText(FText::FromString(TimeString));
}