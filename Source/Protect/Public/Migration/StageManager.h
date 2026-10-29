#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "Migration/RockVisual.h"
#include "Migration/StageObject.h"

#include "StageManager.generated.h"


USTRUCT()
struct FStageObjectGroup
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere) FName GroupName;	// 識別用
	UPROPERTY(EditAnywhere) TSubclassOf<ARockVisual> VisualClass;
	UPROPERTY(EditAnywhere) int32 Count = 30;
	UPROPERTY(EditAnywhere) FVector AreaMin = FVector::ZeroVector;
	UPROPERTY(EditAnywhere) FVector AreaMax = FVector::ZeroVector;
	UPROPERTY(EditAnywhere) float Radius = 200.f;
	UPROPERTY(EditAnywhere) int32 ScoreValue = 100;
};

/**
 *	ステージオブジェクト全体を管理するクラス
 */
UCLASS()
class PROTECT_API UStageManager : public UObject
{
	GENERATED_BODY()

public:
	/** 初期化処理　*/
	void Init(UWorld* World, TSubclassOf<ARockVisual> VisualClass, const TArray<FVector>& RockPositions);
	
	void SpawnGroup(UWorld* World,const FStageObjectGroup& Group);

	/** 更新処理 */
	void Update(float DeltaTime);

	/** ステージオブジェクトの論理 */
	UPROPERTY()
	TArray<UStageObject*> StageObjects;

	/** ステージオブジェクトの見た目 */
	UPROPERTY()
	TArray<ARockVisual*> StageVisuals;
};