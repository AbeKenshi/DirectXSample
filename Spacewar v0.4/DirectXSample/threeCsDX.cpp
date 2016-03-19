// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Chapter 8 threeCsDX.cpp v1.0

#include "threeCsDX.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
ThreeCsDX::ThreeCsDX()
{
    dxFontSmall	= new TextDX();     // DirectX�t�H���g
    dxFontMedium = new TextDX();
    dxFontLarge	= new TextDX();
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
ThreeCsDX::~ThreeCsDX()
{
    releaseAll();               // ���ׂẴO���t�B�b�N�X�A�C�e���ɂ���
								// onLostDevice()���Ăяo��
    SAFE_DELETE(dxFontSmall);
    SAFE_DELETE(dxFontMedium);
    SAFE_DELETE(dxFontLarge);
}

//=============================================================================
// �Q�[����������
// �G���[����GameError���X���[
//=============================================================================
void ThreeCsDX::initialize(HWND hwnd)
{
    Game::initialize(hwnd);
    graphics->setBackColor(graphicsNS::WHITE);

    // DirectX�t�H���g��������
	// ����15�s�N�Z����Arial
    if(dxFontSmall->initialize(graphics, 15, true, false, "Arial") == false)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing DirectX font"));

    // ����62�s�N�Z����Arial
    if(dxFontMedium->initialize(graphics, 62, true, false, "Arial") == false)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing DirectX font"));

    // ����124�s�N�Z����Arial
    if(dxFontLarge->initialize(graphics, 124, true, false, "Arial") == false)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing DirectX font"));

    reset();            // ���ׂẴQ�[���ϐ������Z�b�g
    fpsOn = true;       // �t���[�����[�g�ifps�j��\��
    return;
}


//=============================================================================
// Reset the game to begin play and after a score
//=============================================================================
void ThreeCsDX::reset()
{
    return;
}

//=============================================================================
// move all game items
// frameTime is used to regulate the speed of movement
//=============================================================================
void ThreeCsDX::update()
{
}

//=============================================================================
// �Q�[���A�C�e���������_�[
//=============================================================================
void ThreeCsDX::render()
{
    graphics->spriteBegin();

    dxFontSmall->setFontColor(graphicsNS::BLACK);
    dxFontMedium->setFontColor(graphicsNS::BLACK);
    dxFontLarge->setFontColor(graphicsNS::BLACK);
    dxFontLarge->print("C",20,100);
    dxFontMedium->print("C",114,148);
    dxFontSmall->print("C",164,184);

    graphics->spriteEnd();
}

//=============================================================================
// �O���t�B�b�N�X�f�o�C�X�����������ꍇ
// �O���t�B�b�N�X�f�o�C�X�����Z�b�g�\�ɂ��邽�߁A�\�񂳂�Ă����r�f�I�����������ׂĉ��
//=============================================================================
void ThreeCsDX::releaseAll()
{
    dxFontSmall->onLostDevice();
    dxFontMedium->onLostDevice();
    dxFontLarge->onLostDevice();
    Game::releaseAll();
    return;
}

//=============================================================================
// �O���t�B�b�N�X�f�o�C�X�����Z�b�g���ꂽ�ꍇ
// ���ׂẴT�[�t�F�C�X���č쐬���A���ׂẴG���e�B�e�B�����Z�b�g
//=============================================================================
void ThreeCsDX::resetAll()
{
    dxFontSmall->onResetDevice();
    dxFontMedium->onResetDevice();
    dxFontLarge->onResetDevice();
    Game::resetAll();
    return;
}
