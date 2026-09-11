#include "Migration/SoundSystem.h"

#include "Kismet/GameplayStatics.h"
#include "Migration/StageObject.h"


/**
*	初期化処理
*		UObjectではGetWorld()が使えないため、ここでUWorld*を明示的に渡して保持
*/
void USoundSystem::Init(UWorld* InWorld, USoundBase* InImpactSound)
{
	World = InWorld;
	ImpactSound = InImpactSound;
}

void USoundSystem::HandleCollision(const FCustomCollisionEvent& Event)
{
	if (!ImpactSound || !World.IsValid() || !Event.StageObject) return;

	/** 衝突した岩の位置でサウンドを鳴らす */
	UGameplayStatics::PlaySoundAtLocation(
		World.Get(),
		ImpactSound,
		Event.StageObject->Transform.GetLocation()
	);

	UE_LOG(LogTemp, Warning, TEXT("Sound"));
}