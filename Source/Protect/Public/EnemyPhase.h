#pragma once

#include "CoreMinimal.h"
#include "EnemyPhase.generated.h"

class UEnemyObject;
class AEnemyVisual;

/*+
 *
 */
USTRUCT(BlueprintType)
struct FBossPartInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere,Category="Boss")
	TSubclassOf<AEnemyVisual> VisualClass;
	
	// ボス本体の位置から見た相対位置
	UPROPERTY(EditAnywhere,Category="Boss")
	FVector LocalOffSet = FVector::ZeroVector;
	
	// 弾の当たり判定の半径(見た目のメッシュに合わせる)
	UPROPERTY(EditAnywhere,Category="Boss")
	float Radius = 10.f;
	
	UPROPERTY(EditAnywhere,Category="Boss")
	int32 Hp = 10;	
};

/**
 *	敵1体分の情報
 *		敵の種類
 *		敵の見た目
 *		発生する場所
 *		半径
 *		体力
 *	をエディタ上で設定できるようにする
 */
USTRUCT(BlueprintType)
struct FEnemySpawnInfo
{
	GENERATED_BODY()

	// EnemyObjectにすることで、後の派生敵を指定できるようにする
	UPROPERTY(EditAnywhere, Category = "Enemy")
	TSubclassOf<UEnemyObject> EnemyClass;

	UPROPERTY(EditAnywhere, Category = "Enemy")
	TSubclassOf<AEnemyVisual> VisualClass;

	UPROPERTY(EditAnywhere, Category = "Enemy")
	FVector SpawnPosition = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Enemy")
	float Radius = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy")
	int32 Hp = 10;
	
	// ボスの場合のみ使用する弱点パーツ一覧
	UPROPERTY(EditAnywhere,Category="Boss")
	TArray<FBossPartInfo> Parts;
};

/**
*	1フェーズの敵のまとまり
*		発生のトリガーとなるX座標
* 		敵の配置情報の配列
*	をエディタ上で設定する
*/
USTRUCT(BlueprintType)
struct FEnemyPhase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Phase")
	float TriggerX = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Phase")
	TArray<FEnemySpawnInfo> Enemies;

	// フェーズ開始フラグ(重複防止用)
	bool bTriggered = false;
};
