// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Chapter 9 dashboardDemo.cpp v1.0

#include "dashboardDemo.h"

//=============================================================================
// Constructor
//=============================================================================
DashboardDemo::DashboardDemo()
{
    horizontal = 100;
    vertical = 100;
    dial = 0;
    vu = 100;
}

//=============================================================================
// Destructor
//=============================================================================
DashboardDemo::~DashboardDemo()
{
    releaseAll();				// call deviceLost() for every graphics item
}


//=============================================================================
// initializes the game
// Throws GameError on error
//=============================================================================
void DashboardDemo::initialize(HWND hwnd)
{
    Game::initialize(hwnd); // throws GameError

    // gauge textures
    if (!dashboardTextures.initialize(graphics, DASHBOARD_TEXTURES))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing dashboard textures"));

    horizontalBar.initialize(graphics, &dashboardTextures, HBAR_X, HBAR_Y, 1.0f, graphicsNS::LIME);

    verticalBar.initialize(graphics, &dashboardTextures, VBAR_X, VBAR_Y, 1.0f, graphicsNS::YELLOW);
    verticalBar.setDegrees(270);

    dial180.initialize(graphics, &dashboardTextures, DIAL180_X, DIAL180_Y, 1.0f, dashboardNS::DIAL180, 0.0f, graphicsNS::BLUE, graphicsNS::YELLOW);
    dial270.initialize(graphics, &dashboardTextures, DIAL270_X, DIAL270_Y, 1.0f, dashboardNS::DIAL270, 0.0f, graphicsNS::BLUE, graphicsNS::YELLOW);
    dial360.initialize(graphics, &dashboardTextures, DIAL360_X, DIAL360_Y, 1.0f, dashboardNS::DIAL360, 0.0f, graphicsNS::BLUE, graphicsNS::YELLOW);

    light.initialize(graphics, &dashboardTextures, LIGHT_X, LIGHT_Y, 0.5f, 1.0f, graphicsNS::LIME, graphicsNS::GREEN);

    sevenSegment.initialize(graphics, &dashboardTextures, DIGITS_X, DIGITS_Y, 0.5f, 8, graphicsNS::RED);

    toggleSwitch.initialize(graphics, &dashboardTextures, input, hwnd, TOGGLE_X, TOGGLE_Y, 1.0f);

    pushButton.initialize(graphics, &dashboardTextures, input, hwnd, PUSH_X, PUSH_Y, 0.5f, true);

    barGraph.initialize(graphics, &dashboardTextures, BAR_GRAPH_X, BAR_GRAPH_Y, 1.0f, 10, graphicsNS::ORANGE);
}


//=============================================================================
// Reset the game to begin play and after a score
//=============================================================================
void DashboardDemo::reset()
{
    return;
}

//=============================================================================
// ���ׂẴQ�[���A�C�e�����ړ�
// frameTime�́A�ړ��̑����𐧌䂷�邽�߂Ɏg�p
//=============================================================================
void DashboardDemo::update()
{
    if(toggleSwitch.getSwitchOn())  // �؂�ւ��X�C�b�`���I���̏ꍇ
    {
        horizontal += frameTime*20; // ���������ɑ���
        vertical += frameTime*30;   // ���������ɑ���
        dial += frameTime*10;       // �~�`���[�^�[�𑝉�
    }
    if(horizontal > 100)            // 100�ɒB�����烊�Z�b�g
        horizontal -= 100;
    if(vertical > 100)
        vertical -= 100;
    if(dial > 100)
        dial -= 100;
    horizontalBar.set(horizontal);  // �����o�[�̃T�C�Y��ݒ�
    verticalBar.set(vertical);      // �����o�[�̃T�C�Y��ݒ�
    dial180.set(dial);              // �~�`���[�^�[�̎w�j�̈ʒu��ݒ�
    dial270.set(dial);
    dial360.set(dial);
    light.update(frameTime);        // �R���g���[�����X�V
    toggleSwitch.update(frameTime);
    pushButton.update(frameTime);
    sevenSegment.set(fps);          // �Z�u���Z�O�����g�Ńt���[�����[�g�ifps�j��\��
    if(pushButton.getSwitchOn())    // �v�b�V���{�^���X�C�b�`���I���̏ꍇ
        vu += frameTime*100;        // vu���ړ�
    if(vu > 100)
        vu -= 100;
    barGraph.set(vu);               // �o�[�O���t��vu�Őݒ�
}

//=============================================================================
// render game items and do end of round logic
//=============================================================================
void DashboardDemo::render()
{
    graphics->spriteBegin();                // begin drawing sprites
    horizontalBar.draw(graphicsNS::FILTER);
    verticalBar.draw(graphicsNS::FILTER);
    dial180.draw(graphicsNS::FILTER);
    dial270.draw(graphicsNS::FILTER);
    dial360.draw(graphicsNS::FILTER);
    light.draw(graphicsNS::FILTER);
    sevenSegment.draw(graphicsNS::FILTER);
    toggleSwitch.draw();
    pushButton.draw();
    barGraph.draw(graphicsNS::FILTER);
    graphics->spriteEnd();                  // end drawing sprites
}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void DashboardDemo::releaseAll()
{
    dashboardTextures.onLostDevice();

    Game::releaseAll();
    return;
}

//=============================================================================
// The grahics device has been reset.
// Recreate all surfaces and reset all entities.
//=============================================================================
void DashboardDemo::resetAll()
{
    dashboardTextures.onResetDevice();

    Game::resetAll();
    return;
}
