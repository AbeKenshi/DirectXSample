// Spacewarは新しく作成するクラス
#include "spacewar.h"
#include <iostream>

//=============================================================================
// コンストラクタ
//=============================================================================
Spacewar::Spacewar()
{
	Xvelocity = 0.0f;
	Yvelocity = 0.0f;
}

//=============================================================================
// デストラクタ
//=============================================================================
Spacewar::~Spacewar()
{
	releaseAll();           // すべてのグラフィックスアイテムについて、onLostDevie()を呼び出す
}

//=============================================================================
// ゲームを初期化
// エラー時にGameErrorをスロー
//=============================================================================
void Spacewar::initialize(HWND hwnd)
{
	Game::initialize(hwnd); // GameErrorをスロー
	
	// 星雲（nebula）のテクスチャ
	if (!nebulaTexture.initialize(graphics, NEBULA_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR,
			"Error initializing nebula texture"));
	
	// 惑星（planet）のテクスチャ
	if (!planetTexture.initialize(graphics, PLANET_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR,
			"Error initializing planet texture"));
	
	// 宇宙船テクスチャ
	if (!shipTexture.initialize(graphics, SHIP_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR,
			"Error initializing ship texture"));

	// 星雲（nebula）
	if (!nebula.initialize(graphics, 0, 0, 0, &nebulaTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing nebula"));
	
	// 惑星（planet）
	if (!planet.initialize(graphics, 0, 0, 0, &planetTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing planet"));
	// 画面の中央に惑星を配置
	planet.setX(GAME_WIDTH * 0.5f - planet.getWidth() * 0.5f);
	planet.setY(GAME_HEIGHT * 0.5f - planet.getHeight() * 0.5f);

	// 宇宙船
	if (!ship.initialize(graphics, SHIP_WIDTH, SHIP_HEIGHT, SHIP_COLS, &shipTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ship"));
	ship.setX(GAME_WIDTH / 4);	// 惑星の左上から出発
	ship.setY(GAME_HEIGHT / 4);
	ship.setFrames(SHIP_START_FRAME, SHIP_END_FRAME);	// アニメーションのフレーム
	ship.setCurrentFrame(SHIP_START_FRAME);				// 開始フレーム
	ship.setFrameDelay(SHIP_ANIMATION_DELAY);
	return;
}

//=============================================================================
// すべてのゲームアイテムを更新
//=============================================================================
void Spacewar::update()
{
	if (input->isKeyDown(SHIP_RIGHT_KEY))            // 右に移動する場合
	{
		Xvelocity += frameTime * SHIP_SPEED;
	}
	if (input->isKeyDown(SHIP_LEFT_KEY))             // 左に移動する場合
	{
		Xvelocity -= frameTime * SHIP_SPEED;
	}
	if (input->isKeyDown(SHIP_UP_KEY))               // 上に移動する場合
	{
		Yvelocity -= frameTime * SHIP_SPEED;
	}
	if (input->isKeyDown(SHIP_DOWN_KEY))             // 下に移動する場合
	{
		Yvelocity += frameTime * SHIP_SPEED;
	}
	ship.setX(ship.getX() + frameTime * Xvelocity);	// X速度を適用
	ship.setY(ship.getY() + frameTime * Yvelocity);	// Y速度を適用

	if (ship.getX() > GAME_WIDTH)               // 画面右端を超えたら
		ship.setX((float)-ship.getWidth());     // 画面左端に移動
	if (ship.getX() < -ship.getWidth())         // 画面左端を超えたら
		ship.setX((float)GAME_WIDTH);           // 画面右端に移動
	if (ship.getY() < -ship.getHeight())        // 画面上端を超えたら
		ship.setY((float)GAME_HEIGHT);          // 画面下端に移動
	if (ship.getY() > GAME_HEIGHT)              // 画面下端を超えたら
		ship.setY((float)-ship.getHeight());    // 画面上端に移動

	ship.update(frameTime);
}

//=============================================================================
// 人工知能
//=============================================================================
void Spacewar::ai()
{}

//=============================================================================
// 衝突を処理
//=============================================================================
void Spacewar::collisions()
{}

//=============================================================================
// ゲームアイテムをレンダー
//=============================================================================
void Spacewar::render()
{
	graphics->spriteBegin();	// スプライトの描画を開始

	nebula.draw();				// オリオン星雲をシーンに追加
	planet.draw();				// 惑星をシーンに追加
	ship.draw();

	graphics->spriteEnd();		// スプライトの描画を終了
}

//=============================================================================
// グラフィックスデバイスが消失した場合
// グラフィックスデバイスをリセット可能にするため、
// 予約されていたビデオメモリをすべて解放
//=============================================================================
void Spacewar::releaseAll()
{
	shipTexture.onLostDevice();
	planetTexture.onLostDevice();
	nebulaTexture.onLostDevice();
	Game::releaseAll();
	return;
}

//=============================================================================
// グラフィックスデバイスがリセットされた場合
// すべてのサーフェイスを再作成
//=============================================================================
void Spacewar::resetAll()
{
	nebulaTexture.onResetDevice();
	planetTexture.onResetDevice();
	shipTexture.onResetDevice();
	Game::resetAll();
	return;
}
