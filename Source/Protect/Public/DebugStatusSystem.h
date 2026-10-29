#pragma once

#include "CoreMinimal.h"
#include "DebugStatusSystem.generated.h"

class UBulletManager;

/**
 * デバック用の計測データを集めるクラス
 *	(表示はDebugHUDクラスが行う)	
 */
UCLASS()
class PROTECT_API UDebugStatusSystem : public UObject
{
	GENERATED_BODY()
	
public:
	// 初期化処理	GameManager::BeginPlay()で、BulletManager初期化後に呼ぶ
	void Init(UWorld* InWorld,UBulletManager* InBulletManager);
	// 更新処理処理	GameManager::Tick()から呼ぶ
	void Update(float DeltaTime);
	
	// --------メモリ関連ゲッター----------
	
	// メモリ使用量(MB)の履歴。グラフ描画に使用
	const TArray<float>& GetMemoryHistoryMB() const {return MemoryHistoryMB;}
	// 使用中の弾の数の履歴。時間軸を揃えて比較するため、メモリと同じタイミングで記録する
	const TArray<float>& GetActiveBulletHistory() const { return ActiveBulletHistory; }
	// 現在のメモリ使用量(MB)
	float GetCurrentMemoryMB() const { return CurrentMemoryMB; }
	// ゲーム開始時点のメモリ使用量(MB)、開始からどれだけ増えたかの基準とする
	float GetStartMemoryMB() const { return StartMemoryMB; }
	// 今までのメモリ最大使用量(MB)
	float GetPeakMemoryMB() const { return PeakMemoryMB; }
	// レベル上に存在する Actor の数
	int32 GetActorCount() const { return ActorCount; }
	// 存在する UObject の数
	int32 GetUObjectCount() const { return UObjectCount; }
	
	// --------フレーム時間ゲッター----------
	
	// フレームの履歴。グラフ描画に使用
	const TArray<float>& GetFrameTimeHistoryMs() const { return FrameTimeHistoryMs; }
	float GetAvgFrameMs() const { return AvgFrameMs; }	// 平均
	float GetP95FrameMs() const { return P95FrameMs; }	// 95パーセンタイル
	float GetP99FrameMs() const { return P99FrameMs; }	// 99パーセンタイル
	float GetMaxFrameMs() const { return MaxFrameMs; }	// 最大フレーム数
	
private:
	// 履歴の末尾に値を追加する。上限を超える場合は、先頭を捨てる
	void PushSample(TArray<float>& Buffer,float Value,int32 MaxSamples);
	// メモリ・Actor数・UObject数・弾の使用数を1回分測り、履歴に追加する
	void SampleMemory();
	// フレーム時間の履歴から、平均・P95・P99・最大を計算する
	void RecalculateFrameStats();

	// 定数郡
	static constexpr  int32 MaxFrameSamples = 300;			// 保存するフレーム数(直近300フレーム)
	static constexpr int32 MaxMemorySamples = 240;			// 0.5秒間隔 × 240 = 約2分
	static constexpr float MemorySampleInterval = 0.5f;		// メモリを測る間隔
	static constexpr float FrameStatsInterval = 0.25f;		// 統計の再計算間隔(平均/P95など)
	
	/**
	 *UWorldへの弱参照。
	 * ワールドが破棄されたとき、このクラスが残っている場合、安全に処理される
	 */
	TWeakObjectPtr<UWorld>World;
	
	// 弾プールの使用数を聞くために保持する。(GCで回収されないよう、UPROPERTY())
	UPROPERTY()
	UBulletManager* BulletManager = nullptr;
	
	// 履歴を格納するTArray。(先頭が最古、末尾が最新)
	TArray<float> MemoryHistoryMB;
	TArray<float> ActiveBulletHistory;
	TArray<float> FrameTimeHistoryMs;
	
	// 最新のメモリ関連の値
	float CurrentMemoryMB = 0.f;
	float StartMemoryMB = -1.f;	// -1のとき、「まだ測っていない=最初のサンプリング」と確定
	float PeakMemoryMB = 0.f;
	int32 ActorCount = 0;
	int32 UObjectCount =0;
	
	// 最新のフレーム時間の統計(RecalculateFrameStats()で更新する)
	float AvgFrameMs = 0.f;
	float P95FrameMs = 0.f;
	float P99FrameMs = 0.f;
	float MaxFrameMs = 0.f;
	
	// 経過時間を溜めて、一定間隔ごとに処理を行うためのタイマー
	float MemoryTimer = 0.f;
	float StatusTimer = 0.f;
	
	// 前フレームのTick時刻(実時間・秒)。今回との差でフレーム時間を計算する
	double LastTickSeconds = 0.0;
	// LastTickSecondsに有効な値が入っているか(最初の1フレームは比較相手がいない)
	bool bHasLastTick = false;
};
