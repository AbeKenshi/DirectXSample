// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Chapter 9 plane.h v1.0

#ifndef _PLANE_H                // ���̃t�@�C���������̉ӏ��ŃC���N���[�h�����ꍇ�ɁA
#define _PLANE_H                // ���d�ɒ�`����邱�Ƃ�h���܂��B
#define WIN32_LEAN_AND_MEAN

#include "entity.h"
#include "constants.h"

namespace planeNS
{
    const int   PLANE_SIZE = 64;            // �e�N�X�`���̃T�C�Y
    const int   PLANE_START_FRAME = 0;      // �e��t���[���̔ԍ�
    const int   PLANE_END_FRAME = 1;
    const int   SHADOW_FRAME = 2;
    const int   TEXTURE_COLS = 2;           // �e�N�X�`���摜���̗�
    const int   PLANE_VERTICAL_SPEED = 64;  // �㏸/���~���[�g
    const int   SHADOW_Y = GAME_HEIGHT/2;   // �e��Y���W
    const int   PLANE_MAX_Y = SHADOW_Y - 10; // ��s�@�̍ő�Y���W
    const int   X = GAME_WIDTH/2;           // ��s�@�̊J�n�ʒu
    const int   Y = GAME_HEIGHT/4;
    const float ROTATION_RATE = (float)PI;  // 1�b������̃��W�A����
    const float SPEED = 100;                // 1�b������100�s�N�Z��
    const float ANIMATION_DELAY = 0.01f;    // �t���[���Ԃ̎���
}

// Entity�N���X���p��
class Plane : public Entity
{
private:
    Image   shadow;         // ��s�@�̉e
    float   planeY;         // ��s�@��Y�̈ꎞ�I�ȕۊǗp

public:
	// �R���X�g���N�^
    Plane();

    // �p�����ꂽ�����o�֐�

	// ��s�@�Ɖe��`��
    virtual void draw();

	// ��s�@��������
	// ���s�O�F*gamePtr = Game�I�u�W�F�N�g�ւ̃|�C���^
	//		   width = Image�̕��i�s�N�Z���P�ʁj�i0 = �e�N�X�`���S�̂̕����g�p�j
	//		   height = Image�̍����i�s�N�Z���P�ʁj�i0 = �e�N�X�`���S�̂̍������g�p�j
	//		   ncols = �e�N�X�`�����̗񐔁i1����n�܂Łj�i0��1�Ɠ����j
	//		   *textureM = TextureManager�I�u�W�F�N�g�ւ̃|�C���^
    virtual bool initialize(Game *gamePtr, int width, int height, int ncols,
                            TextureManager *textureM);
    
	// ��s�@�̃A�j���[�V�����ƈʒu���X�V
    void update(float frameTime);
};
#endif

