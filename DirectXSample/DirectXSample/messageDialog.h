// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// messageDialog.h v1.0

#ifndef _MESSAGEDIALOG_H        // prevent multiple definitions if this 
#define _MESSAGEDIALOG_H        // ..file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <string>
#include "constants.h"
#include "textDX.h"
#include "graphics.h"
#include "input.h"

namespace messageDialogNS
{
    const UINT WIDTH = 400;             // �_�C�A���O�̃f�t�H���g�̕�
    const UINT HEIGHT = 100;            // �f�t�H���g�̍���
    const UINT BORDER = 5;
    const UINT MARGIN = 5;              // �g����e�L�X�g�܂ł̗]��
    const char FONT[] = "Arial";        // �t�H���g
    const int FONT_HEIGHT = 18;         // �t�H���g�̍���
    const COLOR_ARGB FONT_COLOR = graphicsNS::WHITE;        // �e�L�X�g�̐F
    const COLOR_ARGB BORDER_COLOR = D3DCOLOR_ARGB(192,192,192,192); // �g�̐F
    const COLOR_ARGB BACK_COLOR = SETCOLOR_ARGB(255,100,100,192);   // �w�i�̐F
    const UINT X = GAME_WIDTH/2 - WIDTH/2; // �f�t�H���g�̈ʒu
    const UINT Y = GAME_HEIGHT/4 - HEIGHT/2;
    const UINT BUTTON_WIDTH = (UINT)(FONT_HEIGHT * 4.5);
    const UINT BUTTON_HEIGHT = FONT_HEIGHT + 4;
    const int MAX_TYPE = 2;
    const int OK_CANCEL = 0;            // OK/Cancel�^�C�v�̃{�^��
    const int YES_NO = 1;               // Yes/No�^�C�v�̃{�^��
    static const char* BUTTON1_TEXT[MAX_TYPE] = {"OK", "YES"};
    static const char* BUTTON2_TEXT[MAX_TYPE] = {"CANCEL", "NO"};
    const byte DIALOG_CLOSE_KEY = VK_RETURN;    // Enter�L�[
    const COLOR_ARGB BUTTON_COLOR = graphicsNS::GRAY;       // �{�^���w�i
    const COLOR_ARGB BUTTON_FONT_COLOR = graphicsNS::WHITE; // �{�^���e�L�X�g�̐F
}

// ���b�Z�[�W�_�C�A���O
class MessageDialog
{
protected:
    Graphics    *graphics;              // �O���t�B�b�N�V�X�e��
    Input       *input;                 // ���̓V�X�e��
    TextDX      dxFont;                 // DirectX�t�H���g
    float       x,y;                    // ��ʂ̈ʒu
    UINT        height;                 // �_�C�A���O�̍����Aprint()�Ōv�Z
    UINT        width;                  // �_�C�A���O�̕�
    std::string text;                   // �_�C�A���O�e�L�X�g
    RECT        textRect;               // �e�L�X�g�̋�`
    RECT        buttonRect;             // �{�^���̋�`
    RECT        button2Rect;            // �{�^��2�̋�`
    COLOR_ARGB  fontColor;              // �t�H���g�̐F(a,r,g,b)
    COLOR_ARGB  borderColor;            // �g�̐F(a,r,g,b)
    COLOR_ARGB  backColor;              // �w�i�̐F(a,r,g,b)
    COLOR_ARGB  buttonColor;            // �{�^���̐F
    COLOR_ARGB  buttonFontColor;        // �{�^���t�H���g�̐F
    VertexC vtx[4];                     // ���_�f�[�^
    LP_VERTEXBUFFER dialogVerts;        // �_�C�A���O�̒��_�o�b�t�@
    LP_VERTEXBUFFER borderVerts;        // �g�̒��_�o�b�t�@
    LP_VERTEXBUFFER buttonVerts;        // �{�^���̒��_�o�b�t�@
    LP_VERTEXBUFFER button2Verts;       // �{�^��2�̒��_�o�b�t�@
    int buttonClicked;                  // �ǂ̃{�^�����N���b�N���ꂽ���i1�܂���2�j
    int buttonType;                     // 0 = OK/Cancel, 1 = Yes/No
    bool    initialized;                // ����ɏ��������ꂽ�ꍇ�Atrue
    bool    visible;                    // true�̏ꍇ�͕\������
    HWND    hwnd;                       // �E�B���h�E�ւ̃n���h��
    float   screenRatioX, screenRatioY;

public:
	// �R���X�g���N�^
    MessageDialog();
    // �f�X�g���N�^
    virtual ~MessageDialog();

    // MessageDialog��������
	// ���s�O�F*g��Graphics�I�u�W�F�N�g���w��
	//		   *in��Input�I�u�W�F�N�g���w��
	//		   hwnd = �E�B���h�E�n���h��
    bool initialize(Graphics *g, Input *in, HWND hwnd);

	// ���_�o�b�t�@������
    void prepareVerts();

	// MessageDialog��\��
    const void draw();

	// �N���b�N���ꂽ�{�^����߂�
	// 0 = �{�^���̓N���b�N����Ă��Ȃ�
	// 1�͍��̃{�^���A2�͉E�̃{�^��
    int getButtonClicked()  {return buttonClicked;}

	// �\��/��\���̏�Ԃ�߂�
    bool getVisible() {return visible;}

	// �t�H���g�̐F��ݒ�
    void setFontColor(COLOR_ARGB fc)    {fontColor = fc;}

	// �g�̐F��ݒ�
    void setBorderColor(COLOR_ARGB bc)  {borderColor = bc;}

	// �w�i�̐F��ݒ�
    void setBackColor(COLOR_ARGB bc)    {backColor = bc;}

	// �{�^���̐F��ݒ�
    void setButtonColor(COLOR_ARGB bc)  {buttonColor = bc;}

	// �{�^���t�H���g�̐F��ݒ�
    void setButtonFontColor(COLOR_ARGB bfc) {buttonFontColor = bfc;}

	// �\��/��\����ݒ�
    void setVisible(bool v) {visible = v;}

    // �{�^���^�C�v��ݒ�i0 = OK/CANCEL, 1 = YES/NO�j
    void setButtonType(UINT t)
    {
        if(t < messageDialogNS::MAX_TYPE)
            buttonType = t;
    }

	// �e�L�X�gstr��MessageDialog�ɕ\��
    void print(const std::string &str);

	// �u����v�C�x���g���`�F�b�N
    void update();

	// �O���t�B�b�N�X�f�o�C�X�����������Ƃ��ɌĂяo��
    void onLostDevice();

	// �O���t�B�b�N�X�f�o�C�X�����Z�b�g���ꂽ�Ƃ��ɌĂяo��
    void onResetDevice();
};

#endif

