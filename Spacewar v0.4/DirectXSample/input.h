#ifndef _INPUT_H	// ���̃t�@�C���������̉ӏ��ŃC���N���[�h�����ꍇ�ɁA
#define _INPUT_H // ���d�ɒ�`����邱�Ƃ�h���܂��B
#define WIN32_LEAN_AND_MEAN

class Input;

#include <windows.h>
#include <WindowsX.h>
#include <string>
#include <Xinput.h>
#include "constants.h"
#include "gameError.h"


// for high-definition mouse
#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC      ((USHORT) 0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE     ((USHORT) 0x02)
#endif
//--------------------------

namespace inputNS
{
	const int KEYS_ARRAY_LEN = 256;     // size of key arrays

	// what values for clear(), bit flag
	const UCHAR KEYS_DOWN = 1;
	const UCHAR KEYS_PRESSED = 2;
	const UCHAR MOUSE = 4;
	const UCHAR TEXT_IN = 8;
	const UCHAR KEYS_MOUSE_TEXT = KEYS_DOWN + KEYS_PRESSED + MOUSE + TEXT_IN;
}

// �f�b�h�]�[���Ƃ��Ĕ͈͂�20%���f�t�H���g�Ƃ���
const DWORD GAMEPAD_THUMBSTICK_DEADZONE = (DWORD)(0.20f * 0X7FFF);
const DWORD GAMEPAD_TRIGGER_DEADZONE = 30;                      // �g���K�[�͈̔͂�0����255�܂�
const DWORD MAX_CONTROLLERS = 4;                                // XInput�ŃT�|�[�g�����R���g���[���[�̍ő吔

// state.Gamepad.wButtons�̃Q�[���p�b�h�{�^���ɑΉ�����r�b�g
const DWORD GAMEPAD_DPAD_UP = 0x0001;
const DWORD GAMEPAD_DPAD_DOWN = 0x0002;
const DWORD GAMEPAD_DPAD_LEFT = 0x0004;
const DWORD GAMEPAD_DPAD_RIGHT = 0x0008;
const DWORD GAMEPAD_START_BUTTON = 0x0010;
const DWORD GAMEPAD_BACK_BUTTON = 0x0020;
const DWORD GAMEPAD_LEFT_THUMB = 0x0040;
const DWORD GAMEPAD_RIGHT_THUMB = 0x0080;
const DWORD GAMEPAD_LEFT_SHOULDER = 0x0100;
const DWORD GAMEPAD_RIGHT_SHOULDER = 0x0200;
const DWORD GAMEPAD_A = 0x1000;
const DWORD GAMEPAD_B = 0x2000;
const DWORD GAMEPAD_X = 0x4000;
const DWORD GAMEPAD_Y = 0x8000;

struct ControllerState
{
	XINPUT_STATE        state;
	XINPUT_VIBRATION    vibration;
	float               vibrateTimeLeft;    // �~���b
	float               vibrateTimeRight;   // �~���b
	bool                connected;
};

class Input
{
private:
	// �w��̃L�[��������Ă����Ԃ̏ꍇ��true
	bool keysDown[inputNS::KEYS_ARRAY_LEN];
	// �w��̃L�[�������ꂽ�ꍇ��true
	bool keysPressed[inputNS::KEYS_ARRAY_LEN];
	std::string textIn;			// ���[�U�[�����͂����e�L�X�g
	char charIn;				// �Ō�ɓ��͂��ꂽ
	bool newLine;				// �V�����s�̊J�n����true
	int mouseX, mouseY;			// �}�E�X�̉�ʍ��W
	int mouseRawX, mouseRawY;	// �����׃}�E�X�̃f�[�^
	int  mouseWheel;                            // mouse wheel movement

	RAWINPUTDEVICE Rid[1];		// �����׃}�E�X�p
	bool mouseCaptured;			// �}�E�X���L���v�`������Ă���ꍇ��true
	bool mouseLButton;			// ���}�E�X�{�^����������Ă���ꍇ��true
	bool mouseRButton;			// �E�}�E�X�{�^����������Ă���ꍇ��true
	bool mouseMButton;			// �����}�E�X�{�^����������Ă���ꍇ��true
	bool mouseX1Button;			// X1�}�E�X�{�^����������Ă���ꍇ��true
	bool mouseX2Button;			// X2�}�E�X�{�^����������Ă���ꍇ��true
	ControllerState controllers[MAX_CONTROLLERS];	// �R���g���[���[�̏��
public:
	// �f�t�H���g�R���X�g���N�^
	Input();
	// �f�X�g���N�^
	virtual ~Input();

	// �}�E�X�ƃR���g���[���[�̓��͂�������
	// �}�E�X���L���v�`������ꍇ�Acapture=true��ݒ�
	// GameError���X���[
	void initialize(HWND hwnd, bool capture);

	// ���̃L�[�ɂ��āAkeysDown�z���keysPressed�z���true��ݒ�
	// ���s�O�FwParam�ɁA���z�L�[�R�[�h�i0~255�j���i�[����Ă���
	void keyDown(WPARAM wParam);
	
	// ���̃L�[�ɂ��āAkeysDown�z���false��ݒ�
	// ���s�O�FwParam�ɁA���z�L�[�R�[�h�i0~255�j���i�[����Ă���
	void keyUp(WPARAM wParam);
	
	// ���͂��ꂽ������textIn������ɕۑ�
	// ���s�O�FwParam�ɁA�������i�[����Ă���
	void keyIn(WPARAM wParam);
	
	// �w�肳�ꂽ���z�L�[��������Ă���ꍇ��true�A����ȊO�̏ꍇ��false��߂�
	bool isKeyDown(UCHAR vkey) const;
	
	// ���߂̃t���[���ɂ����āA�w��̉��z�L�[�������ꂽ���Ƃ�����ꍇ�Atrue��߂��܂��B
	// �L�[�̉��������̏�Ԃ́A�e�t���[���̏I�����ɏ�������܂��B
	bool wasKeyPressed(UCHAR vkey) const;
	
	// ���߂̃t���[���ɂ����āA���炩�̃L�[�������ꂽ�ꍇ�Atrue��߂��܂��B
	// �L�[�̉��������̏�Ԃ́A�e�t���[���̏I�����ɏ�������܂��B
	bool anyKeyPressed() const;
	
	// �w�肳�ꂽ�L�[�̉����������N���A
	void clearKeyPress(UCHAR vkey);
	
	// �w��̓��̓o�b�t�@���N���A
	// what�̒l�ɂ��Ă�input.h���Q��
	void clear(UCHAR what);

	// �L�[�A�}�E�X�A�e�L�X�g���̓f�[�^���N���A
	void clearAll() { clear(inputNS::KEYS_MOUSE_TEXT); }

	// �e�L�X�g���̓o�b�t�@���N���A
	void clearTextIn() { textIn.clear(); }

	// Clear last character entered
	void clearCharIn() { charIn = 0; }

	// �e�L�X�g���͂𕶎���Ƃ��Ė߂�
	std::string getTextIn() { return textIn; }

	// �Ō�ɓ��͂��ꂽ������߂�
	char getCharIn() { return charIn; }

	// �}�E�X�̗��ʈʒu��ǂݎ��AmouseX��mouseY�ɕۑ�
	void mouseIn(LPARAM lParam);
	
	// �}�E�X����̃��[�f�[�^��ǂݎ��AmouseRawX��mouseRawY�ɕۑ�
	// ���̃��[�`���́A�����׃}�E�X�ɑΉ����Ă��܂��B
	void mouseRawIn(LPARAM lParam);

	// Reads mouse wheel movement.
	void mouseWheelIn(WPARAM);
	
	// �}�E�X�{�^���̏�Ԃ�ۑ�
	void setMouseLButton(bool b) { mouseLButton = b; }
	
	// �}�E�X�{�^���̏�Ԃ�ۑ�
	void setMouseMButton(bool b) { mouseMButton = b; }
	
	// �}�E�X�{�^���̏�Ԃ�ۑ�
	void setMouseRButton(bool b) { mouseRButton = b; }
	
	// �}�E�X�{�^���̏�Ԃ�ۑ�
	void setMouseXButton(WPARAM wParam)
	{
		mouseX1Button = (wParam & MK_XBUTTON1) ? true : false;
		mouseX1Button = (wParam & MK_XBUTTON2) ? true : false;
	}
	
	// �}�E�X��X�ʒu��Ԃ�
	int getMouseX() const { return mouseX; }
	
	// �}�E�X��Y�ʒu��Ԃ�
	int getMouseY() const { return mouseY; }
	
	// �}�E�X��X�ʒu�̈ړ��̃��[�f�[�^��߂��B���ւ̈ړ���<0�A�E�ւ̈ړ���>0
	// �����׃}�E�X�ɑΉ�
	int getMouseRawX() const { return mouseRawX; }
	
	// �}�E�X��Y�ʒu�̈ړ��̃��[�f�[�^��߂��B��ւ̈ړ���<0�A���ւ̈ړ���>0
	// �����׃}�E�X�ɑΉ�
	int getMouseRawY() const { return mouseRawY; }

	// Return mouse wheel movement relative to previous position.
	// Forward is >0, Backward is <0, movement is in multiples of WHEEL_DATA (120).
	int  getMouseWheel()
	{
		int wheel = mouseWheel;
		mouseWheel = 0;
		return wheel;
	}
	
	// ���}�E�X�{�^���̏�Ԃ�߂�
	bool getMouseLButton() const { return mouseLButton; }
	
	// �����}�E�X�{�^���̏�Ԃ�߂�
	bool getMouseMButton() const { return mouseMButton; }
	
	// �E�}�E�X�{�^���̏�Ԃ�߂�
	bool getMouseRButton() const { return mouseRButton; }
	
	// X1�}�E�X�{�^���̏�Ԃ�߂�
	bool getMouseX1Button() const { return mouseX1Button; }
	
	// X2�}�E�X�{�^���̏�Ԃ�߂�
	bool getMouseX2Button() const { return mouseX2Button; }
	
	// Update connection status of game controllers.
	void checkControllers();

	// �ڑ�����Ă���R���g���[���[�̏�Ԃ�ǂݎ��
	void readControllers();

	// �w��̃Q�[���R���g���[���[�̏�Ԃ�߂�
	const ControllerState* getConrollerState(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return &controllers[n];
	}

	// �R���g���[��n�̃{�^���̏�Ԃ�߂�
	const WORD getGamepadButtons(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return controllers[n].state.Gamepad.wButtons;
	}

	// �R���g���[���[n�̕����p�b�h��̏�Ԃ�߂�
	bool getGamepadDPadUp(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return ((controllers[n].state.Gamepad.wButtons&GAMEPAD_DPAD_UP) != 0);
	}

	// �R���g���[���[n�̕����p�b�h���̏�Ԃ�߂�
	bool getGamepadDPadDown(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return ((controllers[n].state.Gamepad.wButtons&GAMEPAD_DPAD_DOWN) != 0);
	}

	// �R���g���[���[n�̕����p�b�h���̏�Ԃ�߂�
	bool getGamepadDPadLeft(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return ((controllers[n].state.Gamepad.wButtons&GAMEPAD_DPAD_LEFT) != 0);
	}

	// �R���g���[���[n�̕����p�b�h�E�̏�Ԃ�߂�
	bool getGamepadDPadRight(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((controllers[n].state.Gamepad.wButtons&GAMEPAD_DPAD_RIGHT) != 0);
	}

	// �R���g���[���[n��START�{�^���̏�Ԃ�߂�
	bool getGamepadStart(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((controllers[n].state.Gamepad.wButtons&GAMEPAD_START_BUTTON) != 0);
	}

	// �R���g���[���[n��BACK�{�^���̏�Ԃ�߂�
	bool getGamepadBack(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((controllers[n].state.Gamepad.wButtons&GAMEPAD_BACK_BUTTON) != 0);
	}

	// �R���g���[���[n�̍��T���{�^���̏�Ԃ�߂�
	bool getGamepadLeftThumb(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((controllers[n].state.Gamepad.wButtons&GAMEPAD_LEFT_THUMB) != 0);
	}

	// �R���g���[���[n�̉E�T���{�^���̏�Ԃ�߂�
	bool getGamepadRightThumb(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((controllers[n].state.Gamepad.wButtons&GAMEPAD_RIGHT_THUMB) != 0);
	}

	// �R���g���[���[n�̍��V�����_�[�{�^���̏�Ԃ�߂�
	bool getGamepadLeftShoulder(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((controllers[n].state.Gamepad.wButtons&GAMEPAD_LEFT_SHOULDER) != 0);
	}

	// �R���g���[���[n�̉E�V�����_�[�{�^���̏�Ԃ�߂�
	bool getGamepadRightShoulder(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((controllers[n].state.Gamepad.wButtons&GAMEPAD_RIGHT_SHOULDER) != 0);
	}

	// �R���g���[���[n��A�{�^���̏�Ԃ�߂�
	bool getGamepadA(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((controllers[n].state.Gamepad.wButtons&GAMEPAD_A) != 0);
	}

	// �R���g���[���[n��B�{�^���̏�Ԃ�߂�
	bool getGamepadB(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((controllers[n].state.Gamepad.wButtons&GAMEPAD_B) != 0);
	}

	// �R���g���[���[n��X�{�^���̏�Ԃ�߂�
	bool getGamepadX(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((controllers[n].state.Gamepad.wButtons&GAMEPAD_X) != 0);
	}

	// �R���g���[���[n��Y�{�^���̏�Ԃ�߂�
	bool getGamepadY(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((controllers[n].state.Gamepad.wButtons&GAMEPAD_Y) != 0);
	}

	// �R���g���[���[n�̍��g���K�[�̒l��߂��܂��B
	BYTE getGamepadLeftTrigger(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return (controllers[n].state.Gamepad.bLeftTrigger);
	}

	// �R���g���[���[n�̉E�g���K�[�̒l��߂��܂��B
	BYTE getGamepadRightTrigger(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return (controllers[n].state.Gamepad.bRightTrigger);
	}

	// �R���g���[���[n�̍��T���X�e�B�b�N�AX�̒l��߂��܂��B
	SHORT getGamepadThumbLX(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return (controllers[n].state.Gamepad.sThumbLX);
	}

	// �R���g���[���[n�̍��T���X�e�B�b�N�AY�̒l��߂��܂��B
	SHORT getGamepadThumbLY(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return (controllers[n].state.Gamepad.sThumbLY);
	}

	// �R���g���[���[n�̉E�T���X�e�B�b�N�AX�̒l��߂��܂��B
	SHORT getGamepadThumbRX(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return (controllers[n].state.Gamepad.sThumbRX);
	}

	// �R���g���[���[n�̉E�T���X�e�B�b�N�AY�̒l��߂��܂��B
	SHORT getGamepadThumbRY(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return (controllers[n].state.Gamepad.sThumbRY);
	}

	// �R���g���[���[n�̍����[�^��U��������
	// ���͒���g�U��
	// speed�F0= �I�t�A65536=100�p�[�Z���g
	// sec�F�U�������鎞�ԁi�b�j
	void gamePadVibrateLeft(UINT n, WORD speed, float sec)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		controllers[n].vibration.wLeftMotorSpeed = speed;
		controllers[n].vibrateTimeLeft = sec;
	}

	// �R���g���[���[n�̉E���[�^��U��������
	// �E�͍����g�U��
	// speed�F0= �I�t�A65536=100�p�[�Z���g
	// sec�F�U�������鎞�ԁi�b�j
	void gamePadVibrateRight(UINT n, WORD speed, float sec)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		controllers[n].vibration.wRightMotorSpeed = speed;
		controllers[n].vibrateTimeRight = sec;
	}

	// �ڑ�����Ă���R���g���[���[��U��������
	void vibrateControllers(float frameTime);
};

#endif