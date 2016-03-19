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
// �F���D��������
// ���s��F���������ꍇ��true�A���s�����ꍇ��false��߂�
//=============================================================================
bool Ship::initialize(Game *gamePtr, int width, int height, int ncols,
	TextureManager *textureM)
{
	shield.initialize(gamePtr->getGraphics(), width, height, ncols, textureM);
	shield.setFrames(shipNS::SHIELD_START_FRAME, shipNS::SHIELD_END_FRAME);
	shield.setCurrentFrame(shipNS::SHIELD_START_FRAME);
	shield.setFrameDelay(shipNS::SHIELD_ANIMATION_DELAY);
	shield.setLoop(false);                  // �A�j���[�V���������[�v���Ȃ�
	return(Entity::initialize(gamePtr, width, height, ncols, textureM));
}

//=============================================================================
// �V�[���h�̕`��
//=============================================================================
void Ship::draw()
{
	Image::draw();              // �F���D��`��
	if (shieldOn)
		// colorFilter 50%�A���t�@���g���ăV�[���h��`��
		shield.draw(spriteData, graphicsNS::ALPHA50 & colorFilter);
}

//=============================================================================
// update
// �ʏ�A�t���[�����Ƃ�1��Ăяo��
// frameTime�́A�ړ��ƃA�j���[�V�����̑����𐧌䂷�邽�߂Ɏg�p
//=============================================================================
void Ship::update(float frameTime)
{
	Entity::update(frameTime);
	// �F���D����]������
	spriteData.angle += frameTime * shipNS::ROTATION_RATE;


	spriteData.x += frameTime * velocity.x;     // �F���D��X�����ɓ�����
	spriteData.y += frameTime * velocity.y;     // �F���D��Y�����ɓ�����

												// �ǂł̒��˕Ԃ�
												// ��ʉE�[�ɏՓ˂����ꍇ
	if (spriteData.x > GAME_WIDTH - shipNS::WIDTH*getScale())
	{
		// ��ʉE�[�ɔz�u
		spriteData.x = GAME_WIDTH - shipNS::WIDTH*getScale();
		velocity.x = -velocity.x;               // X�����𔽓]
		audio->playCue(BEEP1);					// �T�E���h���Đ�
	}
	else if (spriteData.x < 0)                  // ��ʍ��[�ɏՓ˂����ꍇ
	{
		spriteData.x = 0;                       // ��ʍ��[�ɔz�u
		velocity.x = -velocity.x;               // X�����𔽓]
		audio->playCue(BEEP2);					// �T�E���h���Đ�
	}
	// ��ʉ��[�ɏՓ˂����ꍇ
	if (spriteData.y > GAME_HEIGHT - shipNS::HEIGHT*getScale())
	{
		// ��ʉ��[�ɔz�u
		spriteData.y = GAME_HEIGHT - shipNS::HEIGHT*getScale();
		velocity.y = -velocity.y;               // Y�����𔽓]
		audio->playCue(BEEP3);					// �T�E���h���Đ�
	}
	else if (spriteData.y < 0)                  // ��ʏ�[�ɏՓ˂����ꍇ
	{
		spriteData.y = 0;                       // ��ʏ�[�ɔz�u
		velocity.y = -velocity.y;               // Y�����𔽓]
		audio->playCue(BEEP4);					// �T�E���h���Đ�
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