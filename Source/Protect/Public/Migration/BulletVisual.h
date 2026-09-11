#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "BulletVisual.generated.h"


/**
* 弾1つ分の見た目クラス
*/
UCLASS()
class PROTECT_API ABulletVisual : public AActor
{
	GENERATED_BODY()
	
public:	
	/** コンストラクタ */
	ABulletVisual();

	/** 弾の位置の適用 */
	void ApplyTransform(const FTransform& WorldTransform);

	/** 弾のアクティブ状隊に応じて見た目のオン/オフを切り替える */
	void SetVisualActive(bool bActive);

private:
	/** 弾の見た目 */
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp;
};
