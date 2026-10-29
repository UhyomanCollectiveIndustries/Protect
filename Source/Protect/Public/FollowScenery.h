#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FollowScenery.generated.h"


/**
 *	プレイヤーに追従する床のクラス
 *		ボス線などの床をループさせたい場合に用いる
 */
UCLASS()
class PROTECT_API AFollowScenery : public AActor
{
	GENERATED_BODY()
	
public:
	// コンストラクタ
	AFollowScenery();
	
	// 更新処理(GameManager::Tickから呼ばれる)
	void Update(const FVector& PlayerPos);
	
private:
	UPROPERTY(EditAnywhere, Category = "Follow") bool bFollowX = true;
	UPROPERTY(EditAnywhere, Category = "Follow") bool bFollowY = false;
	UPROPERTY(EditAnywhere, Category = "Follow") bool bFollowZ = false;
	
	// 追従する軸に足すオフセット
	UPROPERTY(EditAnywhere, Category = "Follow") FVector FollowOffset = FVector::ZeroVector;

	// 0なら滑らかに追従。テクスチャ1枚分の長さを入れると、その単位でカクカクに動く
	UPROPERTY(EditAnywhere, Category = "Follow") float SnapSize = 0.f;
};
