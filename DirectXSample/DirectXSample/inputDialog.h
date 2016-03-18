// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// inputDialog.h v1.0

#ifndef _INPUTDIALOG_H          // Prevent multiple definitions if this 
#define _INPUTDIALOG_H          // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <string>
#include "constants.h"
#include "textDX.h"
#include "graphics.h"
#include "input.h"
#include "messageDialog.h"

namespace inputDialogNS
{
    const COLOR_ARGB TEXT_BACK_COLOR = graphicsNS::WHITE;   // ���̓e�L�X�g�̔w�i
    const COLOR_ARGB TEXT_COLOR = graphicsNS::BLACK;        // ���̓e�L�X�g�̐F
}

// ���b�Z�[�W�_�C�A���O���p��������̓_�C�A���O
class InputDialog : public MessageDialog
{
private:
    std::string inText;                 // ���̓e�L�X�g
    RECT        inTextRect;
    RECT        tempRect;
    COLOR_ARGB  textBackColor;          // �e�L�X�g�̈�̔w�i�̐F
    COLOR_ARGB  textFontColor;          // �e�L�X�g�̈�̃t�H���g�̐F
    LP_VERTEXBUFFER inTextVerts;        // �e�L�X�g�̈�̒��_�o�b�t�@

public:
	// �R���X�g���N�^
    InputDialog();
	// �f�X�g���N�^
    virtual ~InputDialog();

	// ���_�o�b�t�@������
    void prepareVerts();

	// InputDialog��\��
    const void draw();

	// ���̓e�L�X�g��߂�
    std::string getText()   
    {
        if(!visible)
            return inText;
        else
            return "";
    }

	// ���̓e�L�X�g�̃t�H���g�̐F��ݒ�
    void setTextFontColor(COLOR_ARGB fc)  {textFontColor = fc;}

	// ���̓e�L�X�g�̔w�i�̐F��ݒ�
    void setTextBackColor(COLOR_ARGB bc)  {textBackColor = bc;}

	// �e�L�X�gstr��InputDialog�ɕ\��
    void print(const std::string &str);

	// �u����v�C�x���g���`�F�b�N
    void update();

	// �O���t�B�b�N�X�f�o�C�X�����������Ƃ��ɌĂяo��
    void onLostDevice();
};

#endif

