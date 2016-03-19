// Spacewar�͐V�����쐬����N���X
#include "spacewar.h"
#include <iostream>

//=============================================================================
// �R���X�g���N�^
//=============================================================================
Spacewar::Spacewar()
{
	Xvelocity = 0.0f;
	Yvelocity = 0.0f;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
Spacewar::~Spacewar()
{
	releaseAll();           // ���ׂẴO���t�B�b�N�X�A�C�e���ɂ��āAonLostDevie()���Ăяo��
}

//=============================================================================
// �Q�[����������
// �G���[����GameError���X���[
//=============================================================================
void Spacewar::initialize(HWND hwnd)
{
	Game::initialize(hwnd); // GameError���X���[
	
	// ���_�inebula�j�̃e�N�X�`��
	if (!nebulaTexture.initialize(graphics, NEBULA_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR,
			"Error initializing nebula texture"));
	
	// �f���iplanet�j�̃e�N�X�`��
	if (!planetTexture.initialize(graphics, PLANET_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR,
			"Error initializing planet texture"));
	
	// �F���D�e�N�X�`��
	if (!shipTexture.initialize(graphics, SHIP_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR,
			"Error initializing ship texture"));

	// ���_�inebula�j
	if (!nebula.initialize(graphics, 0, 0, 0, &nebulaTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing nebula"));
	
	// �f���iplanet�j
	if (!planet.initialize(graphics, 0, 0, 0, &planetTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing planet"));
	// ��ʂ̒����ɘf����z�u
	planet.setX(GAME_WIDTH * 0.5f - planet.getWidth() * 0.5f);
	planet.setY(GAME_HEIGHT * 0.5f - planet.getHeight() * 0.5f);

	// �F���D
	if (!ship.initialize(graphics, SHIP_WIDTH, SHIP_HEIGHT, SHIP_COLS, &shipTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ship"));
	ship.setX(GAME_WIDTH / 4);	// �f���̍��ォ��o��
	ship.setY(GAME_HEIGHT / 4);
	ship.setFrames(SHIP_START_FRAME, SHIP_END_FRAME);	// �A�j���[�V�����̃t���[��
	ship.setCurrentFrame(SHIP_START_FRAME);				// �J�n�t���[��
	ship.setFrameDelay(SHIP_ANIMATION_DELAY);
	return;
}

//=============================================================================
// ���ׂẴQ�[���A�C�e�����X�V
//=============================================================================
void Spacewar::update()
{
	if (input->isKeyDown(SHIP_RIGHT_KEY))            // �E�Ɉړ�����ꍇ
	{
		Xvelocity += frameTime * SHIP_SPEED;
	}
	if (input->isKeyDown(SHIP_LEFT_KEY))             // ���Ɉړ�����ꍇ
	{
		Xvelocity -= frameTime * SHIP_SPEED;
	}
	if (input->isKeyDown(SHIP_UP_KEY))               // ��Ɉړ�����ꍇ
	{
		Yvelocity -= frameTime * SHIP_SPEED;
	}
	if (input->isKeyDown(SHIP_DOWN_KEY))             // ���Ɉړ�����ꍇ
	{
		Yvelocity += frameTime * SHIP_SPEED;
	}
	ship.setX(ship.getX() + frameTime * Xvelocity);	// X���x��K�p
	ship.setY(ship.getY() + frameTime * Yvelocity);	// Y���x��K�p

	if (ship.getX() > GAME_WIDTH)               // ��ʉE�[�𒴂�����
		ship.setX((float)-ship.getWidth());     // ��ʍ��[�Ɉړ�
	if (ship.getX() < -ship.getWidth())         // ��ʍ��[�𒴂�����
		ship.setX((float)GAME_WIDTH);           // ��ʉE�[�Ɉړ�
	if (ship.getY() < -ship.getHeight())        // ��ʏ�[�𒴂�����
		ship.setY((float)GAME_HEIGHT);          // ��ʉ��[�Ɉړ�
	if (ship.getY() > GAME_HEIGHT)              // ��ʉ��[�𒴂�����
		ship.setY((float)-ship.getHeight());    // ��ʏ�[�Ɉړ�

	ship.update(frameTime);
}

//=============================================================================
// �l�H�m�\
//=============================================================================
void Spacewar::ai()
{}

//=============================================================================
// �Փ˂�����
//=============================================================================
void Spacewar::collisions()
{}

//=============================================================================
// �Q�[���A�C�e���������_�[
//=============================================================================
void Spacewar::render()
{
	graphics->spriteBegin();	// �X�v���C�g�̕`����J�n

	nebula.draw();				// �I���I�����_���V�[���ɒǉ�
	planet.draw();				// �f�����V�[���ɒǉ�
	ship.draw();

	graphics->spriteEnd();		// �X�v���C�g�̕`����I��
}

//=============================================================================
// �O���t�B�b�N�X�f�o�C�X�����������ꍇ
// �O���t�B�b�N�X�f�o�C�X�����Z�b�g�\�ɂ��邽�߁A
// �\�񂳂�Ă����r�f�I�����������ׂĉ��
//=============================================================================
void Spacewar::releaseAll()
{
	shipTexture.onLostDevice();
	planetTexture.onLostDevice();
	nebulaTexture.onLostDevice();
	Game::releaseAll();
	return;
}

//=============================================================================
// �O���t�B�b�N�X�f�o�C�X�����Z�b�g���ꂽ�ꍇ
// ���ׂẴT�[�t�F�C�X���č쐬
//=============================================================================
void Spacewar::resetAll()
{
	nebulaTexture.onResetDevice();
	planetTexture.onResetDevice();
	shipTexture.onResetDevice();
	Game::resetAll();
	return;
}
