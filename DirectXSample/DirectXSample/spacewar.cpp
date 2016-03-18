// Spacewar�͐V�����쐬����N���X
#include "spacewar.h"
#include <iostream>

//=============================================================================
// �R���X�g���N�^
//=============================================================================
Spacewar::Spacewar()
{
	menuOn = true;
	countDownOn = false;
	roundOver = false;
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
	Game::initialize(hwnd); // throws GameError

	// initialize DirectX fonts
	fontBig.initialize(graphics, spacewarNS::FONT_BIG_SIZE, false, false, spacewarNS::FONT);
	fontBig.setFontColor(spacewarNS::FONT_COLOR);

	// menu texture
	if (!menuTexture.initialize(graphics, MENU_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu texture"));

	// ���_�ispace�j�̃e�N�X�`��
	if (!spaceTexture.initialize(graphics, SPACE_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR,
			"Error initializing space texture"));

	// game textures
	if (!textures1.initialize(graphics, TEXTURES1_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing game textures"));


	// nebula texture
	if (!nebulaTexture.initialize(graphics, NEBULA_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing nebula texture"));

	// main game textures
	if (!gameTextures.initialize(graphics, TEXTURES_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing game textures"));

	// ����̉摜
	if (!space.initialize(graphics, 0, 0, 0, &spaceTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR,
			"Error initializing space"));
	space.setScale((float)SPACE_SCALE);

	// moon images
	for (int i = 0; i<4; i++)
	{
		if (!moons[i].initialize(graphics, MOON_SIZE, MOON_SIZE, 4, &textures1))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing moons"));
		moons[i].setFrames(MOON1_FRAME + i, MOON1_FRAME + i);
		moons[i].setCurrentFrame(MOON1_FRAME + i);
		// Start moons in center
		moons[i].setX(GAME_WIDTH / 2 - MOON_SIZE / 2);
		moons[i].setY(GAME_HEIGHT / 2 - MOON_SIZE / 2);
	}

	// nebula image
	if (!nebula.initialize(graphics, 0, 0, 0, &nebulaTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing nebula"));

	// planet
	if (!planet.initialize(this, planetNS::WIDTH, planetNS::HEIGHT, 2, &gameTextures))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing planet"));

	// ship
	if (!ship1.initialize(this, shipNS::WIDTH, shipNS::HEIGHT, shipNS::TEXTURE_COLS, &gameTextures))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ship1"));
	ship1.setFrames(shipNS::SHIP1_START_FRAME, shipNS::SHIP1_END_FRAME);
	ship1.setCurrentFrame(shipNS::SHIP1_START_FRAME);
	ship1.setX(GAME_WIDTH / 4);
	ship1.setY(GAME_HEIGHT / 4);
	ship1.setVelocity(VECTOR2(shipNS::SPEED, -shipNS::SPEED)); // VECTOR2(X, Y)
															   // ship2
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
	// �G���e�B�e�B���X�V
	ship1.update(frameTime);
	ship2.update(frameTime);
	// �f����X�����ɓ�����
	planet.setX(planet.getX() - frameTime * ship1.getVelocity().x);
	// �f����Y�����ɓ�����
	planet.setY(planet.getY() - frameTime * ship1.getVelocity().y);
	planet.update(frameTime);
	for (int i = 0; i < 4; ++i)	// �q���𓮂���
	{
		// �X�N���[���̑������A�q�����Ƃ�20%�x������
		moons[i].setX(moons[i].getX() -
			frameTime * ship1.getVelocity().x * 0.2f * (4 - i));
		moons[i].setY(moons[i].getY() -
			frameTime * ship1.getVelocity().y * 0.2f * (4 - i));
	}
	// ������F���D�Ɣ��΂�X�����Ɉړ�
	space.setX(space.getX() - frameTime * ship1.getVelocity().x);
	// ������F���D�Ɣ��΂�Y�����Ɉړ�
	space.setY(space.getY() - frameTime * ship1.getVelocity().y);

	// ����摜��[�Ő܂�Ԃ�
	// ����̍��[ > ��ʂ̍��[�̏ꍇ
	if (space.getX() > 0)
		// ����摜��SPACE_WIDTH���������ɓ�����
		space.setX(space.getX() - SPACE_WIDTH);
	// ����摜����ʂ���O��č��ɂ���ꍇ
	if (space.getX() < -SPACE_WIDTH)
		// ����摜��SPACE_WIDTH�������E�ɓ�����
		space.setX(space.getX() + SPACE_WIDTH);
	// ����̏�[ > ��ʂ̏�[�̏ꍇ
	if (space.getY() > 0)
		// ����摜��SPACE_HEIGHT��������ɓ�����
		space.setY(space.getY() - SPACE_HEIGHT);
	// ����摜����ʂ���O��ď�ɂ���ꍇ
	if (space.getY() < -SPACE_HEIGHT)
		space.setY(space.getY() + SPACE_HEIGHT);
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
	// �F���D�Ƙf���̏Փ˂̏ꍇ
	if (ship1.collidesWith(planet, collisionVector))
	{
		// �f�����璵�˕Ԃ�
		ship1.bounce(collisionVector, planet);
		ship1.damage(PLANET);
	}
	if (ship2.collidesWith(planet, collisionVector))
	{
		// �f�����璵�˕Ԃ�
		ship2.bounce(collisionVector, planet);
		ship2.damage(PLANET);
	}
	// �F���D���m�̏Փ˂̏ꍇ
	if (ship1.collidesWith(ship2, collisionVector))
	{
		// �F���D���璵�˕Ԃ�
		ship1.bounce(collisionVector, ship2);
		ship1.damage(SHIP);
		// ship2��collisionVector�̕�����ύX����
		ship2.bounce(collisionVector*-1, ship1);
		ship2.damage(SHIP);
	}
}

//=============================================================================
// �Q�[���A�C�e���������_�[
//=============================================================================
void Spacewar::render()
{
	float x = space.getX();
	float y = space.getY();
	graphics->spriteBegin();	// �X�v���C�g�̕`����J�n
	// ����摜��[�Ő܂�Ԃ�
	space.draw();				// ���݂̈ʒu�ŕ`��
	// ����摜�̉E�[��������ꍇ
	if (space.getX() < -SPACE_WIDTH + (int)GAME_WIDTH)
	{
		space.setX(space.getX() + SPACE_WIDTH);		// ���[�ɖ߂�
		space.draw();								// �ēx�A�`�悷��
	}
	// ����摜�̉��[��������ꍇ
	if (space.getY() < -SPACE_HEIGHT + (int)GAME_HEIGHT)
	{
		space.setY(space.getY() + SPACE_HEIGHT);	// ��[�ɖ߂�
		space.draw();								// �ēx�A�`�悷��
		space.setX(x);								// X�ʒu�𕜌�
		// ����摜�̉E�[��������ꍇ
		// ���[�ɖ߂�
		if (space.getX() < -SPACE_WIDTH + (int)GAME_WIDTH)
			space.draw();							// �ēx�A�`�悷��
	}
	space.setY(y);									// Y�ʒu�𕜌�
	//nebula.draw();				// �I���I�����_���V�[���ɒǉ�
	for (int i = 3; i >= 0; --i)
	{
		moons[i].draw();			// �q���̕`��
	}
	planet.draw();				// �f�����V�[���ɒǉ�
	
	// �F���D��`��
	ship1.draw();
	ship2.draw();

	if (menuOn)
		menu.draw();
	if (countDownOn)
	{
		_snprintf_s(buffer, spacewarNS::BUF_SIZE, "%d",
			(int)(ceil(countDownTimer)));
		fontBig.print(buffer, spacewarNS::COUNT_DOWN_X,
			spacewarNS::COUNT_DOWN_Y);
	}
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
