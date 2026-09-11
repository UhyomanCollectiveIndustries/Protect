#include "Migration/BulletManager.h"


/**
*	初期化処理
*		弾の論理/物理プールを弾の最大数で作成する
*		弾の最大数分の弾をレベル上へスポーンさせる
*/
void UBulletManager::Init(UWorld* World, TSubclassOf<ABulletVisual> VisualClass)
{
	BulletPool.Reserve(MaxBullets);
	VisualPool.Reserve(MaxBullets);

	for (int32 i = 0; i < MaxBullets; ++i)
	{
		BulletPool.Add(NewObject<UBulletObject>(this));
		VisualPool.Add(World->SpawnActor<ABulletVisual>(VisualClass));
	}
}

/**
*	発射処理
*		プールから空きスロットを探索し、
*		空きがあった場合に弾1つに対しての初期化処理を行う
*/
void UBulletManager::Fire(const FVector& Pos, const FVector& Dir)
{
	/** 空きスロットを線形探索 */
	for (int32 i = 0; i < MaxBullets; ++i)
	{
		if (!BulletPool[i]->bIsActive)
		{
			BulletPool[i]->FireInit(Pos, Dir);
			VisualPool[i]->SetVisualActive(true);

			return;
		}
	}
	
	/**
	*	空きがなければ何もしない
	*		[feature]プールの最大値に達したら、最初に撃った弾から消すようにしたい/論理弾の実装も検討
	*/	
}

/** 
*	更新処理
*		弾1つ毎のUpdate,位置の適用を行う
*		論理的に非アクティブな弾を見た目にも適用する
*/
void UBulletManager::Update(float DeltaTime)
{
	for (int32 i = 0; i < MaxBullets; ++i)
	{
		if (BulletPool[i]->bIsActive)
		{
			BulletPool[i]->Update(DeltaTime);
			VisualPool[i]->ApplyTransform(BulletPool[i]->Transform);
		}

		VisualPool[i]->SetVisualActive(BulletPool[i]->bIsActive);
	}
}