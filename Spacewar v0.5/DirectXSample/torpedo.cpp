// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// torpedo.cpp v1.0

#include "torpedo.h"

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
Torpedo::Torpedo() : Entity()
{
	active = false;                                 // �~�T�C���̓A�N�e�B�u�łȂ���Ԃ���J�n
	spriteData.width = torpedoNS::WIDTH;			// 1�̉摜�̃T�C�Y
	spriteData.height = torpedoNS::HEIGHT;
	spriteData.rect.bottom = torpedoNS::HEIGHT;		// �摜���̑I�����镔��
	spriteData.rect.right = torpedoNS::WIDTH;
	cols = torpedoNS::TEXTURE_COLS;
	frameDelay = torpedoNS::ANIMATION_DELAY;
	// �~�T�C���A�j���[�V�����̍ŏ��̃t���[��
	startFrame = torpedoNS::START_FRAME;
	// �~�T�C���A�j���[�V�����̍Ō�̃t���[��
	endFrame = torpedoNS::END_FRAME;
	currentFrame = startFrame;
	radius = torpedoNS::COLLISION_RADIUS;			// �~�̏Փ˔���p
	visible = false;
	fireTimer = 0.0f;
	mass = torpedoNS::MASS;
	collisionType = entityNS::CIRCLE;
}

//=============================================================================
// Updata
// �ʏ�A�t���[�����Ƃ�1��Ăяo��
// frameTime�́A�ړ��ƃA�j���[�V�����̑����𐧌䂷�邽�߂Ɏg�p
//=============================================================================
void Torpedo::update(float frameTime)
{
	fireTimer -= frameTime;							// ���˂��\�ɂȂ�܂ł̎c�莞��

	if (visible == false)
		return;

	if (fireTimer < 0)								// ���ˉ\�ȏꍇ
	{
		visible = false;							// �Â��~�T�C���𖳌���
		active = false;
	}

	Image::update(frameTime);

	spriteData.x += frameTime * velocity.x;     // X�����ɓ�����
	spriteData.y += frameTime * velocity.y;     // Y�����ɓ�����

	// ��ʂ̒[�ŉ�荞��
	if (spriteData.x > GAME_WIDTH)              // ��ʉE�[�ɔ�яo�����ꍇ
		spriteData.x = -torpedoNS::WIDTH;       // ��ʍ��[�ɔz�u
	else if (spriteData.x < -torpedoNS::WIDTH)  // ��ʍ��[�Ŕ�яo�����ꍇ
		spriteData.x = GAME_WIDTH;              // ��ʉE�[�ɔz�u
	if (spriteData.y > GAME_HEIGHT)             // ��ʉ��[�Ŕ�яo�����ꍇ
		spriteData.y = -torpedoNS::HEIGHT;      // ��ʏ�[�ɔz�u
	else if (spriteData.y < -torpedoNS::HEIGHT) // ��ʏ�[�Ŕ�яo�����ꍇ
		spriteData.y = GAME_HEIGHT;             // ��ʉ��[�ɔz�u
}

//=============================================================================
// Fire
// �F���D����~�T�C���𔭎�
//=============================================================================
void Torpedo::fire(Entity *ship)
{
	if (fireTimer <= 0.0f)                       // ���ˉ\�ȏꍇ
	{
		velocity.x = (float)cos(ship->getRadians()) * torpedoNS::SPEED;
		velocity.y = (float)sin(ship->getRadians()) * torpedoNS::SPEED;
		spriteData.x = ship->getCenterX() - spriteData.width / 2;
		spriteData.y = ship->getCenterY() - spriteData.height / 2;
		visible = true;                         // �~�T�C����\��
		active = true;                          // �Փˉ\�ɂ���
		fireTimer = torpedoNS::FIRE_DELAY;      // ���˂̊Ԋu
		audio->playCue(TORPEDO_FIRE);
	}
}

