// Spacewarは新しく作成するクラス
#include "spacewar.h"
#include <iostream>

//=============================================================================
// コンストラクタ
//=============================================================================
Spacewar::Spacewar()
{
	menuOn = true;
	countDownOn = false;
	roundOver = false;
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
	Game::initialize(hwnd); // throws GameError

	// initialize DirectX fonts
	fontBig.initialize(graphics, spacewarNS::FONT_BIG_SIZE, false, false, spacewarNS::FONT);
	fontBig.setFontColor(spacewarNS::FONT_COLOR);

	// menu texture
	if (!menuTexture.initialize(graphics, MENU_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu texture"));

	// 星雲（space）のテクスチャ
	if (!spaceTexture.initialize(graphics, SPACE_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR,
			"Error initializing space texture"));

	// game textures
	if (!textures1.initialize(graphics, TEXTURES1_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing game textures"));


	// nebula texture
	if (!nebulaTexture.initialize(graphics, NEBULA_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing nebula texture"));

	// main game textures
	if (!gameTextures.initialize(graphics, TEXTURES_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing game textures"));

	// 星空の画像
	if (!space.initialize(graphics, 0, 0, 0, &spaceTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR,
			"Error initializing space"));
	space.setScale((float)SPACE_SCALE);

	// moon images
	for (int i = 0; i<4; i++)
	{
		if (!moons[i].initialize(graphics, MOON_SIZE, MOON_SIZE, 4, &textures1))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing moons"));
		moons[i].setFrames(MOON1_FRAME + i, MOON1_FRAME + i);
		moons[i].setCurrentFrame(MOON1_FRAME + i);
		// Start moons in center
		moons[i].setX(GAME_WIDTH / 2 - MOON_SIZE / 2);
		moons[i].setY(GAME_HEIGHT / 2 - MOON_SIZE / 2);
	}

	// nebula image
	if (!nebula.initialize(graphics, 0, 0, 0, &nebulaTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing nebula"));

	// planet
	if (!planet.initialize(this, planetNS::WIDTH, planetNS::HEIGHT, 2, &gameTextures))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing planet"));

	// ship
	if (!ship1.initialize(this, shipNS::WIDTH, shipNS::HEIGHT, shipNS::TEXTURE_COLS, &gameTextures))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ship1"));
	ship1.setFrames(shipNS::SHIP1_START_FRAME, shipNS::SHIP1_END_FRAME);
	ship1.setCurrentFrame(shipNS::SHIP1_START_FRAME);
	ship1.setX(GAME_WIDTH / 4);
	ship1.setY(GAME_HEIGHT / 4);
	ship1.setVelocity(VECTOR2(shipNS::SPEED, -shipNS::SPEED)); // VECTOR2(X, Y)
															   // ship2
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
	// エンティティを更新
	ship1.update(frameTime);
	ship2.update(frameTime);
	// 惑星をX方向に動かす
	planet.setX(planet.getX() - frameTime * ship1.getVelocity().x);
	// 惑星をY方向に動かす
	planet.setY(planet.getY() - frameTime * ship1.getVelocity().y);
	planet.update(frameTime);
	for (int i = 0; i < 4; ++i)	// 衛星を動かす
	{
		// スクロールの速さを、衛星ごとに20%遅くする
		moons[i].setX(moons[i].getX() -
			frameTime * ship1.getVelocity().x * 0.2f * (4 - i));
		moons[i].setY(moons[i].getY() -
			frameTime * ship1.getVelocity().y * 0.2f * (4 - i));
	}
	// 星空を宇宙船と反対のX方向に移動
	space.setX(space.getX() - frameTime * ship1.getVelocity().x);
	// 星空を宇宙船と反対のY方向に移動
	space.setY(space.getY() - frameTime * ship1.getVelocity().y);

	// 星空画像を端で折り返す
	// 星空の左端 > 画面の左端の場合
	if (space.getX() > 0)
		// 星空画像をSPACE_WIDTH分だけ左に動かす
		space.setX(space.getX() - SPACE_WIDTH);
	// 星空画像が画面から外れて左にある場合
	if (space.getX() < -SPACE_WIDTH)
		// 星空画像をSPACE_WIDTH分だけ右に動かす
		space.setX(space.getX() + SPACE_WIDTH);
	// 星空の上端 > 画面の上端の場合
	if (space.getY() > 0)
		// 星空画像をSPACE_HEIGHT分だけ上に動かす
		space.setY(space.getY() - SPACE_HEIGHT);
	// 星空画像が画面から外れて上にある場合
	if (space.getY() < -SPACE_HEIGHT)
		space.setY(space.getY() + SPACE_HEIGHT);
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
	// 宇宙船と惑星の衝突の場合
	if (ship1.collidesWith(planet, collisionVector))
	{
		// 惑星から跳ね返る
		ship1.bounce(collisionVector, planet);
		ship1.damage(PLANET);
	}
	if (ship2.collidesWith(planet, collisionVector))
	{
		// 惑星から跳ね返る
		ship2.bounce(collisionVector, planet);
		ship2.damage(PLANET);
	}
	// 宇宙船同士の衝突の場合
	if (ship1.collidesWith(ship2, collisionVector))
	{
		// 宇宙船から跳ね返る
		ship1.bounce(collisionVector, ship2);
		ship1.damage(SHIP);
		// ship2のcollisionVectorの方向を変更する
		ship2.bounce(collisionVector*-1, ship1);
		ship2.damage(SHIP);
	}
}

//=============================================================================
// ゲームアイテムをレンダー
//=============================================================================
void Spacewar::render()
{
	float x = space.getX();
	float y = space.getY();
	graphics->spriteBegin();	// スプライトの描画を開始
	// 星空画像を端で折り返す
	space.draw();				// 現在の位置で描画
	// 星空画像の右端が見える場合
	if (space.getX() < -SPACE_WIDTH + (int)GAME_WIDTH)
	{
		space.setX(space.getX() + SPACE_WIDTH);		// 左端に戻る
		space.draw();								// 再度、描画する
	}
	// 星空画像の下端が見える場合
	if (space.getY() < -SPACE_HEIGHT + (int)GAME_HEIGHT)
	{
		space.setY(space.getY() + SPACE_HEIGHT);	// 上端に戻る
		space.draw();								// 再度、描画する
		space.setX(x);								// X位置を復元
		// 星空画像の右端が見える場合
		// 左端に戻る
		if (space.getX() < -SPACE_WIDTH + (int)GAME_WIDTH)
			space.draw();							// 再度、描画する
	}
	space.setY(y);									// Y位置を復元
	//nebula.draw();				// オリオン星雲をシーンに追加
	for (int i = 3; i >= 0; --i)
	{
		moons[i].draw();			// 衛星の描画
	}
	planet.draw();				// 惑星をシーンに追加
	
	// 宇宙船を描画
	ship1.draw();
	ship2.draw();

	if (menuOn)
		menu.draw();
	if (countDownOn)
	{
		_snprintf_s(buffer, spacewarNS::BUF_SIZE, "%d",
			(int)(ceil(countDownTimer)));
		fontBig.print(buffer, spacewarNS::COUNT_DOWN_X,
			spacewarNS::COUNT_DOWN_Y);
	}
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
