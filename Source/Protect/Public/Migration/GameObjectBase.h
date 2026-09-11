#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "GameObjectBase.generated.h"


/**
 * 
 */
UCLASS()
class PROTECT_API UGameObjectBase : public UObject
{
	GENERATED_BODY()

public:

	virtual void Init(){}
	virtual void Update(float DeltaTime) {}
	
	FTransform Transform;		/** ワールド上の位置 */
	FTransform ModelTransform;	/** モデルオフセット */

	UPROPERTY()
	UStaticMeshComponent* MeshComp = nullptr;
};