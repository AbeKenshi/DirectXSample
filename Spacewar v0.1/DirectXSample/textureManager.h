#ifndef _TEXTUREMANAGER_H		// ���̃t�@�C���������̉ӏ��ŃC���N���[�h�����ꍇ�ɁA
#define _TEXTUREMANAGER_H		// ���d�ɒ�`����邱�Ƃ�h���܂��B
#define WIN32_LEAN_AND_MEAN

#include "graphics.h"
#include "constants.h"

class TextureManager
{
private:
	// TextureManager �v���p�e�B
	UINT width;				// �e�N�X�`���̕��i�s�N�Z���P�ʁj
	UINT height;			// �e�N�X�`���̍����i�s�N�Z���P�ʁj
	LP_TEXTURE texture;		// �e�N�X�`���ւ̃|�C���^
	const char *file;		// �t�@�C����
	Graphics *graphics;		// graphics�ւ̃|�C���^
	bool initialized;		// �������ɐ��������ꍇ�Atrue
	HRESULT hr;				// �W���̖߂�l
public:
	// �R���X�g���N�^
	TextureManager();

	// �f�X�g���N�^
	virtual ~TextureManager();

	// �e�N�X�`���ւ̃|�C���^��߂�
	LP_TEXTURE getTexture() const {
		return texture;
	}

	// �e�N�X�`���̕���߂�
	UINT getWidth() const {
		return width;
	}

	// �e�N�X�`���̍�����߂�
	UINT getHeight() const {
		return height;
	}

	// �e�N�X�`���t�@�C�����f�B�X�N����ǂݍ���
	// ���s��F���������ꍇ��true�A���s�����ꍇ��false��߂�
	virtual bool initialize(Graphics* g, const char *f);

	// �O���t�B�b�N�X�f�o�C�X�����������Ƃ��ɌĂяo�����
	virtual void onLostDevice();

	// �O���t�B�b�N�X�f�o�C�X�����Z�b�g���ꂽ�Ƃ��ɌĂяo�����
	virtual void onResetDevice();

};
#endif // !_TEXTUREMANAGER_H
