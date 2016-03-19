#ifndef _SPACEWAR_H             // ���̃t�@�C���������̉ӏ��ŃC���N���[�h�����ꍇ�ɁA
#define _SPACEWAR_H             // ���d�ɒ�`����邱�Ƃ�h���܂��B
#define WIN32_LEAN_AND_MEAN

#include "game.h"
#include "textureManager.h"
#include "image.h"

//=============================================================================
// ���̃N���X�̓Q�[���G���W���̃R�A
//=============================================================================
class Spacewar : public Game
{
private:
	// �Q�[���A�C�e��
	TextureManager nebulaTexture;	// ���_�inebula�j�̃e�N�X�`��
	TextureManager planetTexture;	// �f���iplanet�j�̃e�N�X�`��
	TextureManager shipTexture;		// �F���D�̃e�N�X�`��
	Image planet;					// �f���iplanet�j�̉摜
	Image nebula;					// ���_�inebula�j�̉摜
	Image ship;						// �F���D�̉摜
	float Xvelocity;				// �F���D��X�����ւ̃X�s�[�h
	float Yvelocity;				// �F���D��Y�����ւ̃X�s�[�h

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
