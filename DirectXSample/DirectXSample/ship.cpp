#include "ship.h"

//=============================================================================
// default constructor
//=============================================================================
Ship::Ship() : Entity()
{
	spriteData.width = shipNS::WIDTH;           // size of Ship1
	spriteData.height = shipNS::HEIGHT;
	spriteData.x = shipNS::X;                   // location on screen
	spriteData.y = shipNS::Y;
	spriteData.rect.bottom = shipNS::HEIGHT;    // rectangle to select parts of an image
	spriteData.rect.right = shipNS::WIDTH;
	velocity.x = 0;                             // velocity X
	velocity.y = 0;                             // velocity Y
	frameDelay = shipNS::SHIP_ANIMATION_DELAY;
	startFrame = shipNS::SHIP1_START_FRAME;     // first frame of ship animation
	endFrame = shipNS::SHIP1_END_FRAME;     // last frame of ship animation
	currentFrame = startFrame;
	radius = shipNS::WIDTH / 2.0;
	shieldOn = false;
	mass = shipNS::MASS;
	collisionType = entityNS::CIRCLE;
}

//=============================================================================
// 宇宙船を初期化
// 実行後：成功した場合はtrue、失敗した場合はfalseを戻す
//=============================================================================
bool Ship::initialize(Game *gamePtr, int width, int height, int ncols,
	TextureManager *textureM)
{
	shield.initialize(gamePtr->getGraphics(), width, height, ncols, textureM);
	shield.setFrames(shipNS::SHIELD_START_FRAME, shipNS::SHIELD_END_FRAME);
	shield.setCurrentFrame(shipNS::SHIELD_START_FRAME);
	shield.setFrameDelay(shipNS::SHIELD_ANIMATION_DELAY);
	shield.setLoop(false);                  // アニメーションをループしない
	return(Entity::initialize(gamePtr, width, height, ncols, textureM));
}

//=============================================================================
// シールドの描画
//=============================================================================
void Ship::draw()
{
	Image::draw();              // 宇宙船を描画
	if (shieldOn)
		// colorFilter 50%アルファを使ってシールドを描画
		shield.draw(spriteData, graphicsNS::ALPHA50 & colorFilter);
}

//=============================================================================
// update
// 通常、フレームごとに1回呼び出す
// frameTimeは、移動とアニメーションの速さを制御するために使用
//=============================================================================
void Ship::update(float frameTime)
{
	Entity::update(frameTime);
	// 宇宙船を回転させる
	spriteData.angle += frameTime * shipNS::ROTATION_RATE;


	spriteData.x += frameTime * velocity.x;     // 宇宙船をX方向に動かす
	spriteData.y += frameTime * velocity.y;     // 宇宙船をY方向に動かす

												// 壁での跳ね返り
												// 画面右端に衝突した場合
	if (spriteData.x > GAME_WIDTH - shipNS::WIDTH*getScale())
	{
		// 画面右端に配置
		spriteData.x = GAME_WIDTH - shipNS::WIDTH*getScale();
		velocity.x = -velocity.x;               // X方向を反転
		audio->playCue(BEEP1);					// サウンドを再生
	}
	else if (spriteData.x < 0)                  // 画面左端に衝突した場合
	{
		spriteData.x = 0;                       // 画面左端に配置
		velocity.x = -velocity.x;               // X方向を反転
		audio->playCue(BEEP2);					// サウンドを再生
	}
	// 画面下端に衝突した場合
	if (spriteData.y > GAME_HEIGHT - shipNS::HEIGHT*getScale())
	{
		// 画面下端に配置
		spriteData.y = GAME_HEIGHT - shipNS::HEIGHT*getScale();
		velocity.y = -velocity.y;               // Y方向を反転
		audio->playCue(BEEP3);					// サウンドを再生
	}
	else if (spriteData.y < 0)                  // 画面上端に衝突した場合
	{
		spriteData.y = 0;                       // 画面上端に配置
		velocity.y = -velocity.y;               // Y方向を反転
		audio->playCue(BEEP4);					// サウンドを再生
	}
	if (shieldOn)
	{
		shield.update(frameTime);
		if (shield.getAnimationComplete())
		{
			shieldOn = false;
			shield.setAnimationComplete(false);
		}
	}
}

//=============================================================================
// damage
//=============================================================================
void Ship::damage(WEAPON weapon)
{
	shieldOn = true;
}