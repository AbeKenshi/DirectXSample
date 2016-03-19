#include "textureManager.h"

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
TextureManager::TextureManager()
{
	graphics = NULL;
	initialized = false;            // �������ɐ��������ꍇ�́Atrue
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
TextureManager::~TextureManager()
{
	for (UINT i = 0; i<texture.size(); i++)
		safeReleaseTexture(texture[i]);
}

//=============================================================================
// �e�N�X�`���t�@�C�����f�B�X�N����ǂݍ���
// ���s��F���������ꍇ��true�A���s�����ꍇ��false��߂�
//=============================================================================
bool TextureManager::initialize(Graphics *g, std::string file)
{
	bool success = true;
	try {
		graphics = g;                       // the graphics object
		for (UINT i = 0; i<file.size(); i++)    // convert to lowercase
			file.at(i) = tolower(file.at(i));
		if (file.rfind(".txt") == file.size() - 4) // if .txt extension
		{
			// open file containing individual texture names
			std::ifstream infile(file.c_str());
			if (!infile)                     // if open failed
				return false;
			std::string name;
			while (getline(infile, name))
			{
				fileNames.push_back(name);  // add to files
				width.push_back(0);         // make room for width
				height.push_back(0);        // make room for height
				texture.push_back(NULL);    // make room for texture
			}
			infile.close();
		}
		else    // not .txt file so file contains name of one texture
		{
			fileNames.push_back(file);      // put one file name in files
			width.push_back(0);         // make room for width
			height.push_back(0);        // make room for height
			texture.push_back(NULL);    // make room for texture
		}

		// load texture files
		for (UINT i = 0; i<fileNames.size(); i++)
		{
			hr = graphics->loadTexture(fileNames[i].c_str(),
				graphicsNS::TRANSCOLOR, width[i], height[i], texture[i]);
			if (FAILED(hr))
				success = false;    // at least one texture failed to load
		}
	}
	catch (...) { return false; }
	initialized = true;                    // set true when initialized
	return success;
}

//=============================================================================
// �O���t�B�b�N�X�f�o�C�X�����������Ƃ��ɌĂяo�����
//=============================================================================
void TextureManager::onLostDevice()
{
	try
	{
		if (!initialized)
			return;
		for (UINT i = 0; i<texture.size(); i++)
			safeReleaseTexture(texture[i]);
	}
	catch (...)
	{
		throw(GameError(gameErrorNS::WARNING,
			"Warning, TextureManager onLostDevice attempted to access an invalid texture."));
	}
}

//=============================================================================
// �O���t�B�b�N�X�f�o�C�X�����Z�b�g���ꂽ�Ƃ��ɌĂяo�����
//=============================================================================
void TextureManager::onResetDevice()
{
	if (!initialized)
		return;
	// load texture files
	for (UINT i = 0; i<fileNames.size(); i++)
	{
		hr = graphics->loadTexture(fileNames[i].c_str(),
			graphicsNS::TRANSCOLOR, width[i], height[i], texture[i]);
		if (FAILED(hr))
			safeReleaseTexture(texture[i]);
	}
}