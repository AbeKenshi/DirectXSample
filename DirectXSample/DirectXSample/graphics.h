#ifndef _GRAPHICS_H             // ���̃t�@�C���������̉ӏ��ŃC���N���[�h�����ꍇ�ɁA
#define _GRAPHICS_H             // ���d�ɒ�`����邱�Ƃ�h���܂��B
#define WIN32_LEAN_AND_MEAN

#ifdef _DEBUG
#define D3D_DEBUG_INFO
#endif
#include <d3d9.h>
#include "constants.h"
#include "gameError.h"

// DirectX�|�C���^�^
#define LP_3DDEVICE LPDIRECT3DDEVICE9
#define LP_3D       LPDIRECT3D9

// �F�̒�`
#define COLOR_ARGB DWORD
#define SETCOLOR_ARGB(a,r,g,b) \
    ((COLOR_ARGB)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))

class Graphics
{
private:
	// DirectX�|�C���^�Ȃ�
	LP_3D       direct3d;
	LP_3DDEVICE device3d;
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE pMode;

	// ���̑��̕ϐ�
	HRESULT     result;         // �W����Windows���^�[���R�[�h
	HWND        hwnd;
	bool        fullscreen;
	int         width;
	int         height;
	// �i�G���W���̎g�p���ɓ����I�ɂ̂ݎg�p���܂��B
	// �����ɂ̓��[�U�[���g�p������̂͂���܂���B�j

	// D3D�v���[���e�[�V�����p�����[�^��������
	void    initD3Dpp();

public:
	// �R���X�g���N�^
	Graphics();

	// �f�X�g���N�^
	virtual ~Graphics();

	// direct3d��device3d�����
	void    releaseAll();

	// DirectX�O���t�B�b�N�X��������
	// hw = �E�B���h�E�ւ̃n���h��
	// width = ���i�s�N�Z���P�ʁj
	// height = �����i�s�N�Z���P�ʁj
	// fullscreen = �S��ʕ\���̏ꍇ��true�A�E�B���h�E�̏ꍇ��false
	// �G���[����GameError���X���[
	void    initialize(HWND hw, int width, int height, bool fullscreen);

	// �I�t�X�N���[���o�b�N�o�b�t�@����ʂɕ\��
	HRESULT showBackbuffer();

	// �A�_�v�^�[���`�F�b�N���āAd3dpp�Ŏw�肳�ꂽBackBuffer�̍����A���A
	// ���t���b�V�����[�g�ɑΉ����Ă��邩�ǂ������m�F���܂��B
	// �Ή�������̂����������ꍇ�́ApMode�\���̂ɁA
	// �Ή����Ă��郂�[�h�̃t�H�[�}�b�g��ݒ肵�܂��B
	// ���s�O�Fd3dpp������������Ă���
	// ���s��F�Ή����Ă��郂�[�h��������ApMode�\���̂Ƀf�[�^��
	// �ݒ肳�ꂽ�ꍇ�Atrue��߂��܂��B
	// �Ή����Ă��郂�[�h��������Ȃ������ꍇ�Afalse��߂��܂��B
	bool isAdapterCompatible();
};

#endif

