#include "PlayerCamera.h"


/**
* コンストラクタ
*	カメラコンポーネントを作成し、初期値を設定する
*/
APlayerCamera::APlayerCamera()
{
	PrimaryActorTick.bCanEverTick = false;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	RootComponent = CameraComp;
	CurrentFOV = BaseFOV;
}

/**
* 更新処理
*	プレイヤの位置・速度に応じてカメラの位置・注視点・FOVを更新する
*/
void APlayerCamera::Update(float DeltaTime, const FTransform& PlayerTransform, float PlayerSpeed)
{
	FVector PlayerPos = PlayerTransform.GetLocation();

	// 追従を遅延つきで計算
	FVector TargetCameraPos = PlayerPos + FollowOffset;
	FVector CurrentCameraPos = GetActorLocation();
	FVector NewCameraPos = FMath::VInterpTo(CurrentCameraPos, TargetCameraPos, DeltaTime, FollowInterSpeed);
	SetActorLocation(NewCameraPos);

	// 注視点をプレイヤより前方・下にずらし、画面中央をそこへ向ける
	FVector LookAtPos = PlayerPos + LookOffset;
	FRotator NewRot = (LookAtPos - NewCameraPos).Rotation();
	SetActorRotation(NewRot);

	// 速度に応じてFOVを広げる
	float SpeedRatio = FMath::Clamp(PlayerSpeed / MaxSpeedReference, 0.f, 1.f);
	float TargetFOV = BaseFOV + MaxFOVBonus * SpeedRatio;
	CurrentFOV = FMath::FInterpTo(CurrentFOV, TargetFOV, DeltaTime, 2.0f);
	CameraComp->SetFieldOfView(CurrentFOV);
}
