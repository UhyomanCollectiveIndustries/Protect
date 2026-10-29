#include "Migration/ScoreSystem.h"

#include "Migration/StageObject.h"


void UScoreSystem::HandleCollision(const FCustomCollisionEvent& Event)
{
	/**
	*	衝突したオブジェクトの種類に応じて
	*	加算するスコアを変える
	*/
	if (Event.EnemyObject)
	{
		// 敵はHPが0になった時のみ加算
		if (Event.bEnemyKilled)
		{
			AddScore(100);
		}
	}
	else if (Event.StageObject)
	{
		AddScore(Event.StageObject->ScoreValue);
	}
}

void UScoreSystem::Reset()
{
	Score = 0;
	OnScoreChanged.Broadcast(Score);	// UIへの反映
}

void UScoreSystem::AddScore(int32 Amount)
{
	Score += Amount;
	OnScoreChanged.Broadcast(Score); // UIへの反映
}
