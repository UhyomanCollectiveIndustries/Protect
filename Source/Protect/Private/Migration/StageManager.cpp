#include "Migration/StageManager.h"


/**
*	初期化処理
*		@param World
*		@param VisualClass		岩の見た目
*		@param RockPositions	岩の位置
*/
void UStageManager::Init(UWorld* World, TSubclassOf<ARockVisual> VisualClass, const TArray<FVector>& RockPositions)
{
	/**
	*	
	*/
	for (const FVector& Pos : RockPositions)
	{
		/** ステージオブジェクト自体について */

		UStageObject* Obj = NewObject<UStageObject>(this);
		Obj->Init(Pos, 200.0f);
		StageObjects.Add(Obj);

		/** 見た目について */

		ARockVisual* Visual = World->SpawnActor<ARockVisual > (VisualClass);
		Visual->ApplyTransform(Obj->Transform);
		Visual->SetVisualActive(true);
		StageVisuals.Add(Visual);
	}
}

/** 更新処理 */
void UStageManager::Update(float DeltaTime)
{
	for (int32 i = 0; i < StageObjects.Num(); ++i)
	{
		/** ヒット済で非アクティブになった岩は見た目を隠す */
		if (!StageObjects[i]->bIsActive)
		{
			StageVisuals[i]->SetVisualActive(false);
		}
	}
}