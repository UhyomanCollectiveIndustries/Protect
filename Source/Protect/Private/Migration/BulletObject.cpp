#include "Migration/BulletObject.h"


/**
*	発射時の初期化
*　		BulletManager::Fireに呼ばれる
*		アクティブをオンにし、プレイヤの位置まで移動
*
*		@param StartPos		発射位置
*		@param Dir			発射方向
*/
void UBulletObject::FireInit(const FVector& StartPos, const FVector& Dir)
{
	bIsActive = true;
	Transform.SetLocation(StartPos);
	Velocity = Dir * 1000.f; /** 速度を設定 */
	LifeTimer = BulletLifeTime; /** 寿命をリセット */
}

/**
*	更新処理
*		フレーム毎の弾の位置の計算と適用
*		寿命タイマーを減らす
*/
void UBulletObject::Update(float DeltaTime)
{
	/** 通常アクティブでない弾は呼ばれないが、念のためアクティブかどうか判定する */
	if (!bIsActive) return;

	FVector Pos = Transform.GetLocation();
	Pos += Velocity * DeltaTime;
	Transform.SetLocation(Pos);

	LifeTimer -= DeltaTime;
	if (LifeTimer <= 0.f)
	{
		bIsActive = false;	/** 実際のプール返却は、BulletManager::Updateで行うが、ここでも明示的に無効化する */
	}
}