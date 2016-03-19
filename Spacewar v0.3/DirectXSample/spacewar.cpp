// Spacewar�͐V�����쐬����N���X
#include "spacewar.h"
#include <iostream>

//=============================================================================
// �R���X�g���N�^
//=============================================================================
Spacewar::Spacewar()
{
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
	
	// �Q�[���Ɏg�p����e�N�X�`���Q
	if (!gameTextures.initialize(graphics, TEXTURES_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR,
			"Error initializing planet texture"));

	// ���_�inebula�j
	if (!nebula.initialize(graphics, 0, 0, 0, &nebulaTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing nebula"));
	
	// �f���iplanet�j
	if (!planet.initialize(this, planetNS::WIDTH, planetNS::HEIGHT, 2, &gameTextures))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing planet"));
	// ��ʂ̒����ɘf����z�u
	planet.setX(GAME_WIDTH * 0.5f - planet.getWidth() * 0.5f);
	planet.setY(GAME_HEIGHT * 0.5f - planet.getHeight() * 0.5f);
	
	// �F���D1
	if (!ship1.initialize(this, shipNS::WIDTH, shipNS::HEIGHT, shipNS::TEXTURE_COLS, &gameTextures))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ship1"));
	ship1.setFrames(shipNS::SHIP1_START_FRAME, shipNS::SHIP1_END_FRAME);
	ship1.setCurrentFrame(shipNS::SHIP1_START_FRAME);
	//ship1.setX(GAME_WIDTH / 4);
	//ship1.setY(GAME_HEIGHT / 4);
	//ship1.setVelocity(VECTOR2(shipNS::SPEED, -shipNS::SPEED)); // VECTOR2(X, Y)
	ship1.setX(GAME_WIDTH / 4 - shipNS::WIDTH);
	ship1.setY(GAME_HEIGHT / 2 - shipNS::HEIGHT);
	ship1.setVelocity(VECTOR2(0, -shipNS::SPEED));
	// �F���D2
	if (!ship2.initialize(this, shipNS::WIDTH, shipNS::HEIGHT, shipNS::TEXTURE_COLS, &gameTextures))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ship2"));
	ship2.setFrames(shipNS::SHIP2_START_FRAME, shipNS::SHIP2_END_FRAME);
	ship2.setCurrentFrame(shipNS::SHIP2_START_FRAME);
	ship2.setX(GAME_WIDTH - GAME_WIDTH / 4);
	ship2.setY(GAME_HEIGHT / 4);
	ship2.setVelocity(VECTOR2(-shipNS::SPEED, -shipNS::SPEED)); // VECTOR2(X, Y)

	return;
}

//=============================================================================
// ���ׂẴQ�[���A�C�e�����X�V
//=============================================================================
void Spacewar::update()
{
	if (ship1.getActive())
	{
		VECTOR2 vector = ship1.getVelocity();
		if (input->isKeyDown(SHIP1_RIGHT_KEY))            // �E�Ɉړ�����ꍇ
		{
			vector.x += frameTime * shipNS::SPEED;
		}
		if (input->isKeyDown(SHIP1_LEFT_KEY))             // ���Ɉړ�����ꍇ
		{
			vector.x -= frameTime * shipNS::SPEED;
		}
		if (input->isKeyDown(SHIP1_UP_KEY))               // ��Ɉړ�����ꍇ
		{
			vector.y -= frameTime * shipNS::SPEED;
		}
		if (input->isKeyDown(SHIP1_DOWN_KEY))             // ���Ɉړ�����ꍇ
		{
			vector.y += frameTime * shipNS::SPEED;
		}
		ship1.setVelocity(vector);
	}
	if (ship2.getActive())
	{
		VECTOR2 vector = ship2.getVelocity();
		if (input->isKeyDown(SHIP2_RIGHT_KEY))            // �E�Ɉړ�����ꍇ
		{
			vector.x += frameTime * shipNS::SPEED;
		}
		if (input->isKeyDown(SHIP2_LEFT_KEY))             // ���Ɉړ�����ꍇ
		{
			vector.x -= frameTime * shipNS::SPEED;
		}
		if (input->isKeyDown(SHIP2_UP_KEY))               // ��Ɉړ�����ꍇ
		{
			vector.y -= frameTime * shipNS::SPEED;
		}
		if (input->isKeyDown(SHIP2_DOWN_KEY))             // ���Ɉړ�����ꍇ
		{
			vector.y += frameTime * shipNS::SPEED;
		}
		ship2.setVelocity(vector);
	}
	ship1.gravityForce(&planet, frameTime);
	ship2.gravityForce(&planet, frameTime);

	planet.update(frameTime);
	ship1.update(frameTime);
	ship2.update(frameTime);
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
{
	VECTOR2 collisionVector;
	// if collision between ship and planet
	if (ship1.collidesWith(planet, collisionVector))
	{
		// bounce off planet
		ship1.bounce(collisionVector, planet);
		ship1.damage(PLANET);
	}
	if (ship2.collidesWith(planet, collisionVector))
	{
		// bounce off planet
		ship2.bounce(collisionVector, planet);
		ship2.damage(PLANET);
	}
	// if collision between ships
	if (ship1.collidesWith(ship2, collisionVector))
	{
		// bounce off ship
		ship1.bounce(collisionVector, ship2);
		ship1.damage(SHIP);
		// change the direction of the collisionVector for ship2
		ship2.bounce(collisionVector*-1, ship1);
		ship2.damage(SHIP);
	}
}

//=============================================================================
// �Q�[���A�C�e���������_�[
//=============================================================================
void Spacewar::render()
{
	graphics->spriteBegin();	// �X�v���C�g�̕`����J�n

	nebula.draw();				// �I���I�����_���V�[���ɒǉ�
	planet.draw();				// �f�����V�[���ɒǉ�
	ship1.draw();
	ship2.draw();

	graphics->spriteEnd();		// �X�v���C�g�̕`����I��
}

//=============================================================================
// �O���t�B�b�N�X�f�o�C�X�����������ꍇ
// �O���t�B�b�N�X�f�o�C�X�����Z�b�g�\�ɂ��邽�߁A
// �\�񂳂�Ă����r�f�I�����������ׂĉ��
//=============================================================================
void Spacewar::releaseAll()
{
	nebulaTexture.onLostDevice();
	gameTextures.onLostDevice();
	Game::releaseAll();
	return;
}

//=============================================================================
// �O���t�B�b�N�X�f�o�C�X�����Z�b�g���ꂽ�ꍇ
// ���ׂẴT�[�t�F�C�X���č쐬
//=============================================================================
void Spacewar::resetAll()
{
	gameTextures.onResetDevice();
	nebulaTexture.onResetDevice();
	Game::resetAll();
	return;
}
