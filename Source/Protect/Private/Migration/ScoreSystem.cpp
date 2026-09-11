#include "Migration/ScoreSystem.h"


void UScoreSystem::HandleCollision(const FCustomCollisionEvent& Event)
{
	AddScore(100);
}

void UScoreSystem::AddScore(int32 Amount)
{
	Score += Amount;
	OnScoreChanged.Broadcast(Score); /** UIへの反映はここで通知する */
}

void UScoreSystem::Reset()
{
	Score = 0;
	OnScoreChanged.Broadcast(Score);
}