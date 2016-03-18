// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// console.h v1.0

#ifndef _CONSOLE_H              // Prevent multiple definitions if this 
#define _CONSOLE_H              // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <string>
#include <deque>
#include "constants.h"
#include "textDX.h"
#include "graphics.h"
#include "input.h"

namespace consoleNS
{
    const UINT WIDTH = 500;             // �R���\�[���̋�`�̕��i�s�N�Z���P�ʁj
    const UINT HEIGHT = 400;            // �R���\�[���̋�`�̍����i�s�N�Z���P�ʁj
    const UINT X = 5;                   // �R���\�[���̍��������̉�ʈʒu
    const UINT Y = 5;
    const UINT MARGIN = 4;              // �R���\�[���̒[����e�L�X�g�܂ł̗]��
    const char FONT[] = "Courier New";  // �R���\�[���̃e�L�X�g�Ɏg�p����t�H���g
    const int FONT_HEIGHT = 14;         // �R���\�[���̃t�H���g�̍���
    const COLOR_ARGB FONT_COLOR = graphicsNS::WHITE;    // �R���\�[���̃t�H���g�̐F
    const COLOR_ARGB BACK_COLOR = SETCOLOR_ARGB(192,128,128,128);    // �R���\�[���̔w�i�̐F
    const int MAX_LINES = 256;          // �e�L�X�g�o�b�t�@���̕\���\�ȃe�L�X�g�̍ő�s��
}

// 
class Console
{
private:
    Graphics    *graphics;              // graphics system
    Input       *input;                 // input system
    TextDX      dxFont;                 // DirectX font
    float       x,y;                    // console location (dynamic)
    int         rows;                   // number of rows of text that will fit on console
    std::string commandStr;             // console command
    std::string inputStr;               // console text input
    std::deque<std::string> text;       // console text
    RECT        textRect;               // text rectangle
    COLOR_ARGB  fontColor;              // font color (a,r,g,b)
    COLOR_ARGB  backColor;              // background color (a,r,g,b)
    VertexC vtx[4];                     // vertex data for background
    LP_VERTEXBUFFER vertexBuffer;       // buffer to hold vertex data
    int         scrollAmount;           // number of lines to scroll the display up
    bool        initialized;            // true when initialized successfully
    bool        visible;                // true to display

public:
	// �R���X�g���N�^
    Console();

	// �f�X�g���N�^
    virtual ~Console();

	// �R���\�[����������
    bool initialize(Graphics *g, Input *in);

	// �R���\�[����`��
	// ���s�O�FBeginScene��EndScene�̓����ɂ���
    const void draw();

	// �R���\�[���̕\��/��\���𐧌䂷��v���p�e�B�𔽓]�����܂��B
	// �\������Ă���R���\�[���͔�\���ɂȂ�A
	// ��\���̃R���\�[���͕\�������悤�ɂȂ�܂��B
    void showHide();

	// �\��/��\���𐧌䂷��v���p�e�B�̌��݂̏�Ԃ�߂��܂��B
    bool getVisible() {return visible;}

	// �R���\�[����\�����܂��B
    void show() {visible = true;}

	// �R���\�[�����\���ɂ��܂��B
    void hide() {visible = false;}

	// �R���\�[���f�B�X�v���C�Ƀe�L�X�g�������ǉ����܂��B
    void print(const std::string &str);

	// �V���O���L�[�̃R���\�[���R�}���h���������܂��B
	// ���[�U�[�����͂����R�}���h�������߂��܂��B
    std::string getCommand();

	// �R���\�[���̓��̓e�L�X�g��߂��܂�
    std::string getInput() {return inputStr;}

	// �R���\�[���̓��̓e�L�X�g���N���A���܂�
    void clearInput()   {inputStr = "";}

    // Call when graphics device is lost.
    void onLostDevice();

    // Call when graphics device is reset.
    void onResetDevice();
};

#endif

