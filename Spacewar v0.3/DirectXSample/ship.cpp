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

	if (spriteData.x > GAME_WIDTH)               // 画面右端を超えたら
		spriteData.x = -shipNS::WIDTH;		     // 画面左端に移動
	if (spriteData.x < -shipNS::WIDTH)			 // 画面左端を超えたら
		spriteData.x = GAME_WIDTH;           // 画面右端に移動
	if (spriteData.y < -shipNS::HEIGHT)        // 画面上端を超えたら
		spriteData.y = GAME_HEIGHT;          // 画面下端に移動
	if (spriteData.y > GAME_HEIGHT)              // 画面下端を超えたら
		spriteData.y = -shipNS::HEIGHT;    // 画面上端に移動


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