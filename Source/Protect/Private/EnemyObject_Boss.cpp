#include "EnemyObject_Boss.h"
#include "BossWeakPoint.h"

UEnemyObject_Boss::UEnemyObject_Boss()
{
	// ボス本体は弾を受けない(当たり判定は弱点だけが持つ)
	bCollidable = false;
}

void UEnemyObject_Boss::Init(const FVector& Pos, float InRadius, int32 Hp)
{
	Super::Init(Pos, InRadius, Hp);

	// スポーン位置のY/Zを、揺れの中心として覚えておく
	BaseY = Pos.Y;
	BaseZ = Pos.Z;
}

void UEnemyObject_Boss::RegisterPart(UBossWeakPoint* Part)
{
	Parts.Add(Part);
}

bool UEnemyObject_Boss::AreAllPartsDestroyed() const
{
	// 弱点が1つも登録されていない場合は「全滅」とみなさない(生成直後の誤判定を防ぐ)
	if (Parts.Num() == 0) return false;

	for (const UBossWeakPoint* Part : Parts)
	{
		if (Part && Part->bIsActive) return false;
	}
	return true;
}

/**
 *	更新処理
 *		基底のUpdate()は「HPが0なら消える」「背後に回ったら消える」判定を持つが、
 *		ボスは弱点の全滅で撃破となり、背後に回ることもないので呼ばない
 */
void UEnemyObject_Boss::Update(float DeltaTime, const FVector& PlayerPos,const FVector& PlayerCameraPos)
{
	if (!bIsActive) return;

	// 弱点が全て壊れたら撃破
	if (AreAllPartsDestroyed())
	{
		bIsActive = false;
		return;
	}

	ElapsedTime += DeltaTime;

	FVector Pos = Transform.GetLocation();

	// X: プレイヤーの前方 KeepDistance の位置を目標に、補間しながら追従する
	//	出現時は遠くにスポーンさせておけば、自然に近づいてくる演出になる
	const float TargetX = PlayerPos.X + KeepDistance;
	Pos.X = FMath::FInterpTo(Pos.X, TargetX, DeltaTime, FollowInterpSpeed);

	// Y/Z: 基準位置を中心にゆっくり揺れる
	Pos.Y = BaseY + FMath::Sin(ElapsedTime * SwaySpeedY) * SwayAmplitudeY;
	Pos.Z = BaseZ + FMath::Sin(ElapsedTime * SwaySpeedZ) * SwayAmplitudeZ;

	Transform.SetLocation(Pos);
}