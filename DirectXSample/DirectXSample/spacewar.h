#ifndef _SPACEWAR_H             // ���̃t�@�C���������̉ӏ��ŃC���N���[�h�����ꍇ�ɁA
#define _SPACEWAR_H             // ���d�ɒ�`����邱�Ƃ�h���܂��B
#define WIN32_LEAN_AND_MEAN

#include "game.h"

//=============================================================================
// Spacewar�͐V�����쐬����N���X�BGame�N���X���p������
//=============================================================================
class Spacewar : public Game
{
private:
	// �ϐ�

public:
	// �R���X�g���N�^
	Spacewar();

	// �f�X�g���N�^
	virtual ~Spacewar();

	// �Q�[����������
	void initialize(HWND hwnd);
	void update();      // Game����̏������z�֐����I�[�o�[���C�h����K�v������
	void ai();          // "
	void collisions();  // "
	void render();      // "
	void releaseAll();
	void resetAll();
};

#endif
