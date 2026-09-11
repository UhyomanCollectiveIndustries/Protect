#pragma once

#include "CoreMinimal.h"

#include "Migration/CustomCollisionEvent.h"

#include "EffectSystem.generated.h"

class UNiagaraSystem;

/**
*
*/
UCLASS()
class PROTECT_API UEffectSystem : public UObject
{
	GENERATED_BODY()

public:
	void Init(UWorld* InWorld, UNiagaraSystem* InImpactEffect);

	void HandleCollision(const FCustomCollisionEvent& Event);

private:
	UNiagaraSystem* ImpactEffect = nullptr;

	/**
	*	TWeakObjectPtrにすることで、ワールドが破棄され、
	*	このシステムだけが生き残っても、無効値として扱うことができる
	*/
	TWeakObjectPtr<UWorld> World;
};
