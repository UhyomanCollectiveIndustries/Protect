#include "DebugHUD.h"

#include "Migration/GameManager.h"
#include "DebugStatusSystem.h"
#include "Migration/BulletManager.h"
#include "Engine/Canvas.h"
#include "Kismet/GameplayStatics.h"


/**
 *	グラフの縦軸の範囲を求める
 *		履歴の最小値〜最大値がグラフにちょうど収まるようにする
 *			そのままだと、値がほとんど変化しない時に縦軸の幅が0になり線が潰れるので、
 *			最低でも1(MB)の幅を確保し、上下に少し余白を付ける
 */
void ADebugHUD::GetMinMax(const TArray<float>& Samples,float& OutMin,float& OutMax)
{
	
	OutMin = FLT_MAX;	// 最小値を探すので、最初は十分大きい値を入れておく
	OutMax = -FLT_MAX;	// 最大値を探すので、最初は十分小さい値を入れておく
	for (float V : Samples)
	{
		OutMin = FMath::Min(OutMin,V);
		OutMax = FMath::Max(OutMax,V);
	}
	// 変化がない場合でも線がつぶれないよう、最低1MB分の幅を確保
	const float Range = FMath::Max(OutMax-OutMin,1.f);
	OutMin -= Range* 0.1f;				// 下に10%の余白
	OutMax = OutMin + Range* 1.2f;		// 余白込みの上端
}

/**
 *	折れ線グラフの描画
 *		1.背景の黒い四角
 *		2.基準線を引く
 *		3.各サンプルを、隣同士でつなぐ
 */
void ADebugHUD::DrawGraph(const TArray<float>&Samples,float X,float Y,float W,float H,
                          float MinV,float MaxV,FLinearColor Color,float RefLineValue)
{
	// 1.背景
	DrawRect(FLinearColor(0.f,0.f,0.f,0.6f),X,Y,W,H);
	
	// 線を引くには、最低2点が必要。縦軸の範囲が不正な場合も描画しない。
	if (Samples.Num() < 2 || MaxV <= MinV) return;
	
	// 値->画面のY座標に変換するラムダ式
	auto ToY = [&](float V)
	{
		// (V-MinV)/(MaxV-MinV) で、MinVを0、MaxVを1とした割合(0〜1)に直す
		const float N = FMath::Clamp((V-MinV)/(MaxV-MinV),0.f,1.f);
		// 画面は下に行くほどYが大きい。Y+Hがグラフの下端なので、
		// 割合Nが大きい(値が大きい)ほど、下端から上に上がる = Yが小さくなる
		return Y + H - N * H;
	};
	
	// 基準線
	if (RefLineValue >= 0.f)
	{
		const float RefY = ToY(RefLineValue);
		DrawLine(X,RefY,X+W,RefY,FLinearColor(1.f,1.f,1.f,0.35f));
	}
	
	// 折れ線
	// (i-1)番目の点と、i番目の点を線で結ぶ。全サンプルで繰り返す。
	const int32 N = Samples.Num();
	for (int32 i=1;i<N;i++)
	{
		const float X0 = X + W * (i-1)/(N-1);
		const float X1 = X + W * i / (N-1);
		DrawLine(X0,ToY(Samples[i-1]),X1,ToY(Samples[i]),Color);
	}
}

/**
 * デバック情報の描画(毎フレーム呼ばれる)
 */
void ADebugHUD::DrawHUD()
{
	Super::DrawHUD();
	
// デバック表示が製品に残らないようにするため、Shippingではこの中身をまるごと除外する。
#if !UE_BUILD_SHIPPING
	// 非表示設定の場合、またはCanvasがない場合は何も描画しない。
	if (!bShowDebug || !Canvas)return;
	
	// GameManagerを見つけていなければ探す。
	//	HUDはGameManagerより先に生成されることがあるため、BeginPlayではなくここで探す。
	//	一度見つけたら弱参照に保持するため、以降は探し直さない。
	if (!GameManager.IsValid())
	{
		GameManager = Cast<AGameManager>(
			UGameplayStatics::GetActorOfClass(GetWorld(),AGameManager::StaticClass()));
	}
	if (!GameManager.IsValid())return;
	
	// 描画に使うデータの取得元
	UDebugStatusSystem* Stats = GameManager->GetDebugStats();
	UBulletManager* Bullets = GameManager->GetBulletManager();
	if (!Stats || !Bullets)return;
	
	// レイアウト
	const float PanelW = 360.f;	// パネルの幅
	const float X = Canvas->SizeX - PanelW -20.f;	// パネルの左端X。画面右端から、パネル幅+余白20pxだけ左
	float Y = 20.f;				// 描画位置(縦)。
	const float Line = 18.f;	// テキスト1行分の高さ
	
	// パネル全体の背景。高さ470は、3セクションが収まる大きさの固定値
	//	表示する行を増やしたら、この値も増やす
	DrawRect(FLinearColor(0.f,0.f,0.f,0.45f),X -10.f ,Y -10.f ,PanelW + 20.f,470.f);
	
	// メモリ
	DrawText(TEXT("[1]Memory(process,MB"),FLinearColor::Yellow,X,Y); Y += Line;
	
	// Now		: 現在値
	// Start	: 開始時の値
	// Delta	: 開始時からの増減(+なら増えている)
	// Peak		: 最大値
	DrawText(FString::Printf(TEXT("Now %.1f Start %.1f Delta %+.1f Peak %.1f"),
		Stats->GetCurrentMemoryMB(),Stats->GetStartMemoryMB(),
		Stats->GetCurrentMemoryMB() - Stats->GetStartMemoryMB(),Stats->GetPeakMemoryMB()),
		FLinearColor::White,X,Y); Y += Line;
	
	// Actor数とUObject数。敵を倒した後に元の数へ戻っていれば、破棄が正しくできている。
	DrawText(FString::Printf(TEXT("Actor %d UObject %d"),
		Stats->GetActorCount(),Stats->GetUObjectCount()),
		FLinearColor::White,X,Y); Y+= Line;
	
	// メモリのグラフ。縦軸の範囲は、履歴の最小/最大から自動で決める
	float MemMin,MemMax;
	GetMinMax(Stats->GetMemoryHistoryMB(), MemMin, MemMax);
	DrawGraph(Stats->GetMemoryHistoryMB(), X, Y, PanelW, 80.f, MemMin, MemMax, FLinearColor::Green);
	Y += 90.f;	// グラフの高さ80 + 余白10
	
	// 弾プール
	DrawText(TEXT("[2] Bullet Pool"),FLinearColor::Yellow,X,Y);Y += Line;
	
	// Capacity	: プールの総数
	// Active	: 使用中の弾の数
	// Free		: 未使用の弾の数
	// Peak		: 同時に使われた数の最大数
	DrawText(FString::Printf(TEXT("Capacity %d Active %d Free %d Peak %d"),
		Bullets->GetCapacity(),Bullets->GetActiveCount(),Bullets->GetFreeCount(),Bullets->GetPeakActiveCount()),
		FLinearColor::White,X,Y);Y += Line;
	const FLinearColor RejectColor = Bullets->GetFireRejectCount() > 0 ? FLinearColor::Red : FLinearColor::White;
	
	// 空きがなくて発射できなかった回数が一回でもあれば、文字を赤くして目立たせる
	DrawText(FString::Printf(TEXT("Fire %d Reject(over limit) %d Grow 0(fixed)"),
		Bullets->GetFireRequestCount(),Bullets->GetFireRejectCount()),
		RejectColor,X,Y); Y+= Line;
	
	// 使用数のグラフ。縦軸は「 0～プールの総数 」で固定
	DrawGraph(Stats->GetActiveBulletHistory(),X,Y,PanelW,50.f,
		0.f,static_cast<float>(Bullets->GetCapacity()),FLinearColor(0.3f,0.8f,1.f));
	Y += 60.f;
	
	// フレーム時間
	DrawText(TEXT("[3] Frame time(ms)"),FLinearColor::Yellow,X,Y); Y += Line;
	
	const TArray<float>& Frames = Stats -> GetFrameTimeHistoryMs();
	// 履歴の最後の要素 = 最新のフレーム時間。履歴が空なら0
	const float NowMs = Frames.Num() > 0 ? Frames.Last() : 0.f;
	
	// Now : 最新  Avg : 平均  P95/P99 : 95/99パーセンタイル  Max : 最大
	DrawText(FString::Printf(TEXT("Now %.2f Avg %.2f P95 %.2f P99 %.2f Max %.2f"),
		NowMs,Stats->GetAvgFrameMs(),Stats->GetP95FrameMs(),Stats->GetP99FrameMs(),Stats->GetMaxFrameMs()),
		FLinearColor::White,X,Y); Y+= Line;
	
	// 縦軸の上端: 最低でも33.3ms(30fps相当)は確保し、それより遅いフレームがあればMaxの1.1倍まで広げる
	const float FrameMax = FMath::Max(33.3f,Stats->GetMaxFrameMs() * 1.1f);
	
	// 16.7msの位置に白い基準線(60fpsの目安)を引く。線より上に出たフレームは60fpsを割っている
	DrawGraph(Frames,X,Y,PanelW,80.f,0.f,FrameMax,FLinearColor(1.0f,0.6f,0.2f),16.7f);
#endif
}