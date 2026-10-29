#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "DebugHUD.generated.h"

class AGameManager;

/**
 *	デバック情報を画面に描画するHUDクラス
 *		(計測はDebugStatusSystemが行う)
 */
UCLASS()
class PROTECT_API ADebugHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	// 描画処理
	virtual void DrawHUD() override;
	// デバックウィンドウを表示するか否かを切り替える
	void ToggleDebugWindow() {bShowDebug = !bShowDebug;}
	
private:
	/**
	 *	折れ線グラフの描画を行う
	 *		@param Samples		描画する値の履歴(古い順)
	 *		@param X,Y			グラフの左上と画面座標(ピクセル)
	 *		@param W,H			グラフの幅と高さ(ピクセル)
	 *		@param MinV,MaxV	縦軸の範囲。Minが下端、Maxが上端
	 *		@param Color		線の色
	 *		@param RefLineValue	基準線を引く値(省略時の-1で引かない)
	 */
	void DrawGraph(const TArray<float>&Samples,float X,float Y,float W,float H,
	               float MinV,float MaxV,FLinearColor Color,float RefLineValue = -1.f);

	/**
	 *	履歴から、グラフの縦軸の範囲(Min,Max)を求める
	 *		staticは「このクラスのメンバ変数を使わない関数」
	 */
	static void GetMinMax(const TArray<float>& Samples,float& OutMin,float& OutMax);
	
	// デバック表示のON/OFF
	bool bShowDebug = true;
	
	// GameManagerへの弱参照。HUDの方が先に作られることがあるため、Draw()内で探して保持する
	TWeakObjectPtr<AGameManager> GameManager;
};