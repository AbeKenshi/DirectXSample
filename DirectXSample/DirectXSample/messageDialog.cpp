// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// messageDialog.cpp v1.1

#include "messageDialog.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
MessageDialog::MessageDialog()
{
    initialized = false;                // ����ɏ��������ꂽ�ꍇ�Atrue��ݒ�
    graphics = NULL;
    visible = false;                    // ��\���ɂ���
    fontColor = messageDialogNS::FONT_COLOR;
    borderColor = messageDialogNS::BORDER_COLOR;
    backColor = messageDialogNS::BACK_COLOR;
    buttonColor = messageDialogNS::BUTTON_COLOR;
    buttonFontColor = messageDialogNS::BUTTON_FONT_COLOR;
    x = messageDialogNS::X;                // �J�n�ʒu
    y = messageDialogNS::Y;
    height = messageDialogNS::HEIGHT;
    width = messageDialogNS::WIDTH;
    textRect.bottom = messageDialogNS::Y + messageDialogNS::HEIGHT - messageDialogNS::MARGIN;
    textRect.left = messageDialogNS::X + messageDialogNS::MARGIN;
    textRect.right = messageDialogNS::X + messageDialogNS::WIDTH - messageDialogNS::MARGIN;
    textRect.top = messageDialogNS::Y + messageDialogNS::MARGIN;
    dialogVerts = NULL;
    borderVerts = NULL;
    buttonVerts = NULL;
    button2Verts = NULL;
    buttonType = 0;     // OK/Cancel
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
MessageDialog::~MessageDialog()
{
    onLostDevice();            // ���ׂẴO���t�B�b�N�A�C�e���ɂ���onLostDevice()���Ăяo��
}

//=============================================================================
// MessageDialog��������
//=============================================================================
bool MessageDialog::initialize(Graphics *g, Input *in, HWND h)
{
    try {
        graphics = g;                   // Graphics�I�u�W�F�N�g
        input = in;                     // Input�I�u�W�F�N�g
        hwnd = h;

		// DirectX�t�H���g��������
        if(dxFont.initialize(graphics, messageDialogNS::FONT_HEIGHT, false, 
                             false, messageDialogNS::FONT) == false)
            return false;               // ���s�̏ꍇ
        dxFont.setFontColor(fontColor);
    } catch(...) {
        return false;
    }

    initialized = true;
    return true;
}

//=============================================================================
// �_�C�A���O�̔w�i�ƃ{�^����`�悷�邽�߂ɒ��_�o�b�t�@������
//=============================================================================
void MessageDialog::prepareVerts()
{
    SAFE_RELEASE(dialogVerts);
    SAFE_RELEASE(borderVerts);
    SAFE_RELEASE(buttonVerts);
    SAFE_RELEASE(button2Verts);

	// �g�̍���
    vtx[0].x = x;
    vtx[0].y = y;
    vtx[0].z = 0.0f;
    vtx[0].rhw = 1.0f;
    vtx[0].color = borderColor;

    // �g�̉E��
    vtx[1].x = x + width;
    vtx[1].y = y;
    vtx[1].z = 0.0f;
    vtx[1].rhw = 1.0f;
    vtx[1].color = borderColor;

	// �g�̉E��
    vtx[2].x = x + width;
    vtx[2].y = y + height;
    vtx[2].z = 0.0f;
    vtx[2].rhw = 1.0f;
    vtx[2].color = borderColor;

	// �g�̍���
    vtx[3].x = x;
    vtx[3].y = y + height;
    vtx[3].z = 0.0f;
    vtx[3].rhw = 1.0f;
    vtx[3].color = borderColor;

    graphics->createVertexBuffer(vtx, sizeof vtx, borderVerts);

    // background top left
    vtx[0].x = x + messageDialogNS::BORDER;
    vtx[0].y = y + messageDialogNS::BORDER;
    vtx[0].z = 0.0f;
    vtx[0].rhw = 1.0f;
    vtx[0].color = backColor;

    // background top right
    vtx[1].x = x + width - messageDialogNS::BORDER;
    vtx[1].y = y + messageDialogNS::BORDER;
    vtx[1].z = 0.0f;
    vtx[1].rhw = 1.0f;
    vtx[1].color = backColor;

    // background bottom right
    vtx[2].x = x + width - messageDialogNS::BORDER;
    vtx[2].y = y + height - messageDialogNS::BORDER;
    vtx[2].z = 0.0f;
    vtx[2].rhw = 1.0f;
    vtx[2].color = backColor;

    // background bottom left
    vtx[3].x = x + messageDialogNS::BORDER;
    vtx[3].y = y + height - messageDialogNS::BORDER;
    vtx[3].z = 0.0f;
    vtx[3].rhw = 1.0f;
    vtx[3].color = backColor;

    graphics->createVertexBuffer(vtx, sizeof vtx, dialogVerts);

    // button top left
    vtx[0].x = x + width/2.0f - messageDialogNS::BUTTON_WIDTH/2.0f;
    vtx[0].y = y + height - messageDialogNS::BORDER - messageDialogNS::MARGIN - messageDialogNS::BUTTON_HEIGHT;
    vtx[0].z = 0.0f;
    vtx[0].rhw = 1.0f;
    vtx[0].color = buttonColor;

    // button top right
    vtx[1].x = x + width/2.0f + messageDialogNS::BUTTON_WIDTH/2.0f;
    vtx[1].y = vtx[0].y;
    vtx[1].z = 0.0f;
    vtx[1].rhw = 1.0f;
    vtx[1].color = buttonColor;

    // button bottom right
    vtx[2].x =  vtx[1].x;
    vtx[2].y = vtx[0].y + messageDialogNS::BUTTON_HEIGHT;
    vtx[2].z = 0.0f;
    vtx[2].rhw = 1.0f;
    vtx[2].color = buttonColor;

    // button bottom left
    vtx[3].x = vtx[0].x;
    vtx[3].y = vtx[2].y;
    vtx[3].z = 0.0f;
    vtx[3].rhw = 1.0f;
    vtx[3].color = buttonColor;

    graphics->createVertexBuffer(vtx, sizeof vtx, buttonVerts);

    // set buttonRect
    buttonRect.left   = (long)vtx[0].x;
    buttonRect.right  = (long)vtx[1].x;
    buttonRect.top    = (long)vtx[0].y;
    buttonRect.bottom = (long)vtx[2].y;

    // button2 top left
    vtx[0].x = x + width - messageDialogNS::BUTTON_WIDTH*1.2f;
    vtx[0].y = y + height - messageDialogNS::BORDER - messageDialogNS::MARGIN - messageDialogNS::BUTTON_HEIGHT;
    vtx[0].z = 0.0f;
    vtx[0].rhw = 1.0f;
    vtx[0].color = buttonColor;
    // button2 top right
    vtx[1].x = vtx[0].x + messageDialogNS::BUTTON_WIDTH;
    vtx[1].y = vtx[0].y;
    vtx[1].z = 0.0f;
    vtx[1].rhw = 1.0f;
    vtx[1].color = buttonColor;
    // button2 bottom right
    vtx[2].x =  vtx[1].x;
    vtx[2].y = vtx[0].y + messageDialogNS::BUTTON_HEIGHT;
    vtx[2].z = 0.0f;
    vtx[2].rhw = 1.0f;
    vtx[2].color = buttonColor;
    // button2 bottom left
    vtx[3].x = vtx[0].x;
    vtx[3].y = vtx[2].y;
    vtx[3].z = 0.0f;
    vtx[3].rhw = 1.0f;
    vtx[3].color = buttonColor;
    graphics->createVertexBuffer(vtx, sizeof vtx, button2Verts);

    // set button2Rect
    button2Rect.left   = (long)vtx[0].x;
    button2Rect.right  = (long)vtx[1].x;
    button2Rect.top    = (long)vtx[0].y;
    button2Rect.bottom = (long)vtx[2].y;
}

//=============================================================================
// MessageDialog��`��
//=============================================================================
const void MessageDialog::draw()
{
    if (!visible || graphics == NULL || !initialized)
        return;

    graphics->drawQuad(borderVerts);        // �g��`��
    graphics->drawQuad(dialogVerts);        // �w�i��`��
    graphics->drawQuad(buttonVerts);        // �{�^����`��
    graphics->drawQuad(button2Verts);       // �{�^��2��`��

    graphics->spriteBegin();                // �X�v���C�g�̕`����J�n

    if(text.size() == 0)
        return;
	// �e�L�X�g��MassegeDialog�ɕ\��
    dxFont.setFontColor(fontColor);
    dxFont.print(text,textRect,DT_CENTER|DT_WORDBREAK);

	// �e�L�X�g���{�^���ɕ\��
    dxFont.setFontColor(buttonFontColor);
    dxFont.print(messageDialogNS::BUTTON1_TEXT[buttonType],buttonRect,
                 DT_SINGLELINE|DT_CENTER|DT_VCENTER);
    dxFont.print(messageDialogNS::BUTTON2_TEXT[buttonType],button2Rect,
                 DT_SINGLELINE|DT_CENTER|DT_VCENTER);

    graphics->spriteEnd();                  // �X�v���C�g�̕`����J�n
}

//=============================================================================
// DIALOG_CLOSE_KEY��OK�{�^���̃N���b�N���`�F�b�N
//=============================================================================
void MessageDialog::update()
{
    if (!initialized || !visible)
        return;
    if (input->wasKeyPressed(messageDialogNS::DIALOG_CLOSE_KEY))
    {
        visible = false;
        buttonClicked = 1;              // �{�^��1���N���b�N���ꂽ
        return;
    }

    if (graphics->getFullscreen() == false) // �E�B���h�E�\���̏ꍇ
    {
		// �E�B���h�E���T�C�Y�ύX����Ă���ꍇ���l�����āA��ʂ̔䗦���v�Z
        RECT clientRect;
        GetClientRect(hwnd, &clientRect);
        screenRatioX = (float)GAME_WIDTH / clientRect.right;
        screenRatioY = (float)GAME_HEIGHT / clientRect.bottom;
    }

    if (input->getMouseLButton())       // �}�E�X�̍��{�^���̏ꍇ
    {
		// �}�E�X�N���b�N���{�^��1�iOK�j�̓����Ŕ��������ꍇ
        if (input->getMouseX()*screenRatioX >= buttonRect.left &&
            input->getMouseX()*screenRatioX <= buttonRect.right &&
            input->getMouseY()*screenRatioY >= buttonRect.top &&
            input->getMouseY()*screenRatioY <= buttonRect.bottom)
        {
            visible = false;            // ���b�Z�[�W�_�C�A���O���\��
            buttonClicked = 1;          // �{�^��1���N���b�N���ꂽ
            return;
        }

		// �}�E�X�N���b�N�{�^��2�iCancel�j�̓����Ŕ��������ꍇ
        if (input->getMouseX()*screenRatioX >= button2Rect.left &&
            input->getMouseX()*screenRatioX <= button2Rect.right &&
            input->getMouseY()*screenRatioY >= button2Rect.top &&
            input->getMouseY()*screenRatioY <= button2Rect.bottom)
        {
            visible = false;            // ���b�Z�[�W�_�C�A���O���\��
            buttonClicked = 2;          // �{�^��2���N���b�N���ꂽ
        }
    }
}

//=============================================================================
// �e�L�X�g�������ݒ�A�_�C�A���O�̉��[���e�L�X�g�����܂�T�C�Y�ɕύX�A
// visible = true��ݒ�
//=============================================================================
void MessageDialog::print(const std::string &str)         
{
	// ����������Ă��Ȃ��A�܂��͊��Ɏg�p���̏ꍇ
    if (!initialized || visible)
        return;
    text = str + "\n\n\n\n";        // �{�^���p�ɋ�Ԃ��󂯂�

	// textRect���_�C�A���O�̃e�L�X�g�̈�Ƃ��Đݒ�
    textRect.left   = (long)(x + messageDialogNS::MARGIN);
    textRect.right  = (long)(x + messageDialogNS::WIDTH - messageDialogNS::MARGIN);
    textRect.top    = (long)(y + messageDialogNS::MARGIN);
    textRect.bottom = (long)(y + messageDialogNS::HEIGHT - messageDialogNS::MARGIN);

	// textRect.bottom���e�L�X�g�ɕK�v�ȍ����ɐݒ�
	// DT_CALCERCT�I�v�V�����̏ꍇ�A�e�L�X�g�͏o�͂���Ȃ�
    dxFont.print(text,textRect,DT_CENTER|DT_WORDBREAK|DT_CALCRECT);
    height = textRect.bottom - (int)y + messageDialogNS::BORDER + messageDialogNS::MARGIN;

    prepareVerts();                 // ���_�o�b�t�@������
    buttonClicked = 0;              // buttonClicked�N���A
    visible = true;
}

//=============================================================================
// �O���t�B�b�N�X�f�o�C�X�����������Ƃ��ɌĂяo��
//=============================================================================
void MessageDialog::onLostDevice()
{
    if (!initialized)
        return;
    dxFont.onLostDevice();
    SAFE_RELEASE(dialogVerts);
    SAFE_RELEASE(borderVerts);
    SAFE_RELEASE(buttonVerts);
    SAFE_RELEASE(button2Verts);
}

//=============================================================================
// �O���t�B�b�N�X�f�o�C�X�����Z�b�g���ꂽ�Ƃ��ɌĂяo��
//=============================================================================
void MessageDialog::onResetDevice()
{
    if (!initialized)
        return;
    prepareVerts();
    dxFont.onResetDevice();
}

