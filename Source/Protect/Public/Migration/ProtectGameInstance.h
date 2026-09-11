#pragma once

#include "CoreMinimal.h"

#include "ProtectGameInstance.generated.h"

UCLASS()
class PROTECT_API UProtectGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int32 FinalScore = 0;
};
