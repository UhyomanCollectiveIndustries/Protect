#include "Migration/CollisionSystem.h"

#include "Migration/BulletManager.h"
#include "Migration/StageManager.h"

#include "Migration/PlayerObject.h"
#include "Migration/CustomCollisionEvent.h"

#include "EnemyManager.h"

/**
*	弾とステージオブジェクトの当たり判定の計算
*/
void FCollisionSystem::CheckBulletVsStage(
	UBulletManager& Bullets,
	UStageManager& Stage,
	TArray<FCustomCollisionEvent>& OutEvents)
{
	/** アクティブな弾を探す */
	for (UBulletObject* Bullet : Bullets.GetBulletsPool())
	{
		if (!Bullet->bIsActive) continue;

		/** アクティブなステージオブジェクトを探す */
		for (UStageObject* Obj : Stage.StageObjects)
		{
			if (!Obj->bIsActive || Obj->bIsHit) continue;

			/** 弾とステージオブジェクトの距離計算 */
			float Dist = FVector::Dist(
				Bullet->Transform.GetLocation(),
				Obj->Transform.GetLocation()
			);

			/** 距離がステージオブジェクトの半径以下(衝突した)だったら*/
			if (Dist < Obj->Radius)
			{
				Bullet->bIsActive = false;

				Obj->bIsHit = true;
				Obj->bIsActive = false;

				FCustomCollisionEvent Event;
				Event.Bullet = Bullet;
				Event.StageObject = Obj;
				OutEvents.Add(Event);

				break;
			}
		}
	}
}


void FCollisionSystem::CheckBulletVsEnemy(
	UBulletManager& Bullets,
	UEnemyManager& Enemies,
	TArray<FCustomCollisionEvent>& OutEvents)
{
	/** アクティブな弾を探す */
	for (UBulletObject* Bullet : Bullets.GetBulletsPool())
	{
		if (!Bullet->bIsActive) continue;

		/** アクティブな敵を探す */
		for (UEnemyObject* Enemy : Enemies.GetActiveEnemies())
		{
			if (!Enemy->bIsActive || !Enemy->bCollidable) continue;

			/** 弾と敵の距離計算 */
			float Dist = FVector::Dist(
				Bullet->Transform.GetLocation(),
				Enemy->Transform.GetLocation()
			);

			/** 距離がステージオブジェクトの半径以下(衝突した)だったら*/
			if (Dist < Enemy->Radius)
			{
				Bullet->bIsActive = false;
				Enemy->SubtractHp(1);

				FCustomCollisionEvent Event;
				Event.Bullet = Bullet;
				Event.EnemyObject = Enemy;
				Event.bEnemyKilled = (Enemy->GetCurrentHp() <= 0);	/** 基本trueが入る */
				OutEvents.Add(Event);

				break;
			}
		}
	}
}

void FCollisionSystem::CheckPlayerVsEnemy(
	UPlayerObject& Player,
	UEnemyManager& Enemies,
	TArray<FCustomCollisionEvent>& OutEvents)
{
	/** プレイヤが無敵時間なら、判定を行わない */
	if (Player.IsInvincible()) return;

	/** アクティブな敵を探す */
	for (UEnemyObject* Enemy : Enemies.GetActiveEnemies())
	{
		if (!Enemy->bIsActive || !Enemy->bCollidable) continue;

		/** プレイヤと敵の距離計算 */
		float Dist = FVector::Dist(
			Player.Transform.GetLocation(),
			Enemy->Transform.GetLocation()
		);

		/** 距離がステージオブジェクトの半径以下(衝突した)だったら*/
		if (Dist < Enemy->Radius + Player.Radius)
		{
			Player.SubtractHp(1);

			FCustomCollisionEvent Event;
			Event.EnemyObject = Enemy;
			Event.PlayerObject = &Player;
			OutEvents.Add(Event);

			break;
		}
	}
}