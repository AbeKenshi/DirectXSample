#define WIN32_LEAN_AND_MEAN
#include "windows.h"
// �֐��v���g�^�C�v
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
bool CreateMainWindow(HINSTANCE, int);
LRESULT WINAPI WinProc(HWND, UINT, WPARAM, LPARAM);
// �O���[�o���ϐ�
HINSTANCE hinst;
HDC hdc;			// �f�o�C�X�R���e�L�X�g�ւ̃n���h��
TCHAR ch = ' ';		// ���͂��ꂽ����	
RECT rect;			// Rectangle�\����
PAINTSTRUCT ps;		// WM_PAINT�Ŏg�p�����
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
	// �E�B���h�E���쐬
	if (!CreateMainWindow(hInstance, nCmdShow))
		return false;
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
	switch (msg)
	{
	case WM_CHAR:								// �L�[�{�[�h���當�������͂��ꂽ�ꍇ
		switch (wParam)							// ������wParam�Ɋi�[����Ă���
		{
		case 0x08:								// �o�b�N�X�y�[�X
		case 0x09:								// �^�u
		case 0x0A:								// ���C���t�B�[�h
		case 0x0D:								// �L�����b�W���^�[��
		case 0x1B:								// �G�X�P�[�v
			MessageBeep((UINT)-1);				// �r�[�v����炷�A�\���͂��Ȃ�
			return 0;
		default:								// �\���\�ȕ���
			ch = (TCHAR)wParam;					// �������擾
			InvalidateRect(hWnd, NULL, TRUE);	// WM_PAINT�������I�ɔ���������
			return 0;
		}
	case WM_PAINT:								// �E�B���h�E���ĕ`�悷��K�v������ꍇ
		hdc = BeginPaint(hWnd, &ps);			// �f�o�C�X�R���e�L�X�g�ւ̃n���h�����擾
		GetClientRect(hWnd, &rect);				// �E�B���h�E�̋�`���擾
		// ������\��
		TextOut(hdc, rect.right / 2, rect.bottom / 2, &ch, 1);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		// Windows�ɂ��̃v���O�������I������悤�ɓ`����
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
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
		(HBRUSH)GetStockObject(BLACK_BRUSH);
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