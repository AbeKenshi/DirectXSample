// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Chapter 8 threeCs.cpp v1.0

#include "threeCs.h"
using namespace textNS;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
ThreeCs::ThreeCs()
{
    fontCK = new Text();        // �X�v���C�g�x�[�X�̃t�H���g
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
ThreeCs::~ThreeCs()
{
    releaseAll();               // ���ׂẴO���t�B�b�N�X�A�C�e���ɂ���
								// onLostDevice()���Ăяo��
    SAFE_DELETE(fontCK);
}

//=============================================================================
// �Q�[����������
// �G���[����GameError���X���[
//=============================================================================
void ThreeCs::initialize(HWND hwnd)
{
    Game::initialize(hwnd);
    graphics->setBackColor(graphicsNS::WHITE);

    // �e�L�X�g��������
    if (!fontCK->initialize(graphics,FONT_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing CKfont"));

    reset();            // reset all game variables
    fpsOn = true;       // display frames per second

    return;
}

//=============================================================================
// Reset the game to begin play and after a score
//=============================================================================
void ThreeCs::reset()
{
    return;
}

//=============================================================================
// move all game items
// frameTime is used to regulate the speed of movement
//=============================================================================
void ThreeCs::update()
{
}

//=============================================================================
// �Q�[���A�C�e���������_�[
//=============================================================================
void ThreeCs::render()
{
    graphics->spriteBegin();
    fontCK->setProportional(false);
    fontCK->setFontColor(graphicsNS::BLACK);
    fontCK->setBackColor(TRANSCOLOR);
    fontCK->setFontHeight(FONT_HEIGHT*2);
    fontCK->print("C",20,100);
    fontCK->setFontHeight(FONT_HEIGHT);
    fontCK->print("C",114,148);
    fontCK->setFontHeight(FONT_HEIGHT/4);
    fontCK->print("C",164,184);
    graphics->spriteEnd();
}

//=============================================================================
// �O���t�B�b�N�X�f�o�C�X�����������ꍇ
// �O���t�B�b�N�X�f�o�C�X�����Z�b�g�\�ɂ��邽�߁A
// �\�񂳂�Ă����r�f�I�����������ׂĉ��
//=============================================================================
void ThreeCs::releaseAll()
{
    fontCK->onLostDevice();
    Game::releaseAll();
    return;
}

//=============================================================================
// �O���t�B�b�N�X�f�o�C�X�����Z�b�g���ꂽ�ꍇ
// ���ׂẴT�[�t�F�C�X���č쐬���A���ׂẴG���e�B�e�B�����Z�b�g
//=============================================================================
void ThreeCs::resetAll()
{
    fontCK->onResetDevice();
    Game::resetAll();
    return;
}
