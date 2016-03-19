// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Chapter 8 threeCs.h v1.0

#ifndef _THREECS_H              // Prevent multiple definitions if this 
#define _THREECS_H              // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "game.h"
#include "image.h"
#include "text.h"
#include "constants.h"

class ThreeCs : public Game
{
private:
    // �Q�[���A�C�e��
    Text *fontCK;               // �X�v���C�g�x�[�X�̃t�H���g

    // game variables

public:
    ThreeCs();
    virtual ~ThreeCs();
    void initialize(HWND hwnd);
    void reset();
    void update();
    void ai() {};
    void collisions() {};
    void render();
    void releaseAll();
    void resetAll();
};

#endif
