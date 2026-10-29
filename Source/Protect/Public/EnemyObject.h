#pragma once

#include "CoreMinimal.h"
#include "Migration/StageObject.h"

#include "EnemyObject.generated.h"


/**
 * 敵の論理クラス
 *	ステージオブジェクトを継承し、HPの管理を追加
 *	abstractクラスではないが、基本的にはゲーム内に直接生成されることはなく、継承して使用する想定
 */
UCLASS()
class PROTECT_API UEnemyObject : public UStageObject
{
	GENERATED_BODY()
	
public:
	// 初期化処理
	virtual void Init(const FVector& Pos, float InRadius, int32 Hp);
	// 更新処理(AIの処理)
	virtual void Update(float DeltaTime,const FVector& PlayerPos,const FVector& PlayerCameraPos);

	// HP関係
	void AddHp(int Amount);	// HPを増やす
	void SubtractHp(int Amount); // HPを減らす
	int32 GetCurrentHp() const { return CurrentHp; }  // 現在のHPを取得
	int32 GetMaxHp() const { return MaxHp; } // 最大HPを取得

	bool bCollidable = true;	// 衝突可能かのフラグ、ボスの本体などはfalse
	
private:
	int32 CurrentHp = 0;	// 現在のHP
	int32 MaxHp = 0;	// 最大HP
};
