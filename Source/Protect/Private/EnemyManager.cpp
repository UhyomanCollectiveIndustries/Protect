#include "EnemyManager.h"

#include "EnemyObject_Boss.h"
#include "BossWeakPoint.h"
#include "DrawDebugHelpers.h"


void UEnemyManager::Init(UWorld* InWorld, const TArray<FEnemyPhase>& InPhases)
{
	World = InWorld;
	Phases = InPhases;
}

/**
*	更新処理
*		プレイヤのX座標に応じて、フェーズを開始する
*			(開始済のものは、無視する)
* 
*		敵1体分の更新も行う		
*/
void UEnemyManager::Update(float DeltaTime, const FVector& PlayerPos,const FVector& PlayerCameraPos)
{
	// フェーズ発動
	for (FEnemyPhase& Phase : Phases)
	{
		if (!Phase.bTriggered && PlayerPos.X >= Phase.TriggerX)
		{
			Phase.bTriggered = true;
			TriggerPhase(Phase);
		}
	}
	
	// 全員のUpdate(弱点パーツの1フレームのズレをなくすため)
	for (int32 i=0;i<ActiveObjects.Num();++i)
	{
		ActiveObjects[i]->Update(DeltaTime,PlayerPos,PlayerCameraPos);
	}

	// 非アクティブな敵の破棄・見た目への反映
	for (int32 i = ActiveObjects.Num() - 1; i >= 0; --i)
	{
		if (!ActiveObjects[i]->bIsActive)
		{
			ActiveVisuals[i]->Destroy();
			ActiveObjects.RemoveAt(i);
			ActiveVisuals.RemoveAt(i);
			continue;
		}

		ActiveVisuals[i]->ApplyTransform(ActiveObjects[i]->Transform);
	}
#if !UE_BUILD_SHIPPING
	if (bDrawCollisionDebug && World.IsValid())
	{
		for (const UEnemyObject* Enemy : ActiveObjects)
		{
			if (Enemy->bCollidable)
			{
				DrawDebugSphere(World.Get(), Enemy->Transform.GetLocation(), Enemy->Radius, 12, FColor::Red);
			}
		}
	}
#endif
}

void UEnemyManager::TriggerPhase(FEnemyPhase& Phase)
{
	if (!World.IsValid())return;

	for (const FEnemySpawnInfo& Info : Phase.Enemies)
	{
		UEnemyObject* Enemy = SpawnEnemy(Info.EnemyClass,Info.VisualClass,Info.SpawnPosition,Info.Radius,Info.Hp);
		if (!Enemy)continue;
		
		// ボスなら弱点パーツを本体の直後に生成して紐づける
		if (UEnemyObject_Boss* Boss = Cast<UEnemyObject_Boss>(Enemy))
		{
			for (const FBossPartInfo& PartInfo : Info.Parts)
			{
				UBossWeakPoint* Part = Cast<UBossWeakPoint>(SpawnEnemy(
					UBossWeakPoint::StaticClass(),PartInfo.VisualClass,
					Info.SpawnPosition + PartInfo.LocalOffSet,PartInfo.Radius,PartInfo.Hp));
				
				if (Part)
				{
					Part->SetupPart(Boss,PartInfo.LocalOffSet);
					Boss->RegisterPart(Part);
				}
			}
		}
	}
}

UEnemyObject* UEnemyManager::SpawnEnemy(TSubclassOf<UEnemyObject>EnemyClass,TSubclassOf<AEnemyVisual>VisualClass,
	const FVector& Pos,float Radius,int32 Hp)
{
	if (!World.IsValid() || !EnemyClass) return nullptr;
	
	UEnemyObject* Enemy = NewObject<UEnemyObject>(this,EnemyClass);
	Enemy->Init(Pos,Radius,Hp);
	ActiveObjects.Add(Enemy);
	
	AEnemyVisual* Visual = World->SpawnActor<AEnemyVisual>(VisualClass);
	Visual->ApplyTransform(Enemy->Transform);
	Visual->SetVisualActive(true);
	ActiveVisuals.Add(Visual);
	
	return Enemy;
}
