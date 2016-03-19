#ifndef _ENTITY_H             // ���̃t�@�C���������̉ӏ��ŃC���N���[�h�����ꍇ�ɁA
#define _ENTITY_H             // ���d�ɒ�`����邱�Ƃ�h���܂��B
#define WIN32_LEAN_AND_MEAN

#include "image.h"
#include "input.h"
#include "game.h"
#include "audio.h"

namespace entityNS
{
	enum COLLISION_TYPE {
		NONE,
		CIRCLE,
		BOX,
		ROTATED_BOX
	};
	const float GRAVITY = 6.67428e-11f;
}

//=============================================================================
//=============================================================================
class Entity : public Image
{
protected:
	// Entity�v���p�e�B
	entityNS::COLLISION_TYPE collisionType;		// �Փ˂̎��
	VECTOR2 center;								// �G���e�B�e�B�̒��S
	float radius;								// �~�̏Փ˔���Ɏg�p����A�~�̔��a
	VECTOR2 distSquared;						// �~�̏Փ˔���Ōv�Z�����x�N�g��
	float sumRadiiSquared;
	RECT edge;									// BOX�܂���ROTATED_BOX�̏Փ˔��肪�g�p����A��`�̈�
	VECTOR2 corners[4];							// ���E�{�b�N�X�̒��_
	VECTOR2 edge01, edge03;						// ���e��
	float   edge01Min, edge01Max, edge03Min, edge03Max; // ���e�̍ŏ��l�ƍő�l
	VECTOR2 velocity;							// ���݂̃G���e�B�e�B�̑��x���܂ރx�N�g��
	VECTOR2 deltaV;								// 
	float mass;									// �G���e�B�e�B�̎���
	float health;								// �G���e�B�e�B�̗̑́i0����100�j
	float rr;									// Radius squared variable
	float force;								// Force of gravity
	float gravity;								// 
	Input *input;								// ���̓V�X�e���ւ̃|�C���^
	Audio *audio;								// �I�[�f�B�I�V�X�e���ւ̃|�C���^
	HRESULT hr;									// �W���̖߂�l
	bool active;								// �G���e�B�e�B�̌��݂̏�ԁB�A�N�e�B�u�ȃG���e�B�e�B�݂̂��Փ˂��N�����܂��B
	bool rotatedBoxReady;						// ROTATED_BOX�̏Փ˔���̂Ƃ��Atrue

	// �~�̏Փ˔��胁�\�b�h
	// collision()�ɂ���ČĂяo�����A�f�t�H���g�̏Փ˔��胁�\�b�h
	// ���s��F�Փ˂��Ă���ꍇ��true�A���Ă��Ȃ��ꍇ��false��߂�
	// �Փ˂��Ă���ꍇ�́AcollisionVector��ݒ�
	virtual bool collideCircle(Entity &ent, VECTOR2 &collisionVector);

	// �����s���E�{�b�N�X�̏Փ˔��胁�\�b�h
	// collision()�ɂ���ČĂяo�����
	// ���s��F�Փ˂��Ă���ꍇ��true�A���Ă��Ȃ��ꍇ��false��Ԃ�
	// �Փ˂��Ă���ꍇ�́AcollisionVector��ݒ�
	virtual bool collideBox(Entity &ent, VECTOR2 &collisionVector);

	// ��]����{�b�N�X�̏Փ˔��胁�\�b�h
	// collision()�ɂ���ČĂяo�����
	// ���s��F�Փ˂��Ă���ꍇ��true�A���Ă��Ȃ��ꍇ��false��Ԃ�
	// �Փ˂��Ă���ꍇ�́AcollisionVector��ݒ�
	// �������e�X�g���g�p���ďՓ˂𔻒�
	// �������e�X�g�F
	// 2�̃{�b�N�X�́A����̒����ւ̎ˉe���d�Ȃ��Ă��Ȃ���ΏՓ˂��Ă��Ȃ�
	virtual bool collideRotatedBox(Entity &ent, VECTOR2 &collisionVector);

	// ��]����{�b�N�X�Ɖ~�̏Փ˔��胁�\�b�h
	// collision()�ɂ���ČĂяo�����
	// �{�b�N�X�̕ӂƉ~�̔��a�ŕ������e�X�g���g�p
	// �~�̒��S���Փ˃{�b�N�X�̊e�ӂ�L�΂��������̊O��
	// �i�{���m�C�̈�ƌĂ΂��j�ɂ���ꍇ�A�{�b�N�X�̍ł��߂����_��
	// �Փ˂��Ȃ����������̃`�F�b�N�Ŕ��肵�܂��B
	// �ł��߂����_�́A�d�Ȃ�̃e�X�g���画�f�ł��܂��B
	//
	//   Voronoi0 |   | Voronoi1
	//         ---0---1---
	//            |   |
	//         ---3---2---
	//   Voronoi3 |   | Voronoi2
	//
	// ���s�O�F���̃G���e�B�e�B�͕K���{�b�N�X�A����̃G���e�B�e�B�ient�j�͕K���~
	// ���s��F�Փ˂��Ă���ꍇ��true�A���Ă��Ȃ��ꍇ��false��߂�
	// �Փ˂��Ă���ꍇ�́AcollisionVector��ݒ�
	virtual bool collideRotatedBoxCircle(Entity &ent, VECTOR2 &collisionVector);
	
	// ��]����{�b�N�X�̒��_�A���e���A���e�̍ŏ��l�ƍő�l���v�Z
	// 0---1  ���_�̔ԍ�
	// |   |
	// 3---2
	void computeRotatedBox();

	// ����̃{�b�N�X���A���̃G���e�B�e�B��edge01�����edge03�ɓ��e
	// collideRotateBox()�ɂ���ČĂяo�����
	// ���s��F���e���d�Ȃ��Ă���ꍇ��true�A����ȊO�̏ꍇ��false��߂�
	bool projectionsOverlap(Entity &ent);

	// �{�b�N�X�̒��_���~�ƏՓ˂��Ă��Ȃ������A�����̃`�F�b�N���g�p���Ĕ���
	// collideRotateBoxCircle()�ɂ���ČĂяo�����
	// ���s��F�Փˎw��ꍇ��true�A���Ă��Ȃ��ꍇ��false��߂�
	// �Փ˂��Ă���ꍇ�́AcollisionVector��ݒ�
	bool collideCornerCircle(VECTOR2 corner, Entity &ent, VECTOR2 &collisionVector);

public:
	// �R���X�g���N�^
	Entity();

	// �f�X�g���N�^
	virtual ~Entity() {}

	// �G���e�B�e�B�̉�ʏ�ł̒��S���W��߂�
	virtual const VECTOR2* getCenter()
	{
		center = VECTOR2(getCenterX(), getCenterY());
		return &center;
	}

	// �~�̏Փ˔���Ɏg�p����~�̔��a��߂�
	virtual float getRadius() const
	{
		return radius;
	}

	// BOX�܂���ROTATED_BOX�̏Փ˔��肪�g�p����A��`�̈��߂�
	virtual const RECT& getEdge() const
	{
		return edge;
	}

	// Return corner c of ROTATED_BOX
	virtual const VECTOR2* getCorner(UINT c) const
	{
		if (c >= 4)
			c = 0;
		return &corners[c];
	}

	// Return velocity vector.
	virtual const VECTOR2 getVelocity() const { return velocity; }

	// Return active.
	virtual bool  getActive()         const { return active; }

	// Return mass.
	virtual float getMass()           const { return mass; }

	// Return gravitational constant.
	virtual float getGravity()        const { return gravity; }

	// Return health;
	virtual float getHealth()         const { return health; }

	// Return collision type (NONE, CIRCLE, BOX, ROTATED_BOX)
	virtual entityNS::COLLISION_TYPE getCollisionType() { return collisionType; }

	////////////////////////////////////////
	//           Set functions            //
	////////////////////////////////////////

	// Set velocity.
	virtual void  setVelocity(VECTOR2 v) { velocity = v; }

	// Set delta velocity. Added to velocity in update().
	virtual void  setDeltaV(VECTOR2 dv) { deltaV = dv; }

	// Set active.
	virtual void  setActive(bool a) { active = a; }

	// Set health.
	virtual void setHealth(float h) { health = h; }

	// Set mass.
	virtual void  setMass(float m) { mass = m; }

	// Set gravitational constant. Default is 6.67428e-11
	virtual void  setGravity(float g) { gravity = g; }

	// Set radius of collision circle.
	virtual void setCollisionRadius(float r) { radius = r; }

	////////////////////////////////////////
	//         Other functions            //
	////////////////////////////////////////

	// Update Entity.
	// typically called once per frame
	// frameTime is used to regulate the speed of movement and animation
	virtual void update(float frameTime);

	// Entity��������
	// ���s�O�F*gamePtr = Game�I�u�W�F�N�g�ւ̃|�C���^
	//		   width = Image�̕��i�s�N�Z���P�ʁj�i0 = �e�N�X�`���S�̂̕����g�p�j
	//		   height = Image�̍����i�s�N�Z���P�ʁj�i0 = �e�N�X�`���S�̂̍������g�p�j
	//		   ncols = �e�N�X�`�����̗񐔁i1����n�܂Łj�i0��1�Ɠ����j
	//		   *textureM = TextureManager�I�u�W�F�N�g�ւ̃|�C���^
	// ���s��F���������ꍇ��true�A���s�����ꍇ��false��߂�
	virtual bool initialize(Game *gamePtr, int width, int height, int ncols,
		TextureManager *textureM);

	// �G���e�B�e�B���A�N�e�B�u��
	virtual void activate();

	// Empty ai function to allow Entity objects to be instantiated.
	virtual void ai(float frameTime, Entity &ent);

	// Is this entity outside the specified rectangle?
	virtual bool outsideRect(RECT rect);

	// Does this entity collide with ent?
	virtual bool collidesWith(Entity &ent, VECTOR2 &collisionVector);

	// Damage this Entity with weapon.
	virtual void damage(int weapon);

	// Entity bounces after collision with other Entity
	void bounce(VECTOR2 &collisionVector, Entity &ent);

	// Adds the gravitational force to the velocity vector of this entity
	void gravityForce(Entity *other, float frameTime);

};

#endif