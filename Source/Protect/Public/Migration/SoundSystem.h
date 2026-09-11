#pragma once

#include "CoreMinimal.h"

#include "CustomCollisionEvent.h"

#include "SoundSystem.generated.h"


/**
* 
*/
UCLASS()
class PROTECT_API USoundSystem : public UObject
{
	GENERATED_BODY()

public:
	void Init(UWorld* InWorld, USoundBase* InImpactSound);

	void HandleCollision(const FCustomCollisionEvent& Event);

private:
	UPROPERTY()
	USoundBase* ImpactSound = nullptr;

	/** 
	*	TWeakObjectPtrにすることで、ワールドが破棄され、
	*	このシステムだけが生き残っても、無効値として扱うことができる
	*/
	TWeakObjectPtr<UWorld> World;
};