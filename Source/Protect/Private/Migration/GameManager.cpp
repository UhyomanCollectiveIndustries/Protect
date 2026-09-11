#include "Migration/GameManager.h"

#include "Migration/CollisionSystem.h"
#include "Migration/ScoreWidget.h"
#include "Blueprint/UserWidget.h"
#include "Migration/ProtectGameInstance.h"
#include <Kismet\GameplayStatics.h>


/** コンストラクタ */
AGameManager::AGameManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

/**
*	初期化処理
*		各オブジェクトの初期化を行う
*/
void AGameManager::BeginPlay()
{
	Super::BeginPlay();

	/** Playerの初期化 */
	Player = NewObject<UPlayerObject>(this);
	Player->Init();

	PlayerVisual = GetWorld()->SpawnActor<AShipVisual>(PlayerVisualClass);

	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		PC->SetViewTarget(PlayerVisual);
	}

	/** Bulletの初期化 */
	BulletManager = NewObject<UBulletManager>(this);
	BulletManager->Init(GetWorld(),BulletVisualClass);

	/** Stageの初期化 */
	StageManager = NewObject<UStageManager>(this);

	FBox SpawnArea(FVector(1000.f, -500.f, 0.f), FVector(6500.f, 500.f,500.f));

	TArray<FVector> RockPositions;
	for (int32 i = 0; i < 30; ++i)
	{
		FVector RandomLocation = FMath::RandPointInBox(SpawnArea);
		RockPositions.Add(RandomLocation);
	}



	StageManager->Init(GetWorld(), RockVisualClass, RockPositions);

	/** イベント関連 */
	EventBus = NewObject<UEventBus>(this);

	SoundSystem = NewObject<USoundSystem>(this);
	SoundSystem->Init(GetWorld(), CollisionSound);

	EffectSystem = NewObject<UEffectSystem>(this);
	EffectSystem->Init(GetWorld(), CollisionEffect);

	ScoreSystem = NewObject<UScoreSystem>(this);

	/**
	*	イベント発火時のシステム郡
	*/
	EventBus->OnCollision.AddUObject(SoundSystem, &USoundSystem::HandleCollision);
	EventBus->OnCollision.AddUObject(EffectSystem, &UEffectSystem::HandleCollision);
	EventBus->OnCollision.AddUObject(ScoreSystem, &UScoreSystem::HandleCollision);

	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		if (UScoreWidget* ScoreWidget = CreateWidget<UScoreWidget>(PC, ScoreWidgetClass))
		{
			ScoreWidget->AddToViewport();
		}
		TimeWidget = CreateWidget<UTimeWidget>(PC, TimeWidgetClass);
		if(TimeWidget)
		{
			TimeWidget->AddToViewport();
		}
	}

	ElapsedTime = PlayTimeLimit;
}

/**
*	更新処理
*		各オブジェクトの更新を行う
*/
void AGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/** Playerの更新 */
	Player->Update(DeltaTime);
	PlayerVisual->ApplyTransform(Player->Transform, Player->ModelTransform);

	/**
	*	Bulletの更新
	*		Bulletの位置の更新などはBulletManager側で行う
	*/
	BulletManager->Update(DeltaTime);

	/** ステージの更新 */
	StageManager->Update(DeltaTime);

	TArray<FCustomCollisionEvent> Events;
	FCollisionSystem::CheckBulletVsStage(*BulletManager, *StageManager, Events);

	for (const FCustomCollisionEvent& Event : Events)
	{
		EventBus->Publish(Event);
	}

	ElapsedTime -= DeltaTime;
	if (TimeWidget)
	{
		TimeWidget->ApplyTime(ElapsedTime);
	}

	/** 画面フロー */
	if (ElapsedTime <= 0)
	{
		/** GameInstanceのFinalScoreにScoreSytemが持っているスコアを渡す */
		if (UProtectGameInstance * GI = Cast<UProtectGameInstance>(GetGameInstance()))
		{
			GI->FinalScore = ScoreSystem ? ScoreSystem->GetScore() : 0;
		}

		UGameplayStatics::OpenLevel(this, ResultLevelName);
	}

}

void AGameManager::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{

}