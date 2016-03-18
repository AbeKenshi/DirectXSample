#include "game.h"
//=============================================================================
// �R���X�g���N�^
//=============================================================================
Game::Game()
{
	input = new Input();	// �L�[�{�[�h���͂𑦎��ɏ�����
	// ���̑��̏������́A���
	// input->initialize()���Ăяo���ď���
	paused = false;			// �Q�[���͈ꎞ��~���łȂ�
	graphics = NULL;
	audio = NULL;
	console = NULL;
	fps = 100;
	fpsOn = false;			// �f�t�H���g�ł̓t���[�����[�g��\�����Ȃ�
	initialized = false;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
Game::~Game()
{
	deleteAll();		// �\�񂳂�Ă��������������ׂĉ��
	ShowCursor(true);	// �J�[�\����\��
}

//=============================================================================
// Windows���b�Z�[�W�n���h��
//=============================================================================
LRESULT Game::messageHandler(
	HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (initialized)	// ����������Ă��Ȃ��ꍇ�̓��b�Z�[�W���������Ȃ�
	{
		switch (msg)
		{
		case WM_DESTROY:
			// windows�ɂ��̃v���O�������I������悤�ɓ`����
			PostQuitMessage(0);
			return 0;
		case WM_KEYDOWN: case WM_SYSKEYDOWN:	// �L�[�������ꂽ
			input->keyDown(wParam);
			return 0;
		case WM_KEYUP: case WM_SYSKEYUP:		// �L�[�������ꂽ
			input->keyUp(wParam);
			return 0;
		case WM_CHAR:							// ���������͂��ꂽ
			input->keyIn(wParam);
			return 0;
		case WM_MOUSEMOVE:						// �}�E�X���ړ����ꂽ
			input->mouseIn(lParam);
			return 0;
		case WM_INPUT:							// �}�E�X����̃��[�f�[�^����
			input->mouseRawIn(lParam);
			return 0;
		case WM_LBUTTONDOWN:					// ���}�E�X�{�^���������ꂽ
			input->setMouseLButton(true);
			input->mouseIn(lParam);
			return 0;
		case WM_LBUTTONUP:						// ���}�E�X�{�^���������ꂽ
			input->setMouseLButton(false);
			input->mouseIn(lParam);
			return 0;
		case WM_MBUTTONDOWN:					// �����}�E�X�{�^���������ꂽ
			input->setMouseMButton(true);
			input->mouseIn(lParam);
			return 0;
		case WM_MBUTTONUP:						// �����}�E�X�{�^���������ꂽ
			input->setMouseMButton(false);
			input->mouseIn(lParam);
			return 0;
		case WM_RBUTTONDOWN:					// �E�}�E�X�{�^���������ꂽ
			input->setMouseRButton(true);
			input->mouseIn(lParam);
			return 0;
		case WM_RBUTTONUP:						// �E�}�E�X�{�^���������ꂽ
			input->setMouseRButton(false);
			input->mouseIn(lParam);
			return 0;
		// �}�E�X��X�{�^���������ꂽ/�����ꂽ
		case WM_XBUTTONDOWN: case WM_XBUTTONUP:
			input->setMouseXButton(wParam);
			input->mouseIn(lParam);
			return 0;
		case WM_DEVICECHANGE:					// �R���g���[���[���`�F�b�N
			input->checkControllers();
			return 0;
		}
	}
	// windows�ɏ�����C����
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

//=============================================================================
// �Q�[����������
// �G���[����GameError���X���[
//=============================================================================
void Game::initialize(HWND hw)
{
	hwnd = hw;	// �E�B���h�E�n���h����ۑ�
	// �O���t�B�b�N�X��������
	graphics = new Graphics();
	// GameError���X���[
	graphics->initialize(hwnd, GAME_WIDTH, GAME_HEIGHT, FULLSCREEN);
	// ���͂��������A�}�E�X�̓L���v�`�����Ȃ�
	input->initialize(hwnd, false);			// GameError���X���[
	// �T�E���h�V�X�e����������
	audio = new Audio();
	// �R���\�[����������
	console = new Console();
	console->initialize(graphics, input);	// �R���\�[��������
	console->print("---Console---");

	// messageDialog��������
	messageDialog = new MessageDialog();
	messageDialog->initialize(graphics, input, hwnd);

	// DirectX�t�H���g��������
	if (dxFont.initialize(graphics, gameNS::POINT_SIZE, false, false, gameNS::FONT) == false)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize DirectX font."));

	dxFont.setFontColor(gameNS::FONT_COLOR);

	// �T�E���h�t�@�C������`����Ă���ꍇ
	if (*WAVE_BANK != '\0' && *SOUND_BANK != '\0')
	{
		if (FAILED(hr = audio->initialize()))
		{
			if (hr == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND))
				throw(GameError(gameErrorNS::FATAL_ERROR,
					"Failed to initialize sound system" \
					"because media file not found."));
			else
				throw(GameError(gameErrorNS::FATAL_ERROR,
					"Failed to initialize sound system."));
		}
	}
	// ������\�^�C�}�[�̏��������݂�
	if (QueryPerformanceFrequency(&timerFreq) == false)
		throw(GameError(gameErrorNS::FATAL_ERROR,
			"Error initializing high resolution timer"));
	QueryPerformanceCounter(&timeStart);	// �J�n���Ԃ��擾
	initialized = true;
}

//=============================================================================
// �Q�[���A�C�e���������_�[
//=============================================================================
void Game::renderGame()
{
	const int BUF_SIZE = 20;
	static char buffer[BUF_SIZE];

	// �����_�����O���J�n
	if (SUCCEEDED(graphics->beginScene()))
	{
		// render�́A�p�������N���X���ŋL�q����K�v�̂��鏃�����z�֐��ł��B
		render();					// �h���N���X��render���Ăяo��
		graphics->spriteBegin();	// �X�v���C�g�̕`����J�n
		if (fpsOn)					// �t���[�����[�g�̕\�����v������Ă���ꍇ
		{
			// fps��Cstring�ɕϊ�
			_snprintf_s(buffer, BUF_SIZE, "fps %d", (int)fps);
			dxFont.print(buffer, GAME_WIDTH - 100, GAME_HEIGHT - 28);
		}
		graphics->spriteEnd();		// �X�v���C�g�̕`����I��
		console->draw();			// �R���\�[���́A�Q�[���̑O�ʂɕ\�������悤�ɂ����ŕ`��
		messageDialog->draw();		// �_�C�A���O��O�ʂɕ`��
		// �����_�����O���I��
		graphics->endScene();
	}
	handleLostGraphicsDevice();
	// �o�b�N�o�b�t�@����ʂɕ\��
	graphics->showBackbuffer();
}

//=============================================================================
// ���������O���t�B�b�N�X�f�o�C�X������
//=============================================================================
void Game::handleLostGraphicsDevice()
{
	// �f�o�C�X�̏������e�X�g���A����ɉ����ď��������s
	hr = graphics->getDeviceState();
	if (FAILED(hr))					// �O���t�B�b�N�X�f�o�C�X���L���ȏ�ԂłȂ��ꍇ
	{
		// �f�o�C�X���������Ă���A���Z�b�g�ł����ԂɂȂ��ꍇ
		if (hr == D3DERR_DEVICELOST)
		{
			Sleep(100);				// CPU���Ԃ𖾂��n���i100�~���b�j
			return;
		}
		// �f�o�C�X���������Ă��邪�A���Z�b�g�ł����Ԃɂ���ꍇ
		else if (hr == D3DERR_DEVICENOTRESET)
		{
			releaseAll();
			hr = graphics->reset();	// �O���t�B�b�N�X�f�o�C�X�̃��Z�b�g�����݂�
			if (FAILED(hr))
				return;
			resetAll();
		}
		else
			return;					// ���̃f�o�C�X�G���[
	}
}

//=============================================================================
// Toggle window or fullscreen mode
//=============================================================================
void Game::setDisplayMode(graphicsNS::DISPLAY_MODE mode)
{
	releaseAll();                   // free all user created surfaces
	graphics->changeDisplayMode(mode);
	resetAll();                     // recreate surfaces
}

//=============================================================================
// WinMain���̃��C���̃��b�Z�[�W���[�v�ŌJ��Ԃ��Ăяo�����
//=============================================================================
void Game::run(HWND hwnd)
{
	if (graphics == NULL)	// �O���t�B�b�N�X������������Ă��Ȃ��ꍇ
		return;
	// �Ō�̃t���[������̌o�ߎ��Ԃ��v�Z�AframeTime�ɕۑ�
	QueryPerformanceCounter(&timeEnd);
	frameTime = (float)(timeEnd.QuadPart - timeStart.QuadPart) /
		(float)timerFreq.QuadPart;
	// �ȓd�̓R�[�h�iwinmm.lib���K�v�j
	// ��]����t���[�����[�g�ɑ΂��Čo�ߎ��Ԃ��Z���ꍇ
	if (frameTime < MIN_FRAME_TIME)
	{
		sleepTime = (DWORD)((MIN_FRAME_TIME - frameTime) * 1000);
		timeBeginPeriod(1);	// 1�~���b�̕���\��windows�^�C�}�[�ɗv��
		Sleep(sleepTime);	// sleepTime�̊ԁACPU�����
		timeEndPeriod(1);	// 1�~���b�̃^�C�}�[����\���I��
		return;
	}
	if (frameTime > 0.0)
		fps = (fps * 0.99f) + (0.01f / frameTime);	// ����fps
	if (frameTime > MAX_FRAME_TIME)					// �t���[�����[�g�����ɒx���ꍇ
		frameTime = MAX_FRAME_TIME;					// �ő�frameTime�𐧌�
	timeStart = timeEnd;

	// update()�Aai()�Acollisions()�͏������z�֐��ł��B
	// �����̊֐��́AGame���p�����Ă���N���X���ŋL�q����K�v������܂��B
	if (!paused)									// �ꎞ��~���łȂ��ꍇ
	{
		update();									// ���ׂẴQ�[���A�C�e�����X�V
		ai();										// �l�H�m�\
		collisions();								// �Փ˂𔻒�
		input->vibrateControllers(frameTime);		// �R���g���[���[�̐U��������
	}
	renderGame();									// ���ׂẴQ�[���A�C�e����`��
	
	// �R���\�[���L�[���`�F�b�N
	if (input->wasKeyPressed(CONSOLE_KEY))
	{
		console->showHide();
		// �R���\�[�����\������Ă���ԁA�Q�[�����ꎞ��~
		paused = console->getVisible();
	}
	consoleCommand();				// ���[�U�[�����͂����R���\�[���R�}���h���擾

	input->readControllers();		// �R���g���[���[�̏�Ԃ�ǂݎ��
	messageDialog->update();		// �{�^���N���b�N���`�F�b�N
	audio->run();					// �T�E���h�G���W���̎����I�^�X�N�����s
									// if Alt+Enter toggle fullscreen/window
	if (input->isKeyDown(ALT_KEY) && input->wasKeyPressed(ENTER_KEY))
		setDisplayMode(graphicsNS::TOGGLE); // toggle fullscreen/window

											// if Esc key, set window mode
	if (input->isKeyDown(ESC_KEY))
		setDisplayMode(graphicsNS::WINDOW); // set window mode

											// ���͂��N���A
											// ���ׂẴL�[�`�F�b�N���s��ꂽ�ケ����Ăяo��
	input->clear(inputNS::KEYS_PRESSED);
}

//=============================================================================
// �O���t�B�b�N�X�f�o�C�X�����������ꍇ
// �O���t�B�b�N�X�f�o�C�X�����Z�b�g�\�ɂ��邽��
// �\�񂳂�Ă����r�f�I�����������ׂĉ��
//=============================================================================
void Game::releaseAll()
{
	SAFE_ON_LOST_DEVICE(messageDialog);
	SAFE_ON_LOST_DEVICE(console);
	dxFont.onLostDevice();
	return;
}

//=============================================================================
// ���ׂẴT�[�t�F�C�X���č쐬���A���ׂẴG���e�B�e�B�����Z�b�g
//=============================================================================
void Game::resetAll()
{
	dxFont.onResetDevice();
	SAFE_ON_RESET_DEVICE(console);
	SAFE_ON_RESET_DEVICE(messageDialog);
	return;
}

//=============================================================================
// �\�񂳂�Ă��������������ׂč폜
//=============================================================================
void Game::deleteAll()
{
	// ���ׂẴO���t�B�b�N�X�A�C�e���ɂ���onLostDevice()���Ăяo��
	releaseAll();
	SAFE_DELETE(graphics);
	SAFE_DELETE(input);
	SAFE_DELETE(audio);
	SAFE_DELETE(console);
	SAFE_DELETE(messageDialog);
	initialized = false;
}

//=============================================================================
// �R���\�[���R�}���h������
// �V�����R���\�[���R�}���h��ǉ�����ꍇ�́A
// ���̊֐���h���N���X�ŃI�[�o�[���C�h����
//=============================================================================
void Game::consoleCommand()
{
	command = console->getCommand();	// �R���\�[������̃R�}���h���擾
	if (command == "")					// �R�}���h���Ȃ��ꍇ
		return;
	if (command == "help")				// �uhelp�v�R�}���h�̏ꍇ
	{
		console->print("Console Commands:");
		console->print("fps - toggle display of frames per second");
		return;
	}
	if (command == "fps")
	{
		fpsOn = !fpsOn;					// �t���[�����[�g�̕\����؂�ւ���
		if (fpsOn)
			console->print("fps On");
		else
			console->print("fps Off");
	}
}