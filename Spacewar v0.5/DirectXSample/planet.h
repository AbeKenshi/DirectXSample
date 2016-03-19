#ifndef _PLANET_H               // ���̃t�@�C���������̉ӏ��ŃC���N���[�h�����ꍇ�ɁA
#define _PLANET_H               // ���d�ɒ�`����邱�Ƃ�h���܂��B
#define WIN32_LEAN_AND_MEAN

#include "entity.h"
#include "constants.h"

namespace planetNS
{
	const int   WIDTH = 128;					// �摜�̕�
	const int   HEIGHT = 128;					// �摜�̍���
	const int   COLLISION_RADIUS = 120 / 2;		// �~�̏Փ˔���p
	const int   X = GAME_WIDTH / 2 - WIDTH / 2; // ��ʏ�̈ʒu
	const int   Y = GAME_HEIGHT / 2 - HEIGHT / 2;
	const float MASS = 1.0e14f;					// ����
	const int   TEXTURE_COLS = 2;				// �e�N�X�`����2��
	const int   START_FRAME = 1;				// �t���[��1����J�n
	const int   END_FRAME = 1;					// �A�j���[�V�����Ȃ�
}

class Planet : public Entity					// Entity�N���X���p��
{
public:
	// �R���X�g���N�^
	Planet();
	void  disable() { visible = false; active = false; }
	void  enable() { visible = true; active = true; }
};
#endif

