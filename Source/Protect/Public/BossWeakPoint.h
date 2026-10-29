#pragma once

#include "CoreMinimal.h"
#include "EnemyObject.h"
#include "BossWeakPoint.generated.h"

class UEnemyObject_Boss;

UCLASS()
class PROTECT_API UBossWeakPoint : public UEnemyObject
{
	GENERATED_BODY()

public:
	// 親(ボス本体)と、本体から見た相対位置を設定する
	void SetupPart(UEnemyObject_Boss* InParent, const FVector& InLocalOffset);

	virtual void Update(float DeltaTime, const FVector& PlayerPos,const FVector& PlayerCameraPos) override;

private:
	UPROPERTY()
	UEnemyObject_Boss* Parent = nullptr;

	FVector LocalOffset = FVector::ZeroVector;
};