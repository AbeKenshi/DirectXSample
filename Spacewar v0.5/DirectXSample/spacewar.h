#ifndef _SPACEWAR_H             // ���̃t�@�C���������̉ӏ��ŃC���N���[�h�����ꍇ�ɁA
#define _SPACEWAR_H             // ���d�ɒ�`����邱�Ƃ�h���܂��B
#define WIN32_LEAN_AND_MEAN

#include <string>
#include "game.h"
#include "textureManager.h"
#include "image.h"
#include "dashboard.h"
#include "planet.h"
#include "ship.h"
#include "torpedo.h"

namespace spacewarNS
{
	const char FONT[] = "Arial Bold";	// �t�H���g
	const int FONT_BIG_SIZE = 256;		// �t�H���g�̍���
	const int FONT_SCORE_SIZE = 48;
	const COLOR_ARGB FONT_COLOR = graphicsNS::YELLOW;
	const COLOR_ARGB SHIP1_COLOR = graphicsNS::BLUE;
	const COLOR_ARGB SHIP2_COLOR = graphicsNS::YELLOW;
	const int SCORE_Y = 10;
	const int SCORE1_X = 60;
	const int SCORE2_X = GAME_WIDTH - 80;
	const int HEALTHBAR_Y = 30;
	const int SHIP1_HEALTHBAR_X = 40;
	const int SHIP2_HEALTHBAR_X = GAME_WIDTH - 100;
	const int COUNT_DOWN_X = GAME_WIDTH / 2 - FONT_BIG_SIZE / 4;
	const int COUNT_DOWN_Y = GAME_HEIGHT / 2 - FONT_BIG_SIZE / 2;
	const int COUNT_DOWN = 5;           // 5����J�E���g�_�E��
	const int BUF_SIZE = 20;
	const int ROUND_TIME = 5;           // �V�������E���h���J�n����܂ł̎���
}

//=============================================================================
// Game�N���X���p������Spacewar�N���X���쐬
//=============================================================================
class Spacewar : public Game
{
private:
	// �Q�[���A�C�e��
	TextureManager menuTexture;		// �^�C�g���̃e�N�X�`��
	TextureManager nebulaTexture;	// ���_�inebula�j�̃e�N�X�`��
	TextureManager gameTextures;	// �Q�[���Ɏg�p����e�N�X�`���Q
	Ship ship1, ship2;				// �F���D
	Torpedo torpedo1, torpedo2;		// �~�T�C��
	Planet planet;					// �f���iplanet�j
	Image nebula;					// ���_�inebula�j�̉摜
	Image   menu;					// ���j���[�̉摜
	Bar     healthBar;				// �F���D�̗̑̓o�[
	TextDX  fontBig;				// �Q�[���o�i�[��DirectX�t�H���g
	TextDX  fontScore;
	bool    menuOn;
	bool    countDownOn;			// �J�E���g�_�E�����\������Ă���ꍇ�Atrue
	float   countDownTimer;
	char buffer[spacewarNS::BUF_SIZE];
	// ���E���h���ŉF���D�̃X�R�A���v�Z�����ꍇ�Atrue
	bool    ship1Scored, ship2Scored;
	bool    roundOver;				// ���E���h���I�������ꍇ�Atrue
	float   roundTimer;				// �V�������E���h���J�n����܂ł̎���
	int     ship1Score, ship2Score; // �X�R�A

public:
	// �R���X�g���N�^
	Spacewar();

	// �f�X�g���N�^
	virtual ~Spacewar();

	// �Q�[����������
	void initialize(HWND hwnd);
	void update();			// Game����̏������z�֐����I�[�o�[���C�h����K�v������
	void ai();				// "
	void collisions();		// "
	void render();			// "
	void consoleCommand();	// �R���\�[���R�}���h������
	void roundStart();		// �v���C�̐V�������E���h���J�n
	void releaseAll();
	void resetAll();
};

#endif
