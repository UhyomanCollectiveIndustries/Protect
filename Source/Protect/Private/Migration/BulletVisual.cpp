#include "Migration/BulletVisual.h"


/** コンストラクタ */
ABulletVisual::ABulletVisual()
{
	PrimaryActorTick.bCanEverTick = false; /** Tick処理はGameManagerで一元管理するため、false */

	/** 弾の見た目についての初期設定 */
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
	MeshComp->SetRelativeScale3D(FVector(0.2f));

	/** 初期設定では、全ての弾の見た目を隠す/当たり判定を無効化する */
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}

/**
*	弾の位置の適用
*		BulletManager::Updateで呼ばれる
*/
void ABulletVisual::ApplyTransform(const FTransform& WorldTransform)
{
	SetActorTransform(WorldTransform);
}

/**
*	BulletObjectのアクティブ状隊に応じて
*	見た目のオン/オフ、当たり判定のオン/オフを切り替える
*		Fireでオン
*		寿命、ステージオブジェクトとの衝突でオフ
*/
void ABulletVisual::SetVisualActive(bool bActive)
{
	SetActorHiddenInGame(!bActive);
	SetActorEnableCollision(bActive);
}