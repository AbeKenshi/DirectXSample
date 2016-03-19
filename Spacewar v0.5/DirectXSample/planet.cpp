// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// planet.cpp v1.0

#include "planet.h"

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
Planet::Planet() : Entity()
{
	spriteData.x = planetNS::X;              // ��ʏ�̈ʒu
	spriteData.y = planetNS::Y;
	radius = planetNS::COLLISION_RADIUS;
	mass = planetNS::MASS;
	startFrame = planetNS::START_FRAME;		// �f���A�j���[�V�����̍ŏ��̃t���[��
	endFrame = planetNS::END_FRAME;			// �f���A�j���[�V�����̍ŏ��̃t���[��
	setCurrentFrame(startFrame);
}