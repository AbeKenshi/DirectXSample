// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// audio.h v1.0

#ifndef _AUDIO_H                // Prevent multiple definitions if this 
#define _AUDIO_H                // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <xact3.h>
#include "constants.h"

class Audio
{
    // Audio�v���p�e�B
  private:
    IXACT3Engine* xactEngine;   // XACT�G���W���ւ̃|�C���^
    IXACT3WaveBank* waveBank;   // XACT�E�F�C�u�o���N�ւ̃|�C���^
    IXACT3SoundBank* soundBank; // XACT�T�E���h�o���N�ւ̃|�C���^
    XACTINDEX cueI;             // XACT�T�E���h�C���f�b�N�X
    void* mapWaveBank;          // UnmapViewOfFile()���Ăяo���ăt�@�C�������
    void* soundBankData;
    bool coInitialized;         // CoInitializeEx�����������ꍇ�Atrue��ݒ�

  public:
	// �R���X�g���N�^
    Audio();

    // �f�X�g���N�^
    virtual ~Audio();

    // �����o�[�֐�
    // Audio��������
    HRESULT initialize();

    // �T�E���h�G���W���̎����I�^�X�N�����s
    void run();

	// �L���[�Ŏw�肳�ꂽ�T�E���h���T�E���h�o���N����Đ�
	// �L���[�����݂��Ȃ��ꍇ�́A�T�E���h���Đ�����Ȃ������ŁA�G���[�͔������Ȃ�
    void playCue(const char cue[]);

	// �L���[�Ŏw�肳�ꂽ�T�E���h���T�E���h�o���N�Œ�~
	// �L���[�����݂��Ȃ��ꍇ�A�G���[�͔������Ȃ�
    void stopCue(const char cue[]);
};

#endif
