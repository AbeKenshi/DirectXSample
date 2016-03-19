#ifndef _TEXTUREMANAGER_H		// ���̃t�@�C���������̉ӏ��ŃC���N���[�h�����ꍇ�ɁA
#define _TEXTUREMANAGER_H		// ���d�ɒ�`����邱�Ƃ�h���܂��B
#define WIN32_LEAN_AND_MEAN

#include <vector>
#include <string>
#include <fstream>
#include "graphics.h"
#include "constants.h"

class TextureManager
{
private:
	// TextureManager �v���p�e�B
	std::vector<UINT>       width;      // width of texture in pixels
	std::vector<UINT>       height;     // height of texture in pixels
	std::vector<LP_TEXTURE> texture;    // pointer to textures
	std::vector<std::string> fileNames; // texture file names
	Graphics *graphics;		// graphics�ւ̃|�C���^
	bool initialized;		// �������ɐ��������ꍇ�Atrue
	HRESULT hr;				// �W���̖߂�l
public:
	// �R���X�g���N�^
	TextureManager();

	// �f�X�g���N�^
	virtual ~TextureManager();

	// �e�N�X�`���ւ̃|�C���^��߂�
	LP_TEXTURE getTexture(UINT n = 0) const {
		if (n >= texture.size())
			return NULL;
		return texture[n];
	}

	// �e�N�X�`���̕���߂�
	UINT getWidth(UINT n = 0) const
	{
		if (n >= texture.size())
			return 0;
		return width[n];
	}

	// �e�N�X�`���̍�����߂�
	UINT getHeight(UINT n = 0) const
	{
		if (n >= texture.size())
			return 0;
		return height[n];
	}

	// �e�N�X�`���t�@�C�����f�B�X�N����ǂݍ���
	// ���s��F���������ꍇ��true�A���s�����ꍇ��false��߂�
    virtual bool initialize(Graphics *g, std::string file);

	// �O���t�B�b�N�X�f�o�C�X�����������Ƃ��ɌĂяo�����
	virtual void onLostDevice();

	// �O���t�B�b�N�X�f�o�C�X�����Z�b�g���ꂽ�Ƃ��ɌĂяo�����
	virtual void onResetDevice();

	// Safely release texture
	void safeReleaseTexture(LP_TEXTURE& ptr)
	{
		if (ptr)
		{
			ptr->Release();
			ptr = NULL;
		}
	}
};
#endif // !_TEXTUREMANAGER_H
