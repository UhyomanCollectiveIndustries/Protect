#pragma once

#include "CoreMinimal.h"

#include "Migration/GameObjectBase.h"

#include "StageObject.generated.h"

UCLASS()
class PROTECT_API UStageObject : public UGameObjectBase
{
	GENERATED_BODY()

public:

	/** アクティブフラグ */
	bool bIsActive = true;
	
	/** 攻撃があたったかのフラグ */
	bool bIsHit = false;

	/**
	*	ステージオブジェクトの半径
	*		SphereColliderの半径になる
	*/
	float Radius = 100.0f;

	/** 初期化処理 */
	void Init(const FVector& Pos, float InRadius);
};
