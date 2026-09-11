#pragma once

#include "CoreMinimal.h"

#include "Migration/CustomCollisionEvent.h"

#include "ScoreSystem.generated.h"

/** UI側はこれをAddUObject / AddLambdaで購読するだけで、ScoreSystem内部を知らなくて済む */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnScoreChanged, int32 /*NewScore*/);


/**
* 
*/
UCLASS()
class PROTECT_API UScoreSystem : public UObject
{
	GENERATED_BODY()

public:
	/**  */
	void HandleCollision(const FCustomCollisionEvent& Event);

	/** スコア加算 */
	void AddScore(int32 Amount);

	int32 GetScore() const { return Score; };

	void Reset();

	/** UI側が購読するデリケート(スコアが変化するたびBroadcastされる) */
	FOnScoreChanged OnScoreChanged;

private:
	int32 Score = 0;
};