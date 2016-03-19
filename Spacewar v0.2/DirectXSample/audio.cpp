// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// audio.cpp v1.0

#include "audio.h"

//=============================================================================
// default constructor
//=============================================================================
Audio::Audio()
{
    xactEngine = NULL;
    waveBank = NULL;
    soundBank = NULL;
    cueI = 0;
    mapWaveBank = NULL;         // Call UnmapViewOfFile() to release file
    soundBankData = NULL;

    HRESULT hr = CoInitializeEx( NULL, COINIT_MULTITHREADED );
    if( SUCCEEDED( hr ) )
        coInitialized = true;
    else
        coInitialized = false;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
Audio::~Audio()
{
    // XACT���V���b�g�_�E��
    if( xactEngine )
    {
		// XACT�G���W�����V���b�g�_�E�����A���\�[�X�����
        xactEngine->ShutDown();
        xactEngine->Release();
    }

    if ( soundBankData )
        delete[] soundBankData;
    soundBankData = NULL;

	// xactEngine()->ShutDown()����߂�����A�������}�b�v�g�t�@�C�������
    if( mapWaveBank )
        UnmapViewOfFile( mapWaveBank );
    mapWaveBank = NULL;

    if( coInitialized )        // CoInitializeEx�����������ꍇ
        CoUninitialize();
}

//=============================================================================
// initialize
// This function does the following:
//      1. Initialize XACT by calling xactEngine->Initialize 
//      2. Create the XACT wave bank(s) you want to use
//      3. Create the XACT sound bank(s) you want to use
//      4. Store indices to the XACT cue(s) your game uses
//=============================================================================
HRESULT Audio::initialize()
{
    HRESULT result = E_FAIL;
    HANDLE hFile;
    DWORD fileSize;
    DWORD bytesRead;
    HANDLE hMapFile;

    if( coInitialized == false)
        return E_FAIL;

    result = XACT3CreateEngine( 0, &xactEngine );
    if( FAILED( result ) || xactEngine == NULL )
        return E_FAIL;

    // XACT�����^�C���̏������ƍ쐬
    XACT_RUNTIME_PARAMETERS xactParams = {0};
    xactParams.lookAheadTime = XACT_ENGINE_LOOKAHEAD_DEFAULT;
    result = xactEngine->Initialize( &xactParams );
    if( FAILED( result ) )
        return result;

    // �������}�b�v�ƃt�@�C��IO���g�p����u�C���������v��XACT�E�F�C�u�o���N�t�@�C�����쐬
    result = E_FAIL; // ���s���f�t�H���g�Ƃ��āA�������ɒu������
    hFile = CreateFile( WAVE_BANK, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL );
    if( hFile != INVALID_HANDLE_VALUE )
    {
        fileSize = GetFileSize( hFile, NULL );
        if( fileSize != -1 )
        {
            hMapFile = CreateFileMapping( hFile, NULL, PAGE_READONLY, 0, fileSize, NULL );
            if( hMapFile )
            {
                mapWaveBank = MapViewOfFile( hMapFile, FILE_MAP_READ, 0, 0, 0 );
                if( mapWaveBank )
                    result = xactEngine->CreateInMemoryWaveBank( mapWaveBank, fileSize, 0, 0, &waveBank );

				// mapWaveBank���t�@�C���ւ̃n���h����ێ�
				// �s�v�ȃn���h���͕���
                CloseHandle( hMapFile );
            }
        }
		// mapWaveBank���t�@�C���ւ̃n���h����ێ����Ă���̂ŁA
		// �s�v�ȃn���h���͕���
        CloseHandle( hFile );
    }
    if( FAILED( result ) )
        return HRESULT_FROM_WIN32( ERROR_FILE_NOT_FOUND );

	// XACT�ō쐬�����T�E���h�o���N�t�@�C����ǂݎ��A�o�^
    result = E_FAIL;    // ���s���f�t�H���g�Ƃ��āA�������ɒu������
    hFile = CreateFile( SOUND_BANK, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL );
    if( hFile != INVALID_HANDLE_VALUE )
    {
        fileSize = GetFileSize( hFile, NULL );
        if( fileSize != -1 )
        {
			// �T�E���h�o���N�p�̃�������\��
            soundBankData = new BYTE[fileSize];
            if( soundBankData )
            {
                if( 0 != ReadFile( hFile, soundBankData, fileSize, &bytesRead, NULL ) )
                    result = xactEngine->CreateSoundBank( soundBankData, fileSize, 0, 0, &soundBank );
            }
        }
        CloseHandle( hFile );
    }
    if( FAILED( result ) )
        return HRESULT_FROM_WIN32( ERROR_FILE_NOT_FOUND );

    return S_OK;
}

//=============================================================================
// �T�E���h�G���W���̎����I�^�X�N�����s
//=============================================================================
void Audio::run()
{
    if (xactEngine == NULL)
        return;
    xactEngine->DoWork();
}

//=============================================================================
// �L���[�Ŏw�肳�ꂽ�T�E���h���T�E���h�o���N����Đ�
// �L���[�����݂��Ȃ��ꍇ�́A�T�E���h���Đ�����Ȃ������ŁA�G���[�͔������Ȃ�
//=============================================================================
void Audio::playCue(const char cue[])
{
    if (soundBank == NULL)
        return;
	// �T�E���h�o���N����L���[�C���f�b�N�X���擾
    cueI = soundBank->GetCueIndex( cue );
    soundBank->Play( cueI, 0, 0, NULL );
}

//=============================================================================
// �L���[�Ŏw�肳�ꂽ�T�E���h���T�E���h�o���N�Œ�~
// �L���[�����ݏꍇ�A�G���[�͔������Ȃ�
//=============================================================================
void Audio::stopCue(const char cue[])
{
    if (soundBank == NULL)
        return;
	// �T�E���h�o���N����L���[�C���f�b�N�X���擾
    cueI = soundBank->GetCueIndex( cue );
    soundBank->Stop( cueI, XACT_FLAG_SOUNDBANK_STOP_IMMEDIATE);
}
