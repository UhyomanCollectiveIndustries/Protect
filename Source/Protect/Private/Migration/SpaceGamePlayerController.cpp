#include "Migration/SpaceGamePlayerController.h"

#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>
#include <Kismet/GameplayStatics.h>


void ASpaceGamePlayerController::BeginPlay()
{
	Super::BeginPlay();

	/**
	*	マッピングコンテキストの追加
	*/
	if (ULocalPlayer* LP = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	/**
	*	GameManagerの取得
	*		移動入力の受け渡し
	*/
	GameManager = Cast<AGameManager>(
		UGameplayStatics::GetActorOfClass(GetWorld(), AGameManager::StaticClass()));
}


void ASpaceGamePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASpaceGamePlayerController::HandleMove);
		EIC->BindAction(MoveAction, ETriggerEvent::Completed, this, &ASpaceGamePlayerController::HandleMoveCompleted);

		EIC->BindAction(SprintAction, ETriggerEvent::Triggered, this, &ASpaceGamePlayerController::HandleSprint);
		EIC->BindAction(SprintAction, ETriggerEvent::Completed, this, &ASpaceGamePlayerController::HandleSprintCompleted);

		EIC->BindAction(FireAction, ETriggerEvent::Started, this, &ASpaceGamePlayerController::HandleFire);
	}
}

void ASpaceGamePlayerController::HandleMove(const FInputActionValue& Value)
{
	FVector2D Axis = Value.Get<FVector2D>();
	if (GameManager && GameManager->GetPlayerObject())
	{
		GameManager->GetPlayerObject()->SetInputAxis(Axis.X, Axis.Y);
	}
}

void ASpaceGamePlayerController::HandleMoveCompleted(const FInputActionValue& Value)
{
	if (GameManager && GameManager->GetPlayerObject())
	{
		GameManager->GetPlayerObject()->SetInputAxis(0.f, 0.f);
	}
}

void ASpaceGamePlayerController::HandleSprint(const FInputActionValue& Value)
{
	float Sprint = Value.Get<float>();
	if (GameManager && GameManager->GetPlayerObject())
	{
		GameManager->GetPlayerObject()->SetInputSprint(Sprint);
	}
}

void ASpaceGamePlayerController::HandleSprintCompleted(const FInputActionValue& Value)
{
	if (GameManager && GameManager->GetPlayerObject())
	{
		GameManager->GetPlayerObject()->SetInputSprint(0.f);
	}
}

void ASpaceGamePlayerController::HandleFire(const FInputActionValue& Value)
{
	if (GameManager && GameManager->GetBulletManager())
	{
		FVector FirePos = GameManager->GetPlayerObject()->Transform.GetLocation();
		FVector FireDir = FVector(1.f, 0.f, 0.f) * 30.f;	/** 前方向[feature]プレイヤの向きに合わせるために、進行方向に変更する */
		GameManager->GetBulletManager()->Fire(FirePos, FireDir);
	}
}