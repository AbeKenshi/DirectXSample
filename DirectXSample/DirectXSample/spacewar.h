#ifndef _SPACEWAR_H             // このファイルが複数の箇所でインクルードされる場合に、
#define _SPACEWAR_H             // 多重に定義されることを防ぎます。
#define WIN32_LEAN_AND_MEAN

#include "game.h"
#include "textureManager.h"
#include "image.h"
#include "planet.h"
#include "ship.h"

namespace spacewarNS
{
	const char FONT[] = "Arial Bold";  // font
	const int FONT_BIG_SIZE = 256;     // font height
	const COLOR_ARGB FONT_COLOR = graphicsNS::YELLOW;
	const COLOR_ARGB SHIP1_COLOR = graphicsNS::BLUE;
	const int COUNT_DOWN_X = GAME_WIDTH / 2 - FONT_BIG_SIZE / 4;
	const int COUNT_DOWN_Y = GAME_HEIGHT / 2 - FONT_BIG_SIZE / 2;
	const int COUNT_DOWN = 2;               // count down
	const int BUF_SIZE = 20;
	const int ROUND_TIME = 5;               // time until new round starts
}

//=============================================================================
// このクラスはゲームエンジンのコア
//=============================================================================
class Spacewar : public Game
{
private:
	// ゲームアイテム
	TextureManager nebulaTexture;	// 星雲（nebula）のテクスチャ
	TextureManager gameTextures;	// ゲームのテクスチャ
	TextureManager menuTexture, spaceTexture, textures1;	// テクスチャ
	Ship ship1, ship2;				// 宇宙船
	Planet planet;					// 惑星
	Image nebula;					// 星雲（nebula）の画像
	Image space;					// 背景画像
	Image moons[4];					// 月
	Image menu;						// メニュー
	bool    menuOn;
	bool    countDownOn;        // true when count down is displayed
	float   countDownTimer;
	TextDX  fontBig;            // DirectX font for game banners
	char buffer[spacewarNS::BUF_SIZE];
	bool    roundOver;          // true when round is over
	float   roundTimer;         // time until new round starts


public:
	// コンストラクタ
	Spacewar();

	// デストラクタ
	virtual ~Spacewar();

	// ゲームを初期化
	void initialize(HWND hwnd);
	void update();      // Gameからの純粋仮想関数をオーバーライドする必要がある
	void ai();          // "
	void collisions();  // "
	void render();      // "
	void roundStart();  // start a new round of play
	void releaseAll();
	void resetAll();
};

#endif
