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
	oldX = shipNS::X;
	oldY = shipNS::Y;
	oldAngle = 0.0f;
	rotation = 0.0f;
	velocity.x = 0;
	velocity.y = 0;
	frameDelay = shipNS::SHIP_ANIMATION_DELAY;
	startFrame = shipNS::SHIP1_START_FRAME;     // first frame of ship animation
	endFrame = shipNS::SHIP1_END_FRAME;     // last frame of ship animation
	currentFrame = startFrame;
	radius = shipNS::WIDTH / 2.0;
	collisionType = entityNS::CIRCLE;
	direction = shipNS::NONE;                   // direction of rotation thruster
	engineOn = false;
	shieldOn = false;
	explosionOn = false;
	mass = shipNS::MASS;
}

//=============================================================================
// 宇宙船を初期化
// 実行後：成功した場合はtrue、失敗した場合はfalseを戻す
//=============================================================================
bool Ship::initialize(Game *gamePtr, int width, int height, int ncols,
	TextureManager *textureM)
{
	engine.initialize(gamePtr->getGraphics(), width, height, ncols, textureM);
	engine.setFrames(shipNS::ENGINE_START_FRAME, shipNS::ENGINE_END_FRAME);
	engine.setCurrentFrame(shipNS::ENGINE_START_FRAME);
	engine.setFrameDelay(shipNS::ENGINE_ANIMATION_DELAY);
	shield.initialize(gamePtr->getGraphics(), width, height, ncols, textureM);
	shield.setFrames(shipNS::SHIELD_START_FRAME, shipNS::SHIELD_END_FRAME);
	shield.setCurrentFrame(shipNS::SHIELD_START_FRAME);
	shield.setFrameDelay(shipNS::SHIELD_ANIMATION_DELAY);
	shield.setLoop(false);                  // do not loop animation
	explosion.initialize(gamePtr->getGraphics(), width, height, ncols, textureM);
	explosion.setFrames(shipNS::EXPLOSION_START_FRAME, shipNS::EXPLOSION_END_FRAME);
	explosion.setCurrentFrame(shipNS::EXPLOSION_START_FRAME);
	explosion.setFrameDelay(shipNS::EXPLOSION_ANIMATION_DELAY);
	explosion.setLoop(false);               // do not loop animation
	return(Entity::initialize(gamePtr, width, height, ncols, textureM));
}

//=============================================================================
// シールドの描画
//=============================================================================
void Ship::draw()
{
	if (explosionOn)
		explosion.draw(spriteData); // draw explosion using current spriteData
	else
	{
		Image::draw();              // draw ship
		if (engineOn)
			engine.draw(spriteData);     // draw engine rocket
		if (shieldOn)
			// draw shield using colorFilter 25% alpha
			shield.draw(spriteData, graphicsNS::ALPHA50 & colorFilter);
	}
}

//=============================================================================
// update
// 通常、フレームごとに1回呼び出す
// frameTimeは、移動とアニメーションの速さを制御するために使用
//=============================================================================
void Ship::update(float frameTime)
{
	if (explosionOn)
	{
		explosion.update(frameTime);
		if (explosion.getAnimationComplete())    // if explosion animation complete
		{
			explosionOn = false;                // turn off explosion
			visible = false;
			explosion.setAnimationComplete(false);
			explosion.setCurrentFrame(shipNS::EXPLOSION_START_FRAME);
		}
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

	if (engineOn)
	{
		velocity.x += (float)cos(spriteData.angle) * shipNS::SPEED * frameTime;
		velocity.y += (float)sin(spriteData.angle) * shipNS::SPEED * frameTime;
		//engine.update(frameTime);
	}


	Entity::update(frameTime);
	oldX = spriteData.x;                        // save current position
	oldY = spriteData.y;
	oldAngle = spriteData.angle;

	switch (direction)                          // rotate ship
	{
	case shipNS::LEFT:
		rotation -= frameTime * shipNS::ROTATION_RATE;  // rotate left
		break;
	case shipNS::RIGHT:
		rotation += frameTime * shipNS::ROTATION_RATE;  // rotate right
		break;
	}
	// 宇宙船を回転させる
	spriteData.angle += frameTime * rotation;
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
}

//=============================================================================
// damage
//=============================================================================
void Ship::damage(WEAPON weapon)
{
	if (shieldOn)
		return;

	switch (weapon)
	{
	case TORPEDO:
//		audio->playCue(TORPEDO_HIT);
		health -= shipNS::TORPEDO_DAMAGE;
		break;
	case SHIP:
//		audio->playCue(COLLIDE);    // play sound
		health -= shipNS::SHIP_DAMAGE;
		break;
	case PLANET:
		health = 0;
		break;
	}
	if (health <= 0)
		explode();
	else
		shieldOn = true;
}

//=============================================================================
// explode
//=============================================================================
void Ship::explode()
{
//	audio->playCue(EXPLODE);
	active = false;
	health = 0;
	explosionOn = true;
	engineOn = false;
	shieldOn = false;
	velocity.x = 0.0f;
	velocity.y = 0.0f;
}

//=============================================================================
// repair
//=============================================================================
void Ship::repair()
{
	active = true;
	health = FULL_HEALTH;
	explosionOn = false;
	engineOn = false;
	shieldOn = false;
	rotation = 0.0f;
	direction = shipNS::NONE;           // direction of rotation thruster
	visible = true;
}