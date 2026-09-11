#pragma once

#include "CoreMinimal.h"

#include "Migration/BulletVisual.h"
#include "Migration/BulletObject.h"

#include "BulletManager.generated.h"


/**
 * プレイヤが発射する弾全体の管理を行うクラス
 */
UCLASS()
class PROTECT_API UBulletManager : public UObject
{
	GENERATED_BODY()

public:

	/** 弾の最大数 */
	static const int32 MaxBullets = 15;

	/** 初期化処理 */
	void Init(UWorld* World, TSubclassOf<ABulletVisual> VisualClass);
	/** 弾発射処理 */
	void Fire(const FVector& Pos, const FVector& Dir);
	/** 更新処理 */
	void Update(float DeltaTime);

	/** プールのゲッター */
	const TArray<UBulletObject*> GetBulletsPool() const { return BulletPool; };

private:

	/** UEのGCで回収されないよう、UPROPRETYでプールを保持する */
	
	/** 弾の論理プール */
	UPROPERTY()
	TArray<UBulletObject*> BulletPool;

	/** 弾の物理プール */
	UPROPERTY()
	TArray<ABulletVisual*> VisualPool;
};