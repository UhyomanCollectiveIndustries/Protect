#include "Migration/PlayerObject.h"


/** 初期化処理 */
void UPlayerObject::Init()
{
	/** 
	*	モデルの回転とスケールの設定
	*		[feature]ステージごとに、モデルの向きや大きさが違うので、ここで設定する形に変更
	*/
	ModelTransform.SetRotation(FRotator(0.f, 0.f, 0.f).Quaternion());
	ModelTransform.SetScale3D(FVector(0.1f));
	SprintStamina = MaxSprintStamina;

	CurrentHp = MaxHp;
	InvincibleTimer = 0.f;

	if (PlayerHPWidget)
	{
		PlayerHPWidget->SetHPPercentToBar(CurrentHp / MaxHp);
	}
}

/**
*	更新処理
*		主に入力に応じた処理を行う
*/
void UPlayerObject::Update(float DeltaTime)
{
	FVector Pos = Transform.GetLocation();

	if (InvincibleTimer > 0.f)
	{
		InvincibleTimer -= DeltaTime;
	}

	if (InputSprint > 0.f && bIsSprint)
	{
		MoveSpeed = FMath::FInterpTo(MoveSpeed, 1000.f, DeltaTime, 10.0f);

		SprintStamina -= DeltaTime * 20.f;

		if (SprintStamina <= 0)
		{
			SprintStamina = 0.f;
			bIsSprint = false;
		}
	}
	else if (InputSprint < 0.f && bIsSprint)
	{
		MoveSpeed = FMath::FInterpTo(MoveSpeed, 10.f, DeltaTime, 10.0f);

		SprintStamina -= DeltaTime * 20.f;
		if (SprintStamina <= 0)
		{
			SprintStamina = 0.f;
			bIsSprint = false;
		}
	}
	else
	{
		MoveSpeed = FMath::FInterpTo(MoveSpeed, 200.f, DeltaTime, 5.0f);

		if (SprintStamina >= MaxSprintStamina)
		{
			SprintStamina = MaxSprintStamina;
		}
		else
		{
			SprintStamina += DeltaTime * 15.f;
		}

		if (SprintStamina >= MaxSprintStamina && !bIsSprint)
		{
			bIsSprint = true;
		}
	}

	if (PlayerStaminaWidget)
	{
		PlayerStaminaWidget->SetStaminaPercent(SprintStamina / MaxSprintStamina);
	}

	/**
	*	常に前進
	*		[feature]ゲームモードによって操作方法を変更(スピードアップ/ダウンの実装も検討)
	*/
	Pos.X += MoveSpeed * DeltaTime;

	/**
	*	上下移動を範囲内にクランプ
	*/
	Pos.Z += InputV * MoveSpeed * DeltaTime;
	Pos.Z = FMath::Clamp(Pos.Z, -50.f, 550.f);
	
	/**
	*	移動に伴う、回転処理
	*		右入力->右に傾く
	*		左入力->左に傾く
	*		入力がない場合->元の角度に戻る
	*/
	FRotator Rot = Transform.Rotator();
	const float BankSpeed = 120.0f;
	const float MaxBank = 20.0f;

	if (InputH < 0.f)	/** 左の入力 */
	{
		Pos.Y += InputH * MoveSpeed * DeltaTime;
		Rot.Roll = FMath::Clamp(Rot.Roll - BankSpeed * DeltaTime, -MaxBank, MaxBank);
	}
	else if (InputH > 0.f)	/** 右の入力 */
	{
		Pos.Y += InputH * MoveSpeed * DeltaTime;
		Rot.Roll = FMath::Clamp(Rot.Roll + BankSpeed * DeltaTime, -MaxBank, MaxBank);
	}
	else /** 入力なし */
	{
		Rot.Roll *= FMath::Pow(0.01f, DeltaTime);
		if (FMath::Abs(Rot.Roll) < 0.1f) Rot.Roll = 0.f;
	}

	/**
	*	左右移動を範囲内にクランプ
	*/
	Pos.Y = FMath::Clamp(Pos.Y, -550.f, 550.f);

	Transform.SetLocation(Pos);
	Transform.SetRotation(Rot.Quaternion());
}

void UPlayerObject::SetInputAxis(float Horizontal, float Vertical)
{
	InputH = Horizontal;
	InputV = Vertical;
}

void UPlayerObject::SetInputSprint(float Sprint)
{
	InputSprint = Sprint;
}

void UPlayerObject::SubtractHp(int32 Amount)
{
	CurrentHp = FMath::Max(CurrentHp - Amount, 0);
	InvincibleTimer = InvincibleDuration;

	if (PlayerHPWidget)
	{
		PlayerHPWidget->SetHPPercentToBar((float)CurrentHp / MaxHp);
	}
}
