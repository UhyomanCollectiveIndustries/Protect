#include "Migration/EffectSystem.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Migration/StageObject.h"



/**
*	初期化処理
*		UObjectではGetWorld()が使えないため、ここでUWorld*を明示的に渡して保持
*/
void UEffectSystem::Init(UWorld* InWorld, UNiagaraSystem* InImpactEffect)
{
	World = InWorld;
	ImpactEffect = InImpactEffect;
}

void UEffectSystem::HandleCollision(const FCustomCollisionEvent& Event)
{
	if (!ImpactEffect || !World.IsValid() || !Event.StageObject) return;

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		World.Get(),
		ImpactEffect,
		Event.StageObject->Transform.GetLocation()
	);
}