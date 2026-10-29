#pragma once

#include "CoreMinimal.h"
#include "Migration/CustomCollisionEvent.h"

#include "EffectSystem.generated.h"

class UNiagaraSystem;


/**
*	エフェクトシステム
*/
UCLASS()
class PROTECT_API UEffectSystem : public UObject
{
	GENERATED_BODY()

public:
	// 初期化とエフェクトアセットの登録
	void Init(UWorld* InWorld,UNiagaraSystem* InStageEffect, UNiagaraSystem* InEnemyEffect);

	// 衝突イベントに応じたエフェクトの発生処理
	void HandleCollision(const FCustomCollisionEvent& Event);

private:

	// 各エフェクトアセットの格納郡
	UNiagaraSystem* StageEffect = nullptr;	// ステージ衝突用
	UNiagaraSystem* EnemyEffect = nullptr;	// 敵消滅用

	// ワールドが破棄時の安全性考慮し、弱参照で保持
	TWeakObjectPtr<UWorld> World;
};
