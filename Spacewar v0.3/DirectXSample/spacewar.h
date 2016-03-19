#ifndef _SPACEWAR_H             // このファイルが複数の箇所でインクルードされる場合に、
#define _SPACEWAR_H             // 多重に定義されることを防ぎます。
#define WIN32_LEAN_AND_MEAN

#include "game.h"
#include "textureManager.h"
#include "image.h"
#include "planet.h"
#include "ship.h"

//=============================================================================
// このクラスはゲームエンジンのコア
//=============================================================================
class Spacewar : public Game
{
private:
	// ゲームアイテム
	TextureManager nebulaTexture;	// 星雲（nebula）のテクスチャ
	TextureManager gameTextures;	// ゲームに使用するテクスチャ群
	Ship ship1, ship2;				// 宇宙船
	Planet planet;					// 惑星（planet）
	Image nebula;					// 星雲（nebula）の画像

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
	void releaseAll();
	void resetAll();
};

#endif
