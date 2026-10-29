#include "EnemyVisual.h"


// コンストラクタ
AEnemyVisual::AEnemyVisual()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
	MeshComp->SetRelativeScale3D(FVector(1.0f));

	SetVisualActive(false);
}

// Transformの適用(更新処理)
void AEnemyVisual::ApplyTransform(const FTransform& WorldTransform)
{
	SetActorTransform(WorldTransform);
}

void AEnemyVisual::SetVisualActive(bool bActive)
{
	SetActorHiddenInGame(!bActive);
	SetActorEnableCollision(bActive);
}
