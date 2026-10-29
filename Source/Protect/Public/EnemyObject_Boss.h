#pragma once

#include "CoreMinimal.h"
#include "EnemyObject.h"
#include "EnemyObject_Boss.generated.h"

class UBossWeakPoint;

/**
 *	ボスエネミーの本体クラス
 */
UCLASS()
class PROTECT_API UEnemyObject_Boss : public UEnemyObject
{
	GENERATED_BODY()

public:
	UEnemyObject_Boss();

	virtual void Init(const FVector& Pos, float InRadius, int32 Hp) override;
	virtual void Update(float DeltaTime, const FVector& PlayerPos,const FVector& PlayerCameraPos) override;

	// 弱点パーツの登録(EnemyManagerが生成後に呼ぶ)
	void RegisterPart(UBossWeakPoint* Part);

	// 弱点が全て壊れたか
	bool AreAllPartsDestroyed() const;

private:
	UPROPERTY()
	TArray<UBossWeakPoint*> Parts;

	// プレイヤーよりどのくらい前方を保つか
	float KeepDistance = 2000.f;

	// 追従の速さ。小さいほど、プレイヤーの加減速に対して位置が遅れる
	float FollowInterpSpeed = 3.f;

	// 基準位置を中心にsinで往復する揺れ(弱点が動くので狙いにくくなる)
	float SwayAmplitudeY = 300.f;
	float SwaySpeedY = 0.8f;
	float SwayAmplitudeZ = 150.f;
	float SwaySpeedZ = 1.3f;

	float BaseY = 0.f;	// 揺れの中心(スポーン位置から取得)
	float BaseZ = 0.f;
	float ElapsedTime = 0.f;
};