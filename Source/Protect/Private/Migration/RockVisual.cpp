#include "Migration/RockVisual.h"


/** コンストラクタ */
ARockVisual::ARockVisual()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
	MeshComp->SetRelativeScale3D(FVector(1.0f));

	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
}

/** */
void ARockVisual::ApplyTransform(const FTransform& WorldTransform)
{
	SetActorTransform(WorldTransform);
}

/** */
void ARockVisual::SetVisualActive(bool bActive)
{
	SetActorHiddenInGame(!bActive);
	SetActorEnableCollision(bActive);
}