#ifndef _GAME_H	// ���̃t�@�C���������̉ӏ��ŃC���N���[�h�����ꍇ�ɁA
#define _GAME_H // ���d�ɒ�`����邱�Ƃ�h���܂��B
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <mmsystem.h>
#include "graphics.h"
#include "input.h"
#include "constants.h"
#include "gameError.h"
#include "audio.h"
#include "textDX.h"
#include "console.h"

namespace gameNS
{
	const char FONT[] = "Courier New";  // �t�H���g
	const int POINT_SIZE = 14;          // �|�C���g�T�C�Y
	const COLOR_ARGB FONT_COLOR = SETCOLOR_ARGB(255, 255, 255, 255);    // ��
}

class Game
{
protected:
	// ���ʂ̃Q�[���v���p�e�B
	Graphics *graphics;			// Graphics�ւ̃|�C���^
	Input *input;				// Input�ւ̃|�C���^
	Audio *audio;				// Audio�ւ̃|�C���^
	Console *console;			// Console�ւ̃|�C���^
	HWND hwnd;					// �E�B���h�E�n���h��
	HRESULT hr;					// �W���̖߂�^
	LARGE_INTEGER timeStart;	// �p�t�H�[�}���X�J�E���^�[�̊J�n�l
	LARGE_INTEGER timeEnd;		// �p�t�H�[�}���X�J�E���^�[�̏I���l
	LARGE_INTEGER timerFreq;	// �p�t�H�[�}���X�J�E���^�[�̎��g��
	float frameTime;			// �Ō�̃t���[���ɗv��������
	float fps;					// �t���[�����[�g�i1�b������̃t���[����)
	TextDX dxFont;				// �t���[�����[�g�p��DirectX�t�H���g
	bool fpsOn;					// true�̏ꍇ�t���[�����[�g��\��
	DWORD sleepTime;			// �t���[���ԂŃX���[�v���鎞�ԁi�~���b�j
	bool paused;				// �Q�[�����ꎞ��~����Ă���ꍇ�Atrue
	bool initialized;
	std::string  command;       // �R���\�[������̃R�}���h
public:
	// �R���X�g���N�^
	Game();
	// �f�X�g���N�^
	virtual ~Game();

	// �����o�[�֐�

	// Windows���b�Z�[�W�n���h��
	LRESULT messageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	
	// �Q�[����������
	// ���s�O�Fhwnd�̓E�B���h�E�ւ̃n���h��
	virtual void initialize(HWND hwnd);
	
	// WinMain���̃��C���̃��b�Z�[�W���[�v��run���J��Ԃ��Ăяo��
	virtual void run(HWND);
	
	// �O���t�B�b�N�X�f�o�C�X�����������Ƃ��ɌĂяo��
	// �O���t�B�b�N�X�f�o�C�X�����Z�b�g�\�ɂ��邽�߁A
	// �\�񂳂�Ă����r�f�I�����������ׂĉ��
	virtual void releaseAll();
	
	// ���ׂẴT�[�t�F�C�X���č쐬���A���ׂẴG���e�B�e�B�����Z�b�g
	virtual void resetAll();
	
	// �\�񂳂�Ă��������������ׂč폜
	virtual void deleteAll();

	// �R���\�[���R�}���h������
	virtual void consoleCommand();
	
	// �Q�[���A�C�e���������_�[
	virtual void renderGame();
	
	// ���������O���t�B�b�N�X�f�o�C�X������
	virtual void handleLostGraphicsDevice();

	// Set display mode (fullscreen, window or toggle)
	void setDisplayMode(graphicsNS::DISPLAY_MODE mode = graphicsNS::TOGGLE);
	
	// Graphics�ւ̃|�C���^��߂�
	Graphics* getGraphics() { return graphics; }
	
	// Input�ւ̃|�C���^��߂�
	Input* getInput() { return input; }

	// �Q�[�����I��
	void exitGame() { PostMessage(hwnd, WM_DESTROY, 0, 0); }

	// Audio�ւ̃|�C���^��߂�
	Audio* getAudio() { return audio; }
	
	// �������z�֐��̐錾
	// �����̊֐��́AGame���p������N���X���ŋL�q����K�v������܂��B
	// �Q�[���A�C�e�����X�V
	virtual void update() = 0;
	
	// AI�v�Z�����s
	virtual void ai() = 0;
	
	// �Փ˂��`�F�b�N
	virtual void collisions() = 0;
	
	// �O���t�B�b�N�X�������_�[
	// graphics->spriteBegin();���Ăяo��
	// �X�v���C�g��`��
	// graphics->spriteEnd();���Ăяo��
	// �X�v���C�g�ȊO��`��
	virtual void render() = 0;
};

#endif
