#include "DebugStatusSystem.h"

#include "Migration/BulletManager.h"
#include "HAL/PlatformMemory.h"		// OSからメモリ情報の取得
#include "HAL/PlatformTime.h"		// 実時間の取得
#include "UObject/UObjectArray.h"


/**
 * 初期化処理
 *		参照を保持し、開始時点のメモリを計測
 */
void UDebugStatusSystem::Init(UWorld* InWorld,UBulletManager* InBulletManager)
{
	World = InWorld;
	BulletManager = InBulletManager;
	SampleMemory();	// 開始点の値を記録する(StartMemoryMBを確定させる)
}

/**
 *	更新処理
 *		1.フレーム時間を測って履歴に追加
 *		2.一定間隔ごとに、フレーム時間の統計を再計算
 *		3.一定間隔ごとに、メモリと弾プールをサンプリング
 */
void UDebugStatusSystem::Update(float DeltaTime)
{
	/**
	 *	フレーム時間の計測
	 *		DeltaTimeはエンジン側で上限を掛けられていたり、時間の流れを変更できたりするため、
	 *		ワールドのDeltaTimeではなく、実時間(の差分)で測る
	 */
	const double Now = FPlatformTime::Seconds();
	if (bHasLastTick)
	{
		// (今回の時刻-前回の時刻) = 1フレームに掛かった時間(秒)->*1000でミリ秒に変換
		PushSample(FrameTimeHistoryMs,static_cast<float>((Now-LastTickSeconds)*1000.0),MaxFrameSamples);
	}
	// 次のフレームで使うために、今回の時刻を覚えておく
	LastTickSeconds = Now;
	bHasLastTick = true;	// 最初の1フレーム目は比較相手がいないため、記録せずにここで有効化
	
	/**
	 *	フレーム時間の統計を再計算
	 *		統計の計算には、ソートが必要なため、毎フレーム(重い)行わず、0.25秒間隔で行う。
	 */
	StatusTimer += DeltaTime;
	if (StatusTimer >= FrameStatsInterval)
	{
		StatusTimer = 0.f;
		RecalculateFrameStats();
	}
	
	/**
	 *  メモリと弾プールのサンプリング
	 *		弾が増えたと同時に、メモリがどう動いたかを見比べるため、同じタイミングで測る
	 */
	MemoryTimer += DeltaTime;
	if (MemoryTimer >= MemorySampleInterval)
	{
		MemoryTimer = 0.f;
		SampleMemory();
	}
}

/**
 *	履歴への追加
 *		上限に達していたら先頭(最古)を捨ててから、末尾に追加
 */
void UDebugStatusSystem::PushSample(TArray<float>& Buffer,float Value,int32 MaxSamples)
{
	if (Buffer.Num() >= MaxSamples)
    {
    	Buffer.RemoveAt(0);	// 数百要素なので線形コストは問題にならない
    }
	Buffer.Add(Value);
}

/**
 *	メモリ関連のサンプリング
 *		メモリ・Actor数・UObject数を測り、メモリと弾の使用数を履歴に追加する
 */
void UDebugStatusSystem::SampleMemory()
{
	// OSから、このプロセスのメモリ情報を取得
	const FPlatformMemoryStats Stats = FPlatformMemory::GetStats();
	
	/**
	 *	USedPhysical : このプロセスがすでに使っている物理メモリ(バイト)
	 *		バイト->KB->MBに変換するため、1024で除算
	 */
	CurrentMemoryMB = static_cast<float>(Stats.UsedPhysical) / (1024.f * 1024.f);
	
	// StartMemoryMBが-1(未測定)のときのみ、今の値を「開始時点の値」として確定する
	if (StartMemoryMB < 0.f)StartMemoryMB = CurrentMemoryMB;
	PeakMemoryMB = FMath::Max(PeakMemoryMB,CurrentMemoryMB);
	
	/**
	 *	レベル上に存在するActorの数
	 *		Worldが無効(破棄済)の場合は、0にしておく
	 */
	ActorCount = World.IsValid()? World->GetActorCount() : 0;
	
	// (破棄したスロット数-空きスロット数) = 生きているUObjectの数
	UObjectCount = GUObjectArray.GetObjectArrayNumMinusAvailable();
	
	PushSample(MemoryHistoryMB,CurrentMemoryMB,MaxMemorySamples);
	
	// 弾の使用数も同じタイミングで記録(BulletManagerがない場合は0)
	PushSample(ActiveBulletHistory,
		BulletManager ? static_cast<float>(BulletManager->GetActiveCount() ): 0.f,
		MaxMemorySamples);
}

/**
 *	フレーム時間の統計計算
 *		平均・P95・P99・最大を、履歴(直近300フレーム)から求める
 *		
 *	[パーセンタイルの求め方]
 *		1.小さい順にソート
 *		2.P95 = 小さい方から数えて 95%の位置の値　-> index = floor((300-1) * 0.95) = 283
 *			(遅い方から数えて上位5%を除いた中での最大値)
 *		
 *		これにより、平均が良くても、P99・MAXが高ければ「稀に重いフレームがある」と判定できる
 */
void UDebugStatusSystem::RecalculateFrameStats()
{
	if (FrameTimeHistoryMs.Num() == 0) return;
	
	// 元の履歴の時系列を壊さないよう、コピーしてからソート
	TArray<float> Sorted = FrameTimeHistoryMs;
	Sorted.Sort();
	
	// 平均
	float Sum = 0.f;
	for (float V : Sorted) Sum += V;
	AvgFrameMs = Sum/Sorted.Num();
	
	// 添え字が0オリジンのため、「個数-1」を掛ける
	P95FrameMs = Sorted[FMath::FloorToInt((Sorted.Num() - 1)*0.95f)];
	P99FrameMs = Sorted[FMath::FloorToInt((Sorted.Num() - 1)*0.99f)];
	
	// 最後の値が最大値
	MaxFrameMs  = Sorted.Last();
}