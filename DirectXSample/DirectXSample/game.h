#ifndef _GAME_H	// このファイルが複数の箇所でインクルードされる場合に、
#define _GAME_H // 多重に定義されることを防ぎます。
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <mmsystem.h>
#include "graphics.h"
#include "input.h"
#include "constants.h"
#include "gameError.h"
#include "audio.h"
#include "textDX.h"
#include "console.h"

namespace gameNS
{
	const char FONT[] = "Courier New";  // フォント
	const int POINT_SIZE = 14;          // ポイントサイズ
	const COLOR_ARGB FONT_COLOR = SETCOLOR_ARGB(255, 255, 255, 255);    // 白
}

class Game
{
protected:
	// 共通のゲームプロパティ
	Graphics *graphics;			// Graphicsへのポインタ
	Input *input;				// Inputへのポインタ
	Audio *audio;				// Audioへのポインタ
	Console *console;			// Consoleへのポインタ
	HWND hwnd;					// ウィンドウハンドル
	HRESULT hr;					// 標準の戻り型
	LARGE_INTEGER timeStart;	// パフォーマンスカウンターの開始値
	LARGE_INTEGER timeEnd;		// パフォーマンスカウンターの終了値
	LARGE_INTEGER timerFreq;	// パフォーマンスカウンターの周波数
	float frameTime;			// 最後のフレームに要した時間
	float fps;					// フレームレート（1秒あたりのフレーム数)
	TextDX dxFont;				// フレームレート用のDirectXフォント
	bool fpsOn;					// trueの場合フレームレートを表示
	DWORD sleepTime;			// フレーム間でスリープする時間（ミリ秒）
	bool paused;				// ゲームが一時停止されている場合、true
	bool initialized;
	std::string  command;       // コンソールからのコマンド
public:
	// コンストラクタ
	Game();
	// デストラクタ
	virtual ~Game();

	// メンバー関数

	// Windowsメッセージハンドラ
	LRESULT messageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	
	// ゲームを初期化
	// 実行前：hwndはウィンドウへのハンドル
	virtual void initialize(HWND hwnd);
	
	// WinMain内のメインのメッセージループでrunを繰り返し呼び出す
	virtual void run(HWND);
	
	// グラフィックスデバイスが消失したときに呼び出す
	// グラフィックスデバイスをリセット可能にするため、
	// 予約されていたビデオメモリをすべて解放
	virtual void releaseAll();
	
	// すべてのサーフェイスを再作成し、すべてのエンティティをリセット
	virtual void resetAll();
	
	// 予約されていたメモリをすべて削除
	virtual void deleteAll();

	// コンソールコマンドを処理
	virtual void consoleCommand();
	
	// ゲームアイテムをレンダー
	virtual void renderGame();
	
	// 消失したグラフィックスデバイスを処理
	virtual void handleLostGraphicsDevice();

	// Set display mode (fullscreen, window or toggle)
	void setDisplayMode(graphicsNS::DISPLAY_MODE mode = graphicsNS::TOGGLE);
	
	// Graphicsへのポインタを戻す
	Graphics* getGraphics() { return graphics; }
	
	// Inputへのポインタを戻す
	Input* getInput() { return input; }

	// ゲームを終了
	void exitGame() { PostMessage(hwnd, WM_DESTROY, 0, 0); }

	// Audioへのポインタを戻す
	Audio* getAudio() { return audio; }
	
	// 純粋仮想関数の宣言
	// これらの関数は、Gameを継承するクラス内で記述する必要があります。
	// ゲームアイテムを更新
	virtual void update() = 0;
	
	// AI計算を実行
	virtual void ai() = 0;
	
	// 衝突をチェック
	virtual void collisions() = 0;
	
	// グラフィックスをレンダー
	// graphics->spriteBegin();を呼び出す
	// スプライトを描画
	// graphics->spriteEnd();を呼び出す
	// スプライト以外を描画
	virtual void render() = 0;
};

#endif
