#pragma once

#include "CoreMinimal.h"

#include "CustomCollisionEvent.generated.h"

class UBulletObject;
class UStageObject;


/**
 * 
 */
USTRUCT()
struct FCustomCollisionEvent
{
	GENERATED_BODY()

	/**
	*	UPROPERTY()での宣言により、イベントがキューに溜まっている間の
	*	GCによる誤回収を防ぐ
	*/
	
	UPROPERTY()
	UBulletObject* Bullet = nullptr;

	UPROPERTY()
	UStageObject* StageObject = nullptr;
};
