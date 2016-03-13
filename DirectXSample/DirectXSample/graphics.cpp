#include "graphics.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
Graphics::Graphics()
{
	direct3d = NULL;
	device3d = NULL;
	fullscreen = false;
	width = GAME_WIDTH;    // ���ƍ�����initialize()�Œu����������
	height = GAME_HEIGHT;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
Graphics::~Graphics()
{
	releaseAll();
}

//=============================================================================
// ���ׂĂ��J��
//=============================================================================
void Graphics::releaseAll()
{
	SAFE_RELEASE(device3d);
	SAFE_RELEASE(direct3d);
}

//=============================================================================
// DirectX�O���t�B�b�N�X��������
// �G���[����GameError���X���[
//=============================================================================
void Graphics::initialize(HWND hw, int w, int h, bool full)
{
	hwnd = hw;
	width = w;
	height = h;
	fullscreen = full;

	//Direct3D��������
	direct3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (direct3d == NULL)
		throw(GameError(gameErrorNS::FATAL_ERROR, 
			"Error initializing Direct3D"));

	initD3Dpp();        // D3D�v���[���e�[�V�����p�����[�^��������
	if (fullscreen)
	{
		if (isAdapterCompatible())
			// �Ή����Ă��郊�t���b�V�����[�g��ݒ�
			d3dpp.FullScreen_RefreshRateInHz = pMode.RefreshRate;
		else
			throw(GameError(gameErrorNS::FATAL_ERROR,
				"The graphics device does not support the " \
				"specified resolution and/or format."));
	}
	
	// �O���t�B�b�N�X�J�[�h���n�[�h�E�F�A�e�N�X�`�������O�ƃ��C�e�B���O�A
	// ���_�V�F�[�_���T�|�[�g���Ă��邩�ǂ����𔻒�
	D3DCAPS9 caps;
	DWORD behavior;
	result = direct3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);
	// �f�o�C�X��HW�e�N�X�`�������O�ƃ��C�e�B���O���T�|�[�g���Ă��Ȃ��ꍇ�A
	// �܂���1.1���_�V�F�[�_�[���n�[�h�E�F�A�ŃT�|�[�g���Ă��Ȃ��ꍇ�A
	// �\�t�g�E�F�A���_�����ɐ؂�ւ��܂��B
	if ((caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 ||
		caps.VertexShaderVersion < D3DVS_VERSION(1, 1))
		// �\�t�g�E�F�A�݂̂̏������g�p
		behavior = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	else
		// �n�[�h�E�F�A�݂̂̏������g�p
		behavior = D3DCREATE_HARDWARE_VERTEXPROCESSING;
														 
	// Direct3D�f�o�C�X���쐬
	result = direct3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hwnd,
		// �ꕔ�̃V�X�e���ł́ADirect3D9 (ERROR) ASSERTION FAILED!
		// ���񍐂���܂����A���ۂɂ̓G���[�ł͂���܂���B
		behavior,
		// �O���t�B�b�N�X�J�[�h���n�[�h�E�F�A���_�������T�|�[�g���Ȃ��ꍇ�͋@�\���Ȃ�
		&d3dpp,
		&device3d);

	if (FAILED(result))
		throw(GameError(gameErrorNS::FATAL_ERROR,
			"Error creating Direct3D device"));

}

//=============================================================================
// D3D�v���[���e�[�V�����p�����[�^��������
//=============================================================================
void Graphics::initD3Dpp()
{
	try {
		ZeroMemory(&d3dpp, sizeof(d3dpp));              // �\���̂�0��ݒ�
		// �K�v�ȃp�����[�^��ݒ�
		d3dpp.BackBufferWidth = width;
		d3dpp.BackBufferHeight = height;
		if (fullscreen)                                // �S��ʕ\���̏ꍇ
			d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;  // 24�r�b�g�J���[
		else
			d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;   // �f�X�N�g�b�v�ݒ���g�p
		d3dpp.BackBufferCount = 1;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.hDeviceWindow = hwnd;
		d3dpp.Windowed = (!fullscreen);
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	}
	catch (...)
	{
		throw(GameError(gameErrorNS::FATAL_ERROR,
			"Error initializing D3D presentation parameters"));

	}
}

//=============================================================================
// �o�b�N�o�b�t�@��\��
//=============================================================================
HRESULT Graphics::showBackbuffer()
{
	result = E_FAIL;    // ���s���f�t�H���g�Ƃ��A�������ɒu������
	// �i���̊֐��́A��̃o�[�W�����ł͈ړ����܂��j
	// �o�b�N�o�b�t�@�����C���O���[���ɃN���A
	device3d->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 255, 0), 0.0f, 0);

	// �o�b�N�o�b�t�@����ʂɕ\��
	result = device3d->Present(NULL, NULL, NULL, NULL);

	return result;
}

//=============================================================================
// �A�_�v�^�[���`�F�b�N���āAd3dpp�Ŏw�肳�ꂽBackBuffer�̍����A���A
// ���t���b�V�����[�g�ɑΉ����Ă��邩�ǂ������m�F���܂��B
// �Ή�������̂����������ꍇ�́ApMode�\���̂ɁA
// �Ή����Ă��郂�[�h�̃t�H�[�}�b�g��ݒ肵�܂��B
// ���s�O�Fd3dpp������������Ă���
// ���s��F�Ή����Ă��郂�[�h��������ApMode�\���̂Ƀf�[�^��
// �ݒ肳�ꂽ�ꍇ�Atrue��߂��܂��B
// �Ή����Ă��郂�[�h��������Ȃ������ꍇ�Afalse��߂��܂��B
//=============================================================================
bool Graphics::isAdapterCompatible()
{
	UINT modes = direct3d->GetAdapterModeCount(D3DADAPTER_DEFAULT,
		d3dpp.BackBufferFormat);
	for (UINT i = 0; i<modes; i++)
	{
		result = direct3d->EnumAdapterModes(D3DADAPTER_DEFAULT,
			d3dpp.BackBufferFormat,
			i, &pMode);
		if (pMode.Height == d3dpp.BackBufferHeight &&
			pMode.Width == d3dpp.BackBufferWidth &&
			pMode.RefreshRate >= d3dpp.FullScreen_RefreshRateInHz)
			return true;
	}
	return false;
}