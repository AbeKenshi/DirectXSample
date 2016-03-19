// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// flowerPower.cpp v1.0
// flowerPower is the class we create.

#include "flowerPower.h"
using namespace flowerPowerNS;

//=============================================================================
// Constructor
//=============================================================================
FlowerPower::FlowerPower()
{
    mapX = 0;
    menuOn = true;
}

//=============================================================================
// Destructor
//=============================================================================
FlowerPower::~FlowerPower()
{
    releaseAll();           // call onLostDevice() for every graphics item
}

//=============================================================================
// initializes the game
//=============================================================================
void FlowerPower::initialize(HWND hwnd)
{
    Game::initialize(hwnd);

    // menu texture
    if (!menuTexture.initialize(graphics,MENU_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu texture"));

    // map textures
    if (!tileTextures.initialize(graphics,TILE_TEXTURES))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing tile textures"));

    // object textures
    if (!objectTextures.initialize(graphics,OBJECT_TEXTURES))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing object textures"));

    // menu image
    if (!menu.initialize(graphics,0,0,0,&menuTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu"));

    // tile image
    if (!tile.initialize(graphics,TEXTURE_SIZE,TEXTURE_SIZE,TEXTURE_COLS,&tileTextures))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing tile"));
    tile.setFrames(0, 0);
    tile.setCurrentFrame(0);

    // butterfly
    if (!butterfly.initialize(this,butterflyNS::WIDTH,butterflyNS::HEIGHT,butterflyNS::TEXTURE_COLS,&objectTextures))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing butterfly"));
    butterfly.setFrames(butterflyNS::FLYING_START_FRAME, butterflyNS::FLYING_END_FRAME);
    butterfly.setCurrentFrame(butterflyNS::FLYING_START_FRAME);
    butterfly.setColorFilter(SETCOLOR_ARGB(255,230,230,255));   // light blue, used for shield and torpedo
    butterfly.setMass(butterflyNS::MASS);
    // Start butterfly in center
    butterfly.setX(GAME_WIDTH/2 - butterflyNS::WIDTH);
    butterfly.setY(GAME_HEIGHT/2 - butterflyNS::HEIGHT);

    return;
}

//=============================================================================
// ���ׂẴQ�[���A�C�e�����X�V
//=============================================================================
void FlowerPower::update()
{
    float butterflyX;

    if (menuOn)
    {
        if (input->anyKeyPressed())
        {
            menuOn = false;
            input->clearAll();
        }
    } 

    butterfly.update(frameTime);        // �`���E���΂�

    butterflyX = butterfly.getX();
    if(butterflyX < 0)                  // �`���E����ʂ̍��ɂ͂ݏo���ꍇ
    {
		// �}�b�v���E�ɃX�N���[��
        mapX -= butterfly.getVelocity().x * frameTime;
        butterfly.setX(0);              // �`���E�����[�ɔz�u
    }
	// �`���E����ʂ̉E�ɂ͂ݏo���ꍇ
    else if(butterflyX > GAME_WIDTH - butterfly.getWidth())
    {
		// �}�b�v�����ɃX�N���[��
        mapX -= butterfly.getVelocity().x * frameTime;
        // �`���E���E�[�ɔz�u
        butterfly.setX((float)(GAME_WIDTH - butterfly.getWidth()));
    }

    if(mapX > 0)    // �}�b�v�����[�𒴂���ꍇ
    {
        mapX = 0;   // �}�b�v�̍��[�Œ�~
        butterfly.setVelocityX(0);  // �`���E���~�߂�
    }
	// �}�b�v���E�[�𒴂���ꍇ
    else if(mapX < (-MAP_WIDTH * TEXTURE_SIZE) + (int)GAME_WIDTH)
    {
		// �}�b�v�̉E�[�Œ�~
        mapX = (-MAP_WIDTH * TEXTURE_SIZE) + (int)GAME_WIDTH;
        butterfly.setVelocityX(0);  // �`���E���~�߂�
    }
}

//=============================================================================
// Artificial Intelligence
//=============================================================================
void FlowerPower::ai()
{}

//=============================================================================
// Handle collisions
//=============================================================================
void FlowerPower::collisions()
{}

//=============================================================================
// �Q�[���A�C�e���������_�[
//=============================================================================
void FlowerPower::render()
{
    graphics->spriteBegin();

    for(int row=0; row<MAP_HEIGHT; row++)       // �}�b�v�̊e�s������
    {
        tile.setY( (float)(row*TEXTURE_SIZE) ); // �^�C����Y��ݒ�
        for(int col=0; col<MAP_WIDTH; col++)    // �}�b�v�̊e�������
        {
            if(tileMap[row][col] >= 0)          // �^�C�������݂���ꍇ
            {
				// �^�C���e�N�X�`����ݒ�
                tile.setCurrentFrame(tileMap[row][col]);
				// �^�C����X��ݒ�
                tile.setX( (float)(col*TEXTURE_SIZE) + mapX );
                // �^�C������ʏ�ɂ��邩�ǂ���
                if(tile.getX() > -TEXTURE_SIZE && tile.getX() < GAME_WIDTH)
                    tile.draw();                // �^�C����`��
            }
        }
    }

	// �`���E��`��
    butterfly.draw();

    if(menuOn)
        menu.draw();

    graphics->spriteEnd();
}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void FlowerPower::releaseAll()
{
    menuTexture.onLostDevice();
    tileTextures.onLostDevice();
    objectTextures.onLostDevice();

    Game::releaseAll();
    return;
}

//=============================================================================
// The grahics device has been reset.
// Recreate all surfaces.
//=============================================================================
void FlowerPower::resetAll()
{
    tileTextures.onResetDevice();
    objectTextures.onResetDevice();
    menuTexture.onResetDevice();

    Game::resetAll();
    return;
}
