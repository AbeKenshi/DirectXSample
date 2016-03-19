#include "ship.h"

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
Ship::Ship() : Entity()
{
	spriteData.width = shipNS::WIDTH;           // �F���D1�̃T�C�Y
	spriteData.height = shipNS::HEIGHT;
	spriteData.x = shipNS::X;                   // ��ʏ�̈ʒu
	spriteData.y = shipNS::Y;
	spriteData.rect.bottom = shipNS::HEIGHT;    // ��ʂ̈ꕔ��I��
	spriteData.rect.right = shipNS::WIDTH;
	oldX = shipNS::X;
	oldY = shipNS::Y;
	oldAngle = 0.0f;
	rotation = 0.0f;
	velocity.x = 0;								// ���xX
	velocity.y = 0;
	frameDelay = shipNS::SHIP_ANIMATION_DELAY;
	// �F���D�A�j���[�V�����̍ŏ��̃t���[��
	startFrame = shipNS::SHIP1_START_FRAME;
	// �F���D�A�j���[�V�����̍Ō�̃t���[��
	endFrame = shipNS::SHIP1_END_FRAME;
	currentFrame = startFrame;
	radius = shipNS::WIDTH / 2.0;
	collisionType = entityNS::CIRCLE;
	direction = shipNS::NONE;                   // ��]�̗͂̕���
	engineOn = false;
	shieldOn = false;
	explosionOn = false;
	mass = shipNS::MASS;
}

//=============================================================================
// �F���D��������
// ���s��F���������ꍇ��true�A���s�����ꍇ��false��߂�
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
	shield.setLoop(false);                  // �A�j���[�V���������[�v���Ȃ�
	explosion.initialize(gamePtr->getGraphics(), width, height, ncols, textureM);
	explosion.setFrames(shipNS::EXPLOSION_START_FRAME, shipNS::EXPLOSION_END_FRAME);
	explosion.setCurrentFrame(shipNS::EXPLOSION_START_FRAME);
	explosion.setFrameDelay(shipNS::EXPLOSION_ANIMATION_DELAY);
	explosion.setLoop(false);               // �A�j���[�V���������[�v���Ȃ�
	return(Entity::initialize(gamePtr, width, height, ncols, textureM));
}

//=============================================================================
// �F���D��`��
//=============================================================================
void Ship::draw()
{
	if (explosionOn)
		explosion.draw(spriteData);			// ���݂�spriteData���g���Ĕ�����`��
	else
	{
		Image::draw();						// �F���D��`��
		if (engineOn)
			engine.draw(spriteData);		// ���P�b�g�G���W����`��
		if (shieldOn)
			// colorFilter��25%�A���t�@���g���ăV�[���h��`��
			shield.draw(spriteData, graphicsNS::ALPHA50 & colorFilter);
	}
}

//=============================================================================
// Update
// �ʏ�A�t���[�����Ƃ�1��Ăяo��
// frameTime�́A�ړ��ƃA�j���[�V�����̑����𐧌䂷�邽�߂Ɏg�p
//=============================================================================
void Ship::update(float frameTime)
{
	if (explosionOn)
	{
		explosion.update(frameTime);
		// �����A�j���[�V���������������ꍇ
		if (explosion.getAnimationComplete())
		{
			explosionOn = false;                // �������I�t
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
		engine.update(frameTime);
	}


	Entity::update(frameTime);
	oldX = spriteData.x;                        // ���݂̈ʒu��ۑ�
	oldY = spriteData.y;
	oldAngle = spriteData.angle;

	switch (direction)                          // �F���D����]
	{
	case shipNS::LEFT:
		rotation -= frameTime * shipNS::ROTATION_RATE;  // ���ɉ�]
		break;
	case shipNS::RIGHT:
		rotation += frameTime * shipNS::ROTATION_RATE;  // �E�ɉ�]
		break;
	}
	// �F���D����]������
	spriteData.angle += frameTime * rotation;
	spriteData.x += frameTime * velocity.x;     // �F���D��X�����ɓ�����
	spriteData.y += frameTime * velocity.y;     // �F���D��Y�����ɓ�����
	// ��ʂ̒[�ŉ�荞��
	if (spriteData.x > GAME_WIDTH)              // ��ʉE�[�𒴂�����
		spriteData.x = -shipNS::WIDTH;		    // ��ʍ��[�Ɉړ�
	if (spriteData.x < -shipNS::WIDTH)			// ��ʍ��[�𒴂�����
		spriteData.x = GAME_WIDTH;				// ��ʉE�[�Ɉړ�
	if (spriteData.y < -shipNS::HEIGHT)			// ��ʏ�[�𒴂�����
		spriteData.y = GAME_HEIGHT;				// ��ʉ��[�Ɉړ�
	if (spriteData.y > GAME_HEIGHT)             // ��ʉ��[�𒴂�����
		spriteData.y = -shipNS::HEIGHT;			// ��ʏ�[�Ɉړ�
}

//=============================================================================
// �_���[�W
//=============================================================================
void Ship::damage(WEAPON weapon)
{
	if (shieldOn)
		return;

	switch (weapon)
	{
	case TORPEDO:
		audio->playCue(TORPEDO_HIT);
		health -= shipNS::TORPEDO_DAMAGE;
		break;
	case SHIP:
		audio->playCue(COLLIDE);    // �T�E���h���Đ�
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
// ����
//=============================================================================
void Ship::explode()
{
	audio->playCue(EXPLODE);
	active = false;
	health = 0;
	explosionOn = true;
	engineOn = false;
	shieldOn = false;
	velocity.x = 0.0f;
	velocity.y = 0.0f;
}

//=============================================================================
// ��
//=============================================================================
void Ship::repair()
{
	active = true;
	health = FULL_HEALTH;
	explosionOn = false;
	engineOn = false;
	shieldOn = false;
	rotation = 0.0f;
	direction = shipNS::NONE;           // ��]�̗͂̕���
	visible = true;
}