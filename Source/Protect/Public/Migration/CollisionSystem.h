#pragma once

#include "CoreMinimal.h"

class UBulletManager;
class UStageManager;
struct FCustomCollisionEvent;


/**
 * 
 */
class FCollisionSystem
{
public:
	
	/**
	*	弾とステージオブジェクトの当たり判定
	*/
	static void CheckBulletVsStage(
		UBulletManager& Bullets,
		UStageManager& Stage,
		TArray<FCustomCollisionEvent>& OutEvents
	);
};
