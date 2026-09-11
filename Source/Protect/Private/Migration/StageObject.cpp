#include "Migration/StageObject.h"

/**
*	初期化処理
*		@param Pos 位置
*		@param InRadius
*/
void UStageObject::Init(const FVector& Pos, float InRadius)
{
	bIsActive = true;
	bIsHit = false;
	Radius = InRadius;
	Transform.SetLocation(Pos);
}