// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// textDX.cpp v1.0
// DirectX font

#include "textDX.h"

//=============================================================================
// default constructor
//=============================================================================
TextDX::TextDX()
{
    color = SETCOLOR_ARGB(255,255,255,255); // default to white font

    // set font position
    fontRect.top = 0;
    fontRect.left = 0;
    fontRect.right = GAME_WIDTH;
    fontRect.bottom = GAME_HEIGHT;
    dxFont = NULL;
    angle  = 0;
}

//=============================================================================
// destructor
//=============================================================================
TextDX::~TextDX()
{
    SAFE_RELEASE(dxFont);
}

//=============================================================================
// DirectX�t�H���g���쐬
//=============================================================================
bool TextDX::initialize(Graphics *g, int height, bool bold, bool italic, 
                        const std::string &fontName)
{
    graphics = g;                   // �O���t�B�b�N�V�X�e��

    UINT weight = FW_NORMAL;
    if(bold)
        weight = FW_BOLD;

	// DirectX�t�H���g���쐬
    if(FAILED(D3DXCreateFont(graphics->get3Ddevice(), height, 0, weight, 1, italic,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE, fontName.c_str(),
        &dxFont))) return false;

    // �ϊ��s���������
    D3DXMatrixTransformation2D(&matrix, NULL, 0.0f, NULL, NULL, 0.0f, NULL);

    return true;
}

//=============================================================================
// �e�L�X�g��x�Ay�ɏo��
// ���s���ɂ�0���A�������ɂ̓e�L�X�g�̍�����߂�
// ���s�O�FspriteBegin()
// ���s��FspriteEnd()
//=============================================================================
int TextDX::print(const std::string &str, int x, int y)
{
    if(dxFont == NULL)
        return 0;
	// �t�H���g�ʒu��ݒ�
    fontRect.top = y;
    fontRect.left = x;

	// ��]���S
    D3DXVECTOR2 rCenter=D3DXVECTOR2((float)x,(float)y);
    // �e�L�X�g�����̊p�x�ŉ�]����s�������
    D3DXMatrixTransformation2D(&matrix, NULL, 0.0f, NULL, &rCenter, angle, NULL);
    // �s����X�v���C�g�ɓK�p
    graphics->getSprite()->SetTransform(&matrix);
    return dxFont->DrawTextA(graphics->getSprite(), str.c_str(), -1, &fontRect, DT_LEFT, color);
}

//=============================================================================
// DirectX�e�L�X�g�������g���āARECT���Ƀe�L�X�g���o��
// ���s���ɂ�0���A�������ɂ̓e�L�X�g�̍�����߂�
// ���s�O�FspriteBegin()
// ���s��FspriteEnd()
//=============================================================================
int TextDX::print(const std::string &str, RECT &rect, UINT format)
{
    if(dxFont == NULL)
        return 0;

	// �e�L�X�g����]�����Ȃ��s�������
    D3DXMatrixTransformation2D(&matrix, NULL, 0.0f, NULL, NULL, NULL, NULL);
	// �s����X�v���C�g�ɓK�p
    graphics->getSprite()->SetTransform(&matrix);
    return dxFont->DrawTextA(graphics->getSprite(), str.c_str(), -1, &rect, format, color);
}

//=============================================================================
// called when graphics device is lost
//=============================================================================
void TextDX::onLostDevice()
{
    if(dxFont == NULL)
        return;
    dxFont->OnLostDevice();
}

//=============================================================================
// called when graphics device is reset
//=============================================================================
void TextDX::onResetDevice()
{
    if(dxFont == NULL)
        return;
    dxFont->OnResetDevice();
}
