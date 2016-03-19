// Spacewarは新しく作成するクラス
#include "spacewar.h"
#include <iostream>

//=============================================================================
// コンストラクタ
//=============================================================================
Spacewar::Spacewar()
{
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
	
	// ゲームに使用するテクスチャ群
	if (!gameTextures.initialize(graphics, TEXTURES_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR,
			"Error initializing planet texture"));

	// 星雲（nebula）
	if (!nebula.initialize(graphics, 0, 0, 0, &nebulaTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing nebula"));
	
	// 惑星（planet）
	if (!planet.initialize(this, planetNS::WIDTH, planetNS::HEIGHT, 2, &gameTextures))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing planet"));
	// 画面の中央に惑星を配置
	planet.setX(GAME_WIDTH * 0.5f - planet.getWidth() * 0.5f);
	planet.setY(GAME_HEIGHT * 0.5f - planet.getHeight() * 0.5f);
	
	// 宇宙船1
	if (!ship1.initialize(this, shipNS::WIDTH, shipNS::HEIGHT, shipNS::TEXTURE_COLS, &gameTextures))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ship1"));
	ship1.setFrames(shipNS::SHIP1_START_FRAME, shipNS::SHIP1_END_FRAME);
	ship1.setCurrentFrame(shipNS::SHIP1_START_FRAME);
	//ship1.setX(GAME_WIDTH / 4);
	//ship1.setY(GAME_HEIGHT / 4);
	//ship1.setVelocity(VECTOR2(shipNS::SPEED, -shipNS::SPEED)); // VECTOR2(X, Y)
	ship1.setX(GAME_WIDTH / 4 - shipNS::WIDTH);
	ship1.setY(GAME_HEIGHT / 2 - shipNS::HEIGHT);
	ship1.setVelocity(VECTOR2(0, -shipNS::SPEED));
	// 宇宙船2
	if (!ship2.initialize(this, shipNS::WIDTH, shipNS::HEIGHT, shipNS::TEXTURE_COLS, &gameTextures))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ship2"));
	ship2.setFrames(shipNS::SHIP2_START_FRAME, shipNS::SHIP2_END_FRAME);
	ship2.setCurrentFrame(shipNS::SHIP2_START_FRAME);
	ship2.setX(GAME_WIDTH - GAME_WIDTH / 4);
	ship2.setY(GAME_HEIGHT / 4);
	ship2.setVelocity(VECTOR2(-shipNS::SPEED, -shipNS::SPEED)); // VECTOR2(X, Y)

	return;
}

//=============================================================================
// すべてのゲームアイテムを更新
//=============================================================================
void Spacewar::update()
{
	if (ship1.getActive())
	{
		VECTOR2 vector = ship1.getVelocity();
		if (input->isKeyDown(SHIP1_RIGHT_KEY))            // 右に移動する場合
		{
			vector.x += frameTime * shipNS::SPEED;
		}
		if (input->isKeyDown(SHIP1_LEFT_KEY))             // 左に移動する場合
		{
			vector.x -= frameTime * shipNS::SPEED;
		}
		if (input->isKeyDown(SHIP1_UP_KEY))               // 上に移動する場合
		{
			vector.y -= frameTime * shipNS::SPEED;
		}
		if (input->isKeyDown(SHIP1_DOWN_KEY))             // 下に移動する場合
		{
			vector.y += frameTime * shipNS::SPEED;
		}
		ship1.setVelocity(vector);
	}
	if (ship2.getActive())
	{
		VECTOR2 vector = ship2.getVelocity();
		if (input->isKeyDown(SHIP2_RIGHT_KEY))            // 右に移動する場合
		{
			vector.x += frameTime * shipNS::SPEED;
		}
		if (input->isKeyDown(SHIP2_LEFT_KEY))             // 左に移動する場合
		{
			vector.x -= frameTime * shipNS::SPEED;
		}
		if (input->isKeyDown(SHIP2_UP_KEY))               // 上に移動する場合
		{
			vector.y -= frameTime * shipNS::SPEED;
		}
		if (input->isKeyDown(SHIP2_DOWN_KEY))             // 下に移動する場合
		{
			vector.y += frameTime * shipNS::SPEED;
		}
		ship2.setVelocity(vector);
	}
	ship1.gravityForce(&planet, frameTime);
	ship2.gravityForce(&planet, frameTime);

	planet.update(frameTime);
	ship1.update(frameTime);
	ship2.update(frameTime);
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
{
	VECTOR2 collisionVector;
	// if collision between ship and planet
	if (ship1.collidesWith(planet, collisionVector))
	{
		// bounce off planet
		ship1.bounce(collisionVector, planet);
		ship1.damage(PLANET);
	}
	if (ship2.collidesWith(planet, collisionVector))
	{
		// bounce off planet
		ship2.bounce(collisionVector, planet);
		ship2.damage(PLANET);
	}
	// if collision between ships
	if (ship1.collidesWith(ship2, collisionVector))
	{
		// bounce off ship
		ship1.bounce(collisionVector, ship2);
		ship1.damage(SHIP);
		// change the direction of the collisionVector for ship2
		ship2.bounce(collisionVector*-1, ship1);
		ship2.damage(SHIP);
	}
}

//=============================================================================
// ゲームアイテムをレンダー
//=============================================================================
void Spacewar::render()
{
	graphics->spriteBegin();	// スプライトの描画を開始

	nebula.draw();				// オリオン星雲をシーンに追加
	planet.draw();				// 惑星をシーンに追加
	ship1.draw();
	ship2.draw();

	graphics->spriteEnd();		// スプライトの描画を終了
}

//=============================================================================
// グラフィックスデバイスが消失した場合
// グラフィックスデバイスをリセット可能にするため、
// 予約されていたビデオメモリをすべて解放
//=============================================================================
void Spacewar::releaseAll()
{
	nebulaTexture.onLostDevice();
	gameTextures.onLostDevice();
	Game::releaseAll();
	return;
}

//=============================================================================
// グラフィックスデバイスがリセットされた場合
// すべてのサーフェイスを再作成
//=============================================================================
void Spacewar::resetAll()
{
	gameTextures.onResetDevice();
	nebulaTexture.onResetDevice();
	Game::resetAll();
	return;
}
