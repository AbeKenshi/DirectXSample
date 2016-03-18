#ifndef _SPACEWAR_H             // ���̃t�@�C���������̉ӏ��ŃC���N���[�h�����ꍇ�ɁA
#define _SPACEWAR_H             // ���d�ɒ�`����邱�Ƃ�h���܂��B
#define WIN32_LEAN_AND_MEAN

#include "game.h"
#include "textureManager.h"
#include "image.h"
#include "planet.h"
#include "ship.h"

namespace spacewarNS
{
	const char FONT[] = "Arial Bold";  // font
	const int FONT_BIG_SIZE = 256;     // font height
	const COLOR_ARGB FONT_COLOR = graphicsNS::YELLOW;
	const COLOR_ARGB SHIP1_COLOR = graphicsNS::BLUE;
	const int COUNT_DOWN_X = GAME_WIDTH / 2 - FONT_BIG_SIZE / 4;
	const int COUNT_DOWN_Y = GAME_HEIGHT / 2 - FONT_BIG_SIZE / 2;
	const int COUNT_DOWN = 2;               // count down
	const int BUF_SIZE = 20;
	const int ROUND_TIME = 5;               // time until new round starts
}

//=============================================================================
// ���̃N���X�̓Q�[���G���W���̃R�A
//=============================================================================
class Spacewar : public Game
{
private:
	// �Q�[���A�C�e��
	TextureManager nebulaTexture;	// ���_�inebula�j�̃e�N�X�`��
	TextureManager gameTextures;	// �Q�[���̃e�N�X�`��
	TextureManager menuTexture, spaceTexture, textures1;	// �e�N�X�`��
	Ship ship1, ship2;				// �F���D
	Planet planet;					// �f��
	Image nebula;					// ���_�inebula�j�̉摜
	Image space;					// �w�i�摜
	Image moons[4];					// ��
	Image menu;						// ���j���[
	bool    menuOn;
	bool    countDownOn;        // true when count down is displayed
	float   countDownTimer;
	TextDX  fontBig;            // DirectX font for game banners
	char buffer[spacewarNS::BUF_SIZE];
	bool    roundOver;          // true when round is over
	float   roundTimer;         // time until new round starts


public:
	// �R���X�g���N�^
	Spacewar();

	// �f�X�g���N�^
	virtual ~Spacewar();

	// �Q�[����������
	void initialize(HWND hwnd);
	void update();      // Game����̏������z�֐����I�[�o�[���C�h����K�v������
	void ai();          // "
	void collisions();  // "
	void render();      // "
	void roundStart();  // start a new round of play
	void releaseAll();
	void resetAll();
};

#endif
