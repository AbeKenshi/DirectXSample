#define WIN32_LEAN_AND_MEAN
#include "windows.h"

// �֐��v���g�^�C�v
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
bool CreateMainWindow(HINSTANCE, int);
LRESULT WINAPI WinProc(HWND, UINT, WPARAM, LPARAM);
bool AnotherInstance();

// �O���[�o���ϐ�
HINSTANCE hinst;
HDC hdc;			// �f�o�C�X�R���e�L�X�g�ւ̃n���h��
TCHAR ch = ' ';		// ���͂��ꂽ����	
RECT rect;			// Rectangle�\����
PAINTSTRUCT ps;		// WM_PAINT�Ŏg�p�����
bool vkKeys[256];	// ���z�L�[�̏�ԁAtrue��false

// �萔
const char CLASS_NAME[] = "WinMain";
const char APP_TITLE[] = "Hello World"; // �^�C�g���o�[�̃e�L�X�g
const int WINDOW_WIDTH = 400; // �E�B���h�E�̕�
const int WINDOW_HEIGHT = 400; // �E�B���h�E�̍���

// Windows�A�v���P�[�V�����̊J�n�_
// �p�����[�^
//		hInstance�F�A�v���P�[�V�����̌��݂̃C���X�^���X�ւ̃n���h��
//		hPrevInstance�F���NULL�i�p�~���ꂽ�p�����[�^�j
//		lpCmdLine�F�R�}���h���C��������NULL�I�[�����ւ̃|�C���^
//		nCmdShow�F�E�B���h�E���ǂ̂悤�ɕ\�����邩���w��
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;

	// �����̃C���X�^���X�̐�����h��
	if (AnotherInstance())
		return false;

	// �E�B���h�E���쐬
	if (!CreateMainWindow(hInstance, nCmdShow))
		return false;

	for (int i = 0; i < 256; ++i)	// ���z�L�[�̔z��̏�����
	{
		vkKeys[i] = false;
	}

	// ���C���̃��b�Z�[�W���[�v
	int done = 0;
	while (!done)
	{
		// PeekMessage��Windows���b�Z�[�W�����邩���e�X�g�����u���b�N���\�b�h
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// �I�����b�Z�[�W�����m
			if (msg.message == WM_QUIT)
				done = 1;
			// ���b�Z�[�W���f�R�[�h����WinProc�ɓn��
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return msg.wParam;
}

// �E�B���h�E�C�x���g�R�[���o�b�N�֐�
LRESULT WINAPI WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	short nVirtKey;								// ���z�L�[�R�[�h
	const short SHIFTED = (short)0x8000;
	TEXTMETRIC tm;
	DWORD chWidth = 20;							// �����̕�
	DWORD chHeight = 20;						// �����̍���

	switch (msg)
	{
	case WM_CREATE:
		// text metrics���擾
		hdc = GetDC(hWnd);
		GetTextMetrics(hdc, &tm);
		ReleaseDC(hWnd, hdc);
		chWidth = tm.tmAveCharWidth;
		chHeight = tm.tmHeight;
		return 0;

	case WM_DESTROY:
		// Windows�Ƀv���O�������I������悤�ɓ`����
		PostQuitMessage(0);
		return 0;

	case WM_CHAR:								// �L�[�{�[�h���當�������͂��ꂽ�ꍇ
		switch (wParam)							// ������wParam�Ɋi�[����Ă���
		{
		case 0x08:								// �o�b�N�X�y�[�X
		case 0x09:								// �^�u
		case 0x0A:								// ���C���t�B�[�h
		case 0x0D:								// �L�����b�W���^�[��
		case 0x1B:								// �G�X�P�[�v
			return 0;
		default:								// �\���\�ȕ���
			ch = (TCHAR)wParam;					// �������擾
			InvalidateRect(hWnd, NULL, TRUE);	// WM_PAINT�������I�ɔ���������
			return 0;
		}

	case WM_KEYDOWN:							// �L�[�������ꂽ�ꍇ
		vkKeys[wParam] = true;
		switch (wParam)
		{
			case VK_SHIFT:							// Shift�L�[
				nVirtKey = GetKeyState(VK_LSHIFT);	// ��Shift�L�[�̏�Ԃ��擾
				if (nVirtKey & SHIFTED)				// ��Shift�L�[�̏ꍇ
					vkKeys[VK_LSHIFT] = true;
				nVirtKey = GetKeyState(VK_RSHIFT);	// �EShift�L�[�̏�Ԃ��擾
				if (nVirtKey & SHIFTED)				// �EShift�L�[�̏ꍇ
					vkKeys[VK_RSHIFT] = true;
				break;
			case VK_CONTROL:						// Ctrl�L�[
				nVirtKey = GetKeyState(VK_LCONTROL);
				if (nVirtKey & SHIFTED)			// ��Ctrl�L�[�̏ꍇ
					vkKeys[VK_LCONTROL] = true;
				nVirtKey = GetKeyState(VK_RCONTROL);
				if (nVirtKey & SHIFTED)			// ��Ctrl�L�[�̏ꍇ
					vkKeys[VK_RCONTROL] = true;
				break;
		}
		InvalidateRect(hWnd, NULL, TRUE);			// WM_PAINT�������I�ɔ���������
		return 0;
		break;

	case WM_KEYUP:									// �L�[�������ꂽ�ꍇ
		vkKeys[wParam] = false;
		switch (wParam)
		{
		case VK_SHIFT:							// Shift�L�[
			nVirtKey = GetKeyState(VK_LSHIFT);	// ��Shift�L�[�̏�Ԃ��擾
			if ((nVirtKey & SHIFTED) == 0)				// ��Shift�L�[�̏ꍇ
				vkKeys[VK_LSHIFT] = false;
			nVirtKey = GetKeyState(VK_RSHIFT);	// �EShift�L�[�̏�Ԃ��擾
			if ((nVirtKey & SHIFTED) == 0)				// �EShift�L�[�̏ꍇ
				vkKeys[VK_RSHIFT] = false;
			break;
		case VK_CONTROL:						// Ctrl�L�[
			nVirtKey = GetKeyState(VK_LCONTROL);
			if ((nVirtKey & SHIFTED) == 0)			// ��Ctrl�L�[�̏ꍇ
				vkKeys[VK_LCONTROL] = false;
			nVirtKey = GetKeyState(VK_RCONTROL);
			if ((nVirtKey & SHIFTED) == 0)			// ��Ctrl�L�[�̏ꍇ
				vkKeys[VK_RCONTROL] = false;
			break;
		}
		InvalidateRect(hWnd, NULL, TRUE);			// WM_PAINT�������I�ɔ���������
		return 0;
		break;

	case WM_PAINT:								// �E�B���h�E���ĕ`�悷��K�v������ꍇ
		hdc = BeginPaint(hWnd, &ps);			// �f�o�C�X�R���e�L�X�g�ւ̃n���h�����擾
		TextOut(hdc, 0, 0, &ch, 1);				// ������\��

		// vkKeys�z��̏�Ԃ�\��
		// �L�[��������Ă����T��\���A������Ă��Ȃ����F��\��
		for (int r = 0; r < 16; ++r)
		{
			for (int c = 0; c < 16; ++c)
			{
				if (vkKeys[r * 16 + c])
				{
					SetBkMode(hdc, OPAQUE);		// �����̔w�i��s�����ɂ���
					TextOut(hdc, c * chWidth + chWidth * 2, r * chHeight + chHeight * 2, "T ", 2);
				} else {
					SetBkMode(hdc, TRANSPARENT);// �����̔w�i�𓧖��ɂ���
					TextOut(hdc, c * chWidth + chWidth * 2, r * chHeight + chHeight * 2, "F ", 2);
				}
			}
		}

		EndPaint(hWnd, &ps);
		return 0;

	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}

// �E�B���h�E���쐬
// �߂�l�F�G���[�̏ꍇ�Afalse
bool CreateMainWindow(HINSTANCE hInstance, int nCmdShow)
{
	WNDCLASSEX wcx;
	HWND hwnd;

	// �E�B���h�E�N���X�̍\���̂����C���E�B���h�E���L�q����p�����[�^�Őݒ肵�܂��B
	wcx.cbSize = sizeof(wcx);					// �\���̂̃T�C�Y
	wcx.style = CS_HREDRAW | CS_VREDRAW;		// �E�B���h�E�T�C�Y�ύX���ɍĕ`��
	wcx.lpfnWndProc = WinProc;					// �E�B���h�E�v���V�[�W�����w��
	wcx.cbClsExtra = 0;							// �g���N���X�������Ȃ�
	wcx.cbWndExtra = 0;							// �g���E�B���h�E�������Ȃ�
	wcx.hInstance = hInstance;					// �C���X�^���X�ւ̃n���h��
	wcx.hIcon = NULL;
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);	// ���O��`����Ă�����J�[�\��
	wcx.hbrBackground =							// �w�i�u���V
		(HBRUSH)GetStockObject(GRAY_BRUSH);
	wcx.lpszMenuName = NULL;					// ���j���[���\�[�X�̖��O
	wcx.lpszClassName = CLASS_NAME;				// �E�B���h�E�N���X�̖��O
	wcx.hIconSm = NULL;
	// �E�B���h�E�N���X��o�^
	// �G���[���ARegisterClassEx��0��߂�
	if (RegisterClassEx(&wcx) == 0)				// �G���[�̏ꍇ
		return false;
	// �E�B���h�E���쐬
	hwnd = CreateWindow(
		CLASS_NAME,				// �E�B���h�E�N���X�̖��O
		APP_TITLE,				// �^�C�g���o�[�̃e�L�X�g
		WS_OVERLAPPEDWINDOW,	// �E�B���h�E�̃X�^�C��
		CW_USEDEFAULT,			// �E�B���h�E�̐����ʒu�̃f�t�H���g
		CW_USEDEFAULT,			// �E�B���h�E�̐����ʒu�̃f�t�H���g
		WINDOW_WIDTH,			// �E�B���h�E�̕�
		WINDOW_HEIGHT,			// �E�B���h�E�̍���
		(HWND)NULL,				// �e�E�B���h�E�Ȃ�
		(HMENU)NULL,			// ���j���[�Ȃ�
		hInstance,				// �A�v���P�[�V�����C���X�^���X�ւ̃n���h��
		(LPVOID)NULL);			// �E�B���h�E�p�����[�^�Ȃ�
	// �E�B���h�E�̍쐬�ŃG���[�����������ꍇ
	if (!hwnd)
		return false;
	// �E�B���h�E��\��
	ShowWindow(hwnd, nCmdShow);
	// �E�B���h�E�v���V�[�W����WM_PAINT���b�Z�[�W�𑗂�
	UpdateWindow(hwnd);
	return true;
}

// ���݂̃A�v���P�[�V�����̕ʂ̃C���X�^���X���Ȃ������`�F�b�N
// �߂�l�F�ʂ̃C���X�^���X�����������ꍇ�Atrue
//		�@ ���g���B��̃C���X�^���X�ł���ꍇ�Afalse
bool AnotherInstance()
{
	HANDLE ourMutex;

	// �ŗL�̕�������g���ă~���[�e�b�N�X�̍쐬�����݂�
	ourMutex = CreateMutex(NULL, true,
		"MUTEX");
	if (GetLastError() == ERROR_ALREADY_EXISTS)
		return true;	// �ʂ̃C���X�^���X�����������ꍇ
	return false;		// ���g���B��̃C���X�^���X�ł���ꍇ
}