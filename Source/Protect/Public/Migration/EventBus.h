#pragma once

#include "CoreMinimal.h"

#include "Migration/CustomCollisionEvent.h"

#include "EventBus.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCollisionEvent, const FCustomCollisionEvent&);


/**
*	イベントバス
*/
UCLASS()
class PROTECT_API UEventBus : public UObject
{
	GENERATED_BODY()

public:
	FOnCollisionEvent OnCollision;

	void Publish(const FCustomCollisionEvent& Event)
	{
		OnCollision.Broadcast(Event);
	}
};
