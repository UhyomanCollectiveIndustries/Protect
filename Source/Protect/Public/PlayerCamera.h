#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"

#include "PlayerCamera.generated.h"


/**
 *	プレイヤー追従カメラクラス
 *		プレイヤーの位置に応じてカメラを追従させる
 */
UCLASS()
class PROTECT_API APlayerCamera : public AActor
{
	GENERATED_BODY()
	
public:	
	// コンストラクタ
	APlayerCamera();

	// 更新処理
	void Update(float DeltaTime, const FTransform& PlayerTransform, float PlayerSpeed);

private:
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	// 追従のオフセット(プレイヤからみた相対位置)
	UPROPERTY(EditDefaultsOnly,Category="Camera")
	FVector FollowOffset = FVector(-400.f, 0.f, 150.f); // 後方かつ上

	// 画面内でプレイヤを少ししたに移すための注視店のオフセット
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	FVector LookOffset = FVector(400.f, 0.f, -100.f);	// 前方かつ下

	// 追従の後れ具合
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float FollowInterSpeed = 3.0f;

	// FOVの基本値
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float BaseFOV = 90.0f;

	// FOVの速度に応じた最大値
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float MaxFOVBonus = 20.0f;

	// どの速度でFOVボーナスが最大になるか(PlayerSpeedの最高速度に合わせる)
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float MaxSpeedReference = 1000.0f;

	float CurrentFOV = 90.0f;
};
