#pragma once

#include "CoreMinimal.h"
#include "CustomCollisionEvent.h"

#include "SoundSystem.generated.h"


/**
*	サウンドシステム
*/
UCLASS()
class PROTECT_API USoundSystem : public UObject
{
	GENERATED_BODY()

public:
	// 初期化とサウンドアセットの登録
	void Init(UWorld* InWorld, USoundBase* InStageSound, USoundBase* InEnemySound);

	// 衝突イベントに応じたサウンドの発生処理
	void HandleCollision(const FCustomCollisionEvent& Event);

private:

	// 各サウンドアセットの格納郡
	UPROPERTY()
	USoundBase* StageSound = nullptr;	// ステージ衝突用
	UPROPERTY()
	USoundBase* EnemySound = nullptr;	// 敵消滅用

	// ワールドが破棄時の安全性考慮し、弱参照で保持
	TWeakObjectPtr<UWorld> World;

	// ピッチ処理
	float CurrentPitch = 1.0f;
	const float BasePitch = 1.0f;
	const float PitchStep = 0.1f; // 1ヒット毎の上昇量
	const float MaxPitch = 2.0f;  // 最大ピッチ
	const float ComboResetTime = 1.0f; // この秒数間隔が空いたらコンボリセット

	float LastHitTime = -100.0f; // 初期値は必ずリセット扱いされるよう十分小さい値にしておく
};