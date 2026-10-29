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
	PlayerCamera = GetWorld()->SpawnActor<APlayerCamera>(PlayerCameraClass);

	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		PC->SetViewTarget(PlayerCamera);

		if (UPlayerStaminaWidget* StaminaWidget = CreateWidget<UPlayerStaminaWidget>(PC, StaminaWidgetClass))
		{
			StaminaWidget->AddToViewport();
			Player->SetStaminaWidget(StaminaWidget);
		}
		if (UPlayerHPWidget* HPWidget = CreateWidget<UPlayerHPWidget>(PC, HPWidgetClass))
		{
			HPWidget->AddToViewport();
			Player->SetHPWidget(HPWidget);
		}
	}

	/** Bulletの初期化 */
	BulletManager = NewObject<UBulletManager>(this);
	BulletManager->Init(GetWorld(),BulletVisualClass);

	/** Stageの初期化 */
	StageManager = NewObject<UStageManager>(this);
	
	for (const FStageObjectGroup& Group : StageObjectGroups)
	{
		StageManager->SpawnGroup(GetWorld(),Group);
	}
	
	
	TArray<AActor*> Found;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFollowScenery::StaticClass(), Found);
	
	for (AActor* A : Found)
	{
		FollowSceneries.Add(Cast<AFollowScenery>(A));
	}

	/** 敵の初期化 */
	EnemyManager = NewObject<UEnemyManager>(this);
	EnemyManager->Init(GetWorld(), EnemyPhases);

	/** イベント関連 */
	EventBus = NewObject<UEventBus>(this);

	SoundSystem = NewObject<USoundSystem>(this);
	SoundSystem->Init(GetWorld(), CollisionSound_Player,CollisionSound_Enemy);

	EffectSystem = NewObject<UEffectSystem>(this);
	EffectSystem->Init(GetWorld(),CollisionEffect_Player,CollisionEffect_Enemy);

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
	
	/* デバックウィンドウ関連 BulletManagerの初期化後 */
	DebugStats = NewObject<UDebugStatusSystem>(this);
	DebugStats->Init(GetWorld(),BulletManager);

	ElapsedTime = PlayTimeLimit;
}

/**
*	更新処理
*		各オブジェクトの更新を行う
*/
void AGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DebugStats->Update(DeltaTime);

	/** Playerの更新 */
	Player->Update(DeltaTime);
	PlayerVisual->ApplyTransform(Player->Transform, Player->ModelTransform);
	PlayerCamera->Update(DeltaTime, Player->Transform, Player->GetCurrentSpeed());

	/**
	*	Bulletの更新
	*		Bulletの位置の更新などはBulletManager側で行う
	*/
	BulletManager->Update(DeltaTime);

	/** ステージの更新 */
	StageManager->Update(DeltaTime);
	
	for (AFollowScenery* S : FollowSceneries)
	{
		S->Update(Player->Transform.GetLocation());
	}

	/** 敵の更新 */
	EnemyManager->Update(DeltaTime, Player->Transform.GetLocation(),PlayerCamera->GetActorLocation());

	TArray<FCustomCollisionEvent> Events;
	FCollisionSystem::CheckBulletVsStage(*BulletManager, *StageManager, Events);

	FCollisionSystem::CheckBulletVsEnemy(*BulletManager, *EnemyManager, Events);

	FCollisionSystem::CheckPlayerVsEnemy(*Player, *EnemyManager, Events);

	for (const FCustomCollisionEvent& Event : Events)
	{
		EventBus->Publish(Event);
	}

	/** ゲームタイム計算/UIへ適用 */
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

	/** デモ:プレイヤのHPが0以下になったら */
	if (Player->GetCurrentHp() <= 0)
	{
		// GameInstanceのFinalScoreにScoreSystemが持っているスコアを渡す
		if (UProtectGameInstance* GI = Cast<UProtectGameInstance>(GetGameInstance()))
		{
			GI->FinalScore = ScoreSystem ? ScoreSystem->GetScore() : 0;
		}

		UGameplayStatics::OpenLevel(this, ResultLevelName);
	}

}

void AGameManager::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{

}