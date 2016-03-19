// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Version 1.0

#ifndef _TORPEDO_H              // ���̃t�@�C���������̉ӏ��ŃC���N���[�h�����ꍇ�ɁA
#define _TORPEDO_H              // ���d�ɒ�`����邱�Ƃ�h���܂��B
#define WIN32_LEAN_AND_MEAN

#include "entity.h"
#include "constants.h"

namespace torpedoNS
{
	const int   WIDTH = 32;             // �摜�̕�
	const int   HEIGHT = 32;			// �摜�̍���
	const int   COLLISION_RADIUS = 4;   // �~�̏Փ˔���
	const float SPEED = 200;            // 1�b������̃s�N�Z����
	const float MASS = 300.0f;          // ����
	const float FIRE_DELAY = 4.0f;      // �~�T�C�����˂̊Ԋu��4�b
	const int   TEXTURE_COLS = 8;       // �e�N�X�`����8��
	const int   START_FRAME = 40;       // �t���[��40����J�n
	const int   END_FRAME = 43;         // �A�j���[�V�����t���[���́A40�A41�A42�A43
	const float ANIMATION_DELAY = 0.1f; // �t���[���Ԃ̎���
}

class Torpedo : public Entity           // Entity�N���X���p��
{
private:
	float   fireTimer;                  // ���˂��\�ɂȂ�܂ł̎c�莞��
public:
	// �R���X�g���N�^
	Torpedo();

	// �p�����ꂽ�����o�[�֐�
	void update(float frameTime);
	float getMass()    const { return torpedoNS::MASS; }

	// �V���������o�[�֐�
	void fire(Entity *ship);                // �F���D����~�T�C���𔭎�
};
#endif

