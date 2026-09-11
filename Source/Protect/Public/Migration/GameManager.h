#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Migration/GameObjectBase.h"
#include "Migration/PlayerObject.h"
#include "Migration/ShipVisual.h"

#include "Migration/BulletManager.h"
#include "Migration/StageManager.h"

#include "Migration/EventBus.h"
#include "Migration/SoundSystem.h"
#include "Migration/EffectSystem.h"
#include "Migration/ScoreSystem.h"
#include "Migration/TimeWidget.h"


#include "GameManager.generated.h"

class UScoreWidget;


/**
*	ゲームマネージャークラス
*/
UCLASS()
class PROTECT_API AGameManager : public AActor
{
	GENERATED_BODY()

public:
	/** コンストラクタ */
	AGameManager();

	/** ゲッター */

	UPlayerObject* GetPlayerObject() const { return Player; }
	AShipVisual* GetPlayerVisual() const { return PlayerVisual; }
	UBulletManager* GetBulletManager() const { return BulletManager; }
	UScoreSystem* GetScoreSytem() const { return ScoreSystem; };
	float GetElapsedTime() { return ElapsedTime; };

protected:
	/**
	*	初期化処理
	*		BeginPlayはここにしか存在しない
	*/
	virtual void BeginPlay() override;

	/**
	*	更新処理
	*		Tickはここにしか存在しない
	*/
	virtual void Tick(float DeltaTime) override;

	/**
	*	
	*/
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);

private:
	/** ゲームフロー関連 */
	UPROPERTY(EditDefaultsOnly, Category = "GameFlow")
	float PlayTimeLimit = 30.f;

	float ElapsedTime = 0.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "GameFlow")
	FName ResultLevelName = "Lvl_Result";
	
	/** Player関連 */

	UPROPERTY()
	UPlayerObject* Player;

	UPROPERTY()
	AShipVisual* PlayerVisual;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	TSubclassOf<AShipVisual> PlayerVisualClass;


	/** Bullet関連 */

	UPROPERTY()
	UBulletManager* BulletManager;

	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	TSubclassOf<ABulletVisual> BulletVisualClass;

	/** Stage関連 */

	UPROPERTY()
	UStageManager* StageManager;

	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	TSubclassOf<ARockVisual> RockVisualClass;

	/** イベント関連 */

	UPROPERTY()
	UEventBus* EventBus;

	UPROPERTY()
	USoundSystem* SoundSystem;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* CollisionSound;

	UPROPERTY()
	UEffectSystem* EffectSystem;

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	UNiagaraSystem* CollisionEffect;

	UPROPERTY()
	UScoreSystem* ScoreSystem;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UScoreWidget> ScoreWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	UTimeWidget* TimeWidget;
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UTimeWidget> TimeWidgetClass;

};