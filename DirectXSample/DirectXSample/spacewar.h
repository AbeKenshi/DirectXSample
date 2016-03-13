#ifndef _SPACEWAR_H             // このファイルが複数の箇所でインクルードされる場合に、
#define _SPACEWAR_H             // 多重に定義されることを防ぎます。
#define WIN32_LEAN_AND_MEAN

#include "game.h"

//=============================================================================
// Spacewarは新しく作成するクラス。Gameクラスを継承する
//=============================================================================
class Spacewar : public Game
{
private:
	// 変数

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
