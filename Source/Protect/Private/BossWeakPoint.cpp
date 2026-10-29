#include "BossWeakPoint.h"
#include "EnemyObject_Boss.h"

void UBossWeakPoint::SetupPart(UEnemyObject_Boss* InParent, const FVector& InLocalOffset)
{
	Parent = InParent;
	LocalOffset = InLocalOffset;
}

/**
 *	更新処理
 *		基底のUpdate()は「プレイヤーの背後に回ったら消える」判定を含むので、ここでは呼ばない
 *		(弱点はボス本体に追従する存在で、独自に背後判定をする必要がないため)
 */
void UBossWeakPoint::Update(float DeltaTime, const FVector& PlayerPos,const FVector& PlayerCameraPos)
{
	if (!bIsActive) return;

	// 本体がいなくなったら、弱点も一緒に消える
	if (!Parent || !Parent->bIsActive)
	{
		bIsActive = false;
		return;
	}
	
	// HPが尽きたら破壊(非アクティブ化 → EnemyManagerが見た目ごと破棄する)
	if (GetCurrentHp() <= 0)
	{
		bIsActive = false;
		return;
	}

	// 本体の位置 + 相対オフセットに追従
	Transform.SetLocation(Parent->Transform.GetLocation() + LocalOffset);
}