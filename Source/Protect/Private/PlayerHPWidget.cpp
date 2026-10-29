#include "PlayerHPWidget.h"

#include "Migration/PlayerObject.h"


void UPlayerHPWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

// 体力の割合をプログレスバーに適用
void UPlayerHPWidget::SetHPPercentToBar(float InPercent)
{
	float clampedPercent = FMath::Clamp(InPercent, 0.0f, 1.0f);
	// UIへ反映
	HPBar->SetPercent(clampedPercent);
}
