#pragma once

#include "CoreMinimal.h"
#include "Migration/GameObjectBase.h"

#include "BulletObject.generated.h"


/**
 * 弾1つ分のロジッククラス
 */
UCLASS()
class PROTECT_API UBulletObject:public UGameObjectBase
{
	GENERATED_BODY()

public:

	/** 弾がアクティブかどうかのフラグ */
	bool bIsActive = false;

	/** 弾の移動量(方向*速度) */
	FVector Velocity = FVector::ZeroVector;

	/** 弾の残り生存時間タイマー */
	float LifeTimer = 0.f;

	/** 弾の寿命(秒) */
	static constexpr float BulletLifeTime = 1.5f;


	/** 発射時の初期化 */
	void FireInit(const FVector& StartPos, const FVector& Dir);

	/** 更新処理 */
	virtual void Update(float DeltaTime) override;
};
