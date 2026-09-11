#include "Migration/ShipVisual.h"

/** コンストラクタ */
AShipVisual::AShipVisual()
{
	PrimaryActorTick.bCanEverTick = false;	/** Tickは持たせないため、無効化 */

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootScene);

	/**
	*	カメラの設定
	*		[featrue]カメラとプレイヤを分けて作成する
	*/
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(RootScene);
	CameraComp->SetRelativeLocation(FVector(-300.0f, 0.f, 100.f));	 /** 自機の後ろかつ上 */
	CameraComp->SetRelativeRotation(FRotator(-10.f, 0.f, 0.f));     /** 見下ろし */
}

void AShipVisual::ApplyTransform(const FTransform& WorldTransform, const FTransform& ModelOffset)
{
	SetActorTransform(WorldTransform);				/** アクター本体 */
	MeshComp->SetRelativeTransform(ModelOffset);	/** モデルの向き・スケール */
}