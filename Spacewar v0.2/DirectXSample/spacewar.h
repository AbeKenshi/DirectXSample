#ifndef _SPACEWAR_H             // このファイルが複数の箇所でインクルードされる場合に、
#define _SPACEWAR_H             // 多重に定義されることを防ぎます。
#define WIN32_LEAN_AND_MEAN

#include "game.h"
#include "textureManager.h"
#include "image.h"

//=============================================================================
// このクラスはゲームエンジンのコア
//=============================================================================
class Spacewar : public Game
{
private:
	// ゲームアイテム
	TextureManager nebulaTexture;	// 星雲（nebula）のテクスチャ
	TextureManager planetTexture;	// 惑星（planet）のテクスチャ
	TextureManager shipTexture;		// 宇宙船のテクスチャ
	Image planet;					// 惑星（planet）の画像
	Image nebula;					// 星雲（nebula）の画像
	Image ship;						// 宇宙船の画像
	float Xvelocity;				// 宇宙船のX方向へのスピード
	float Yvelocity;				// 宇宙船のY方向へのスピード

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
