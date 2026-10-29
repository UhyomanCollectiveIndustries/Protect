#include "FollowScenery.h"


AFollowScenery::AFollowScenery()
{
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}


void AFollowScenery::Update(const FVector& PlayerPos)
{
	auto Snap = [this](double V)
	{
		return SnapSize > 0.f ? FMath::FloorToDouble(V / SnapSize) * SnapSize : V;
	};

	FVector Loc = GetActorLocation();
	if (bFollowX) Loc.X = Snap(PlayerPos.X) + FollowOffset.X;
	if (bFollowY) Loc.Y = Snap(PlayerPos.Y) + FollowOffset.Y;
	if (bFollowZ) Loc.Z = Snap(PlayerPos.Z) + FollowOffset.Z;
	SetActorLocation(Loc);
}