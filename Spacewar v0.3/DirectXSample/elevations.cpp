// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Elevations.cpp Isometric Elevation Demo v1.0
// Elevations is the class we create.

#include "Elevations.h"
using namespace elevationsNS;

//=============================================================================
// Constructor
//=============================================================================
Elevations::Elevations()
{}

//=============================================================================
// Destructor
//=============================================================================
Elevations::~Elevations()
{
    releaseAll();           // call onLostDevice() for every graphics item
}

//=============================================================================
// initializes the game
// Throws GameError on error
//=============================================================================
void Elevations::initialize(HWND hwnd)
{
    Game::initialize(hwnd);

    // map textures
    if (!textures.initialize(graphics,ELEVATION1_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing textures"));

    // object textures
    if (!textures2.initialize(graphics,ELEVATION2_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing textures2"));

    // map tile image
    mapTile.initialize(graphics,TEXTURE_SIZE,TEXTURE_SIZE,TEXTURE_COLS,&textures);
    mapTile.setFrames(0, 0);
    mapTile.setCurrentFrame(0);

    //// Tree image
    tree.initialize(graphics,TEXTURE2_SIZE,TEXTURE2_SIZE,TEXTURE2_COLS,&textures2);
    tree.setFrames(TREE0_FRAME, TREE0_FRAME);
    tree.setCurrentFrame(TREE0_FRAME);
}

//=============================================================================
// Update all game items
//=============================================================================
void Elevations::update()
{
    mapTile.update(frameTime);
}

//=============================================================================
// Artificial Intelligence
//=============================================================================
void Elevations::ai()
{}

//=============================================================================
// Handle collisions
//=============================================================================
void Elevations::collisions()
{}

//=============================================================================
// �Q�[���A�C�e���������_�[
//=============================================================================
void Elevations::render()
{
    graphics->spriteBegin();

	// ���p���e�̂Ђ��`�Ƀ}�b�v��`��
    for(int row=0; row<MAP_SIZE; row++)
    {
        for(int col=0; col<MAP_SIZE; col++)
        {
            mapTile.setCurrentFrame(tileMap[row][col]);
            mapTile.setX((float)( SCREEN_X - (row*TEXTURE_SIZE/2) + (col*TEXTURE_SIZE/2) ));
            mapTile.setY((float)( SCREEN_Y + (row*TEXTURE_SIZE/4) + (col*TEXTURE_SIZE/4) -
                                  heightMap[row][col] * HEIGHT_CHANGE));
            mapTile.draw();
        }
    }

	// �I�u�W�F�N�g�̕`��A0=��A1=��0�A2=��1
    float treeX = 0, treeY = 0;
    for(int row=0; row<MAP_SIZE; row++)
    {
        for(int col=0; col<MAP_SIZE; col++)
        {
            switch(objectMap[row][col])
            {
            case 1:     // ��0
                tree.setX((float)( SCREEN_X - (row*TEXTURE_SIZE/2) + (col*TEXTURE_SIZE/2) ) +
                                    TREE_OFFSET_X);
                tree.setY((float)( SCREEN_Y + (row*TEXTURE_SIZE/4) + (col*TEXTURE_SIZE/4) -
                                   heightMap[row][col] * HEIGHT_CHANGE) + TREE_OFFSET_Y);
                if(col%2)
                    tree.flipHorizontal(true);
                // �e��`��
                tree.setCurrentFrame(TREE0_SHADOW);
                tree.setDegrees(TREE_SHADOW_DEGREES);
                treeX = tree.getX();
                treeY = tree.getY();
                tree.setX(treeX + TREE_SHADOW_X);
                tree.setY(treeY + TREE_SHADOW_Y);
                tree.draw(graphicsNS::ALPHA25 & graphicsNS::BLACK);
                tree.setX(treeX);   // X�𕜌�
                tree.setY(treeY);   // Y�𕜌�
                // �؂�`��
                tree.setDegrees(0);
                tree.setCurrentFrame(TREE0_FRAME);
                tree.draw();
                tree.flipHorizontal(false);
                break;
            case 2:     // ��1
                tree.setX((float)( SCREEN_X - (row*TEXTURE_SIZE/2) + (col*TEXTURE_SIZE/2) ) +
                                    TREE_OFFSET_X);
                tree.setY((float)( SCREEN_Y + (row*TEXTURE_SIZE/4) + (col*TEXTURE_SIZE/4) -
                                   heightMap[row][col] * HEIGHT_CHANGE) + TREE_OFFSET_Y);
                if(col%2)
                    tree.flipHorizontal(true);
                // �e��`��
                tree.setCurrentFrame(TREE1_SHADOW);
                tree.setDegrees(TREE_SHADOW_DEGREES);   // �e����]
                treeX = tree.getX();                    // �؂�x�Ay��ۑ�
                treeY = tree.getY();
                tree.setX(treeX + TREE_SHADOW_X);       // �e��z�u
                tree.setY(treeY + TREE_SHADOW_Y);
				// �e��`��
                tree.draw(graphicsNS::ALPHA25 & graphicsNS::BLACK);
                tree.setX(treeX);                       // �؂�x�Ay�𕜌�
                tree.setY(treeY);
                // �؂�`��
                tree.setDegrees(0);
                tree.setCurrentFrame(TREE1_FRAME);
                tree.draw();
                tree.flipHorizontal(false);
                break;
            }
        }
    }

    graphics->spriteEnd();
}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void Elevations::releaseAll()
{
    textures.onLostDevice();

    Game::releaseAll();
    return;
}

//=============================================================================
// The grahics device has been reset.
// Recreate all surfaces.
//=============================================================================
void Elevations::resetAll()
{
    textures.onResetDevice();

    Game::resetAll();
    return;
}
