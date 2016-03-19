// Spacewar�͐V�����쐬����N���X
#include "spacewar.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
Spacewar::Spacewar()
{
	menuOn = true;
	countDownOn = false;
	roundOver = false;
	ship1Score = 0;
	ship2Score = 0;
	ship1Scored = false;
	ship2Scored = false;
	initialized = false;
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

	// DirectX�t�H���g��������
	fontBig.initialize(graphics, spacewarNS::FONT_BIG_SIZE, false, false, spacewarNS::FONT);
	fontBig.setFontColor(spacewarNS::FONT_COLOR);
	fontScore.initialize(graphics, spacewarNS::FONT_SCORE_SIZE, false, false, spacewarNS::FONT);

	// ���j���[�̃e�N�X�`��
	if (!menuTexture.initialize(graphics, MENU_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu texture"));

	// ���_�̃e�N�X�`��
	if (!nebulaTexture.initialize(graphics, NEBULA_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing nebula texture"));

	// ���C���̃Q�[���e�N�X�`��
	if (!gameTextures.initialize(graphics, TEXTURES_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing game textures"));

	// ���j���[�̉摜
	if (!menu.initialize(graphics, 0, 0, 0, &menuTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu"));

	// ���_�̉摜
	if (!nebula.initialize(graphics, 0, 0, 0, &nebulaTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing nebula"));

	// �f��
	if (!planet.initialize(this, planetNS::WIDTH, planetNS::HEIGHT, 2, &gameTextures))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing planet"));

	// �F���D1
	if (!ship1.initialize(this, shipNS::WIDTH, shipNS::HEIGHT, shipNS::TEXTURE_COLS, &gameTextures))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ship1"));
	ship1.setFrames(shipNS::SHIP1_START_FRAME, shipNS::SHIP1_END_FRAME);
	ship1.setCurrentFrame(shipNS::SHIP1_START_FRAME);
	// ���邢�F�A�V�[���h�Ɏg�p
	ship1.setColorFilter(SETCOLOR_ARGB(255, 230, 230, 255));
	ship1.setMass(shipNS::MASS);

	// �F���D2
	if (!ship2.initialize(this, shipNS::WIDTH, shipNS::HEIGHT, shipNS::TEXTURE_COLS, &gameTextures))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ship2"));

	ship2.setFrames(shipNS::SHIP2_START_FRAME, shipNS::SHIP2_END_FRAME);
	ship2.setCurrentFrame(shipNS::SHIP2_START_FRAME);
	// ���邢���F�A�V�[���h�Ɏg�p
	ship2.setColorFilter(SETCOLOR_ARGB(255, 255, 255, 64));
	ship2.setMass(shipNS::MASS);

	// �~�T�C��1
	if (!torpedo1.initialize(this, torpedoNS::WIDTH, torpedoNS::HEIGHT, torpedoNS::TEXTURE_COLS, &gameTextures))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing torpedo1"));

	torpedo1.setFrames(torpedoNS::START_FRAME, torpedoNS::END_FRAME);
	torpedo1.setCurrentFrame(torpedoNS::START_FRAME);
	// ���邢�F
	torpedo1.setColorFilter(SETCOLOR_ARGB(255, 128, 128, 255));

	// �~�T�C��2
	if (!torpedo2.initialize(this, torpedoNS::WIDTH, torpedoNS::HEIGHT, torpedoNS::TEXTURE_COLS, &gameTextures))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing torpedo2"));

	torpedo2.setFrames(torpedoNS::START_FRAME, torpedoNS::END_FRAME);
	torpedo2.setCurrentFrame(torpedoNS::START_FRAME);
	// ���邢���F
	torpedo2.setColorFilter(SETCOLOR_ARGB(255, 255, 255, 64));

	// �̗̓o�[
	healthBar.initialize(graphics, &gameTextures, 0, spacewarNS::HEALTHBAR_Y, 2.0f, graphicsNS::WHITE);

	// 2�̉F���D�͘f��������ŗ�������o�����A���̋O��������v���Ɏ���
	ship1.setX(GAME_WIDTH / 4 - shipNS::WIDTH);
	ship2.setX(GAME_WIDTH - GAME_WIDTH / 4);
	ship1.setY(GAME_HEIGHT / 2 - shipNS::HEIGHT);
	ship2.setY(GAME_HEIGHT / 2);
	ship1.setVelocity(VECTOR2(0, -shipNS::SPEED));
	ship2.setVelocity(VECTOR2(0, shipNS::SPEED));

	return;
}

//=============================================================================
// ���ׂẴQ�[���A�C�e�����X�V
//=============================================================================
void Spacewar::update()
{
	if (menuOn)
	{
		if (input->anyKeyPressed())
		{
			menuOn = false;
			input->clearAll();
			roundStart();
		}
	}
	else if (countDownOn)
	{
		countDownTimer -= frameTime;
		if (countDownTimer <= 0)
			countDownOn = false;
	}
	else
	{
		if (ship1.getActive())
		{
			// �G���W����L���ɂ���ꍇ
			if (input->isKeyDown(SHIP1_FORWARD_KEY) || input->getGamepadDPadUp(0))
			{
				ship1.setEngineOn(true);
				audio->playCue(ENGINE1);
			}
			else
			{
				ship1.setEngineOn(false);
				audio->stopCue(ENGINE1);
			}
			ship1.rotate(shipNS::NONE);
			// �F���D1�����Ɍ�����ꍇ
			if (input->isKeyDown(SHIP1_LEFT_KEY) || input->getGamepadDPadLeft(0))
				ship1.rotate(shipNS::LEFT);
			// �F���D1���E�Ɍ�����ꍇ
			if (input->isKeyDown(SHIP1_RIGHT_KEY) || input->getGamepadDPadRight(0))
				ship1.rotate(shipNS::RIGHT);
			// �F���D1���~�T�C���𔭎˂���ꍇ
			if (input->isKeyDown(SHIP1_FIRE_KEY) || input->getGamepadA(0))
				torpedo1.fire(&ship1);                  // �~�T�C��1�𔭎�
		}
		if (ship2.getActive())
		{
			// �G���W����L���ɂ���ꍇ
			if (input->isKeyDown(SHIP2_FORWARD_KEY) || input->getGamepadDPadUp(1))
			{
				ship2.setEngineOn(true);
				audio->playCue(ENGINE2);
			}
			else
			{
				ship2.setEngineOn(false);
				audio->stopCue(ENGINE2);
			}
			ship2.rotate(shipNS::NONE);
			// �F���D2�����Ɍ�����ꍇ
			if (input->isKeyDown(VK_LEFT) || input->getGamepadDPadLeft(1))
				ship2.rotate(shipNS::LEFT);
			// �F���D2���E�Ɍ�����ꍇ
			if (input->isKeyDown(VK_RIGHT) || input->getGamepadDPadRight(1))
				ship2.rotate(shipNS::RIGHT);
			// �F���D2���~�T�C���𔭎˂���ꍇ
			if (input->isKeyDown(SHIP2_FIRE_KEY) || input->getGamepadA(1))
				torpedo2.fire(&ship2);                  // �~�T�C��2�𔭎�
		}
		if (roundOver)
		{
			roundTimer -= frameTime;
			if (roundTimer <= 0)
				roundStart();
		}
	}
	ship1.gravityForce(&planet, frameTime);
	ship2.gravityForce(&planet, frameTime);
	torpedo1.gravityForce(&planet, frameTime);
	torpedo2.gravityForce(&planet, frameTime);

	// �G���e�B�e�B���X�V
	planet.update(frameTime);
	ship1.update(frameTime);
	ship2.update(frameTime);
	torpedo1.update(frameTime);
	torpedo2.update(frameTime);
}

//=============================================================================
// �v���C�̐V�������E���h���J�n
//=============================================================================
void Spacewar::roundStart()
{
	// 2�̉F���D�͘f��������ŗ�������o�����A���̋O��������v���Ɏ���
	ship1.setX(GAME_WIDTH / 4 - shipNS::WIDTH);
	ship2.setX(GAME_WIDTH - GAME_WIDTH / 4);
	ship1.setY(GAME_HEIGHT / 2 - shipNS::HEIGHT);
	ship2.setY(GAME_HEIGHT / 2);
	ship1.setVelocity(VECTOR2(0, -shipNS::SPEED));
	ship2.setVelocity(VECTOR2(0, shipNS::SPEED));

	ship1.setDegrees(0);
	ship2.setDegrees(180);
	ship1.repair();
	ship2.repair();
	countDownTimer = spacewarNS::COUNT_DOWN;
	countDownOn = true;
	roundOver = false;
	ship1Scored = false;
	ship2Scored = false;
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
	// �F���D1�Ƙf���̏Փ˂̏ꍇ
	if (ship1.collidesWith(planet, collisionVector))
	{
		ship1.toOldPosition();      // �F���D���Փ˂��痣���悤�Ɉړ�
		ship1.damage(PLANET);
		// �R���g���[���[0��U��������i100%�A1.0�b�j
		input->gamePadVibrateLeft(0, 65535, 1.0);
	}
	// �F���D2�Ƙf���̏Փ˂̏ꍇ
	if (ship2.collidesWith(planet, collisionVector))
	{
		ship2.toOldPosition();      // �F���D���Փ˂��痣���悤�Ɉړ�
		ship2.damage(PLANET);
		// �R���g���[���[1��U��������i100%�A1.0�b�j
		input->gamePadVibrateLeft(1, 65535, 1.0);
	}
	// �F���D1�ƉF���D2�̏Փ˂̏ꍇ
	if (ship1.collidesWith(ship2, collisionVector))
	{
		// �F���D���璵�˕Ԃ�
		ship1.bounce(collisionVector, ship2);
		ship2.bounce(collisionVector*-1, ship1);
		ship1.damage(SHIP);
		ship2.damage(SHIP);
		input->gamePadVibrateRight(0, 30000, 0.5);
		input->gamePadVibrateRight(1, 30000, 0.5);
	}
	// �~�T�C���ƉF���D�̏Փ˂̏ꍇ
	if (torpedo1.collidesWith(ship2, collisionVector))
	{
		ship2.damage(TORPEDO);
		torpedo1.setVisible(false);
		torpedo1.setActive(false);
		input->gamePadVibrateRight(1, 20000, 0.5);
	}
	if (torpedo2.collidesWith(ship1, collisionVector))
	{
		ship1.damage(TORPEDO);
		torpedo2.setVisible(false);
		torpedo2.setActive(false);
		input->gamePadVibrateRight(0, 20000, 0.5);
	}
	// �~�T�C���Ƙf���̏Փ˂̏ꍇ
	if (torpedo1.collidesWith(planet, collisionVector))
	{
		torpedo1.setVisible(false);
		torpedo1.setActive(false);
		audio->playCue(TORPEDO_CRASH);
	}
	if (torpedo2.collidesWith(planet, collisionVector))
	{
		torpedo2.setVisible(false);
		torpedo2.setActive(false);
		audio->playCue(TORPEDO_CRASH);
	}

	// �X�R�A���`�F�b�N
	if (ship1.getActive() == false && ship2Scored == false)
	{
		ship2Score++;
		ship2Scored = true;
		if (roundOver == false)
		{
			roundTimer = spacewarNS::ROUND_TIME;
			roundOver = true;
		}
	}
	if (ship2.getActive() == false && ship1Scored == false)
	{
		ship1Score++;
		ship1Scored = true;
		if (roundOver == false)
		{
			roundTimer = spacewarNS::ROUND_TIME;
			roundOver = true;
		}
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

	// �X�R�A��\��
	fontScore.setFontColor(spacewarNS::SHIP1_COLOR);
	_snprintf_s(buffer, spacewarNS::BUF_SIZE, "%d", (int)ship1Score);
	fontScore.print(buffer, spacewarNS::SCORE1_X, spacewarNS::SCORE_Y);
	fontScore.setFontColor(spacewarNS::SHIP2_COLOR);
	_snprintf_s(buffer, spacewarNS::BUF_SIZE, "%d", (int)ship2Score);
	fontScore.print(buffer, spacewarNS::SCORE2_X, spacewarNS::SCORE_Y);
	
	// �̗̓o�[��\��
	healthBar.setX((float)spacewarNS::SHIP1_HEALTHBAR_X);
	healthBar.set(ship1.getHealth());
	healthBar.draw(spacewarNS::SHIP1_COLOR);
	healthBar.setX((float)spacewarNS::SHIP2_HEALTHBAR_X);
	healthBar.set(ship2.getHealth());
	healthBar.draw(spacewarNS::SHIP2_COLOR);
	
	// �F���D��`��
	ship1.draw();
	ship2.draw();

	// colorFilter���g���ă~�T�C����`��
	torpedo1.draw(graphicsNS::FILTER);
	torpedo2.draw(graphicsNS::FILTER);

	if (menuOn)
		menu.draw();
	if (countDownOn)
	{
		_snprintf_s(buffer, spacewarNS::BUF_SIZE, "%d", (int)(ceil(countDownTimer)));
		fontBig.print(buffer, spacewarNS::COUNT_DOWN_X, spacewarNS::COUNT_DOWN_Y);
	}

	graphics->spriteEnd();		// �X�v���C�g�̕`����I��
}

//=============================================================================
// �R���\�[���R�}���h������
//=============================================================================
void Spacewar::consoleCommand()
{
	command = console->getCommand();    // �R���\�[������̃R�}���h���擾
	if (command == "")                  // �R�}���h���Ȃ��ꍇ
		return;

	if (command == "help")              // �uhelp�v�R�}���h�̏ꍇ
	{
		console->print("Console Commands:");
		console->print("fps - toggle display of frames per second");
		console->print("gravity off - turns off planet gravity");
		console->print("gravity on - turns on planet gravity");
		console->print("planet off - disables planet");
		console->print("planet on - enables planet");
		return;
	}
	if (command == "fps")
	{
		fpsOn = !fpsOn;                 // �t���[�����[�g�̕\����؂�ւ���
		if (fpsOn)
			console->print("fps On");
		else
			console->print("fps Off");
	}

	if (command == "gravity off")
	{
		planet.setMass(0);
		console->print("Gravity Off");
	}
	else if (command == "gravity on")
	{
		planet.setMass(planetNS::MASS);
		console->print("Gravity On");
	}
	else if (command == "planet off")
	{
		planet.disable();
		console->print("Planet Off");
	}
	else if (command == "planet on")
	{
		planet.enable();
		console->print("Planet On");
	}
}

//=============================================================================
// �O���t�B�b�N�X�f�o�C�X�����������ꍇ
// �O���t�B�b�N�X�f�o�C�X�����Z�b�g�\�ɂ��邽�߁A
// �\�񂳂�Ă����r�f�I�����������ׂĉ��
//=============================================================================
void Spacewar::releaseAll()
{
	menuTexture.onLostDevice();
	nebulaTexture.onLostDevice();
	gameTextures.onLostDevice();
	fontScore.onLostDevice();
	fontBig.onLostDevice();

	Game::releaseAll();
	return;
}

//=============================================================================
// �O���t�B�b�N�X�f�o�C�X�����Z�b�g���ꂽ�ꍇ
// ���ׂẴT�[�t�F�C�X���č쐬
//=============================================================================
void Spacewar::resetAll()
{
	fontBig.onResetDevice();
	fontScore.onResetDevice();
	gameTextures.onResetDevice();
	nebulaTexture.onResetDevice();
	menuTexture.onResetDevice();

	Game::resetAll();
	return;
}
