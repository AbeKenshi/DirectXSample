#ifndef _CONSTANTS_H            // prevent multiple definitions if this 
#define _CONSTANTS_H            // ..file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <windows.h>

//-----------------------------------------------
// Useful macros
//-----------------------------------------------
// �|�C���^�Q�Ƃ����A�C�e�������S�ɊJ��
#define SAFE_DELETE(ptr)       { if (ptr) { delete (ptr); (ptr)=NULL; } }
// �|�C���^�Q�Ƃ����A�C�e�������S�ɊJ��
#define SAFE_RELEASE(ptr)      { if(ptr) { (ptr)->Release(); (ptr)=NULL; } }
// Safely delete pointer referenced array
#define SAFE_DELETE_ARRAY(ptr) { if(ptr) { delete [](ptr); (ptr)=NULL; } }

//-----------------------------------------------
//                  Constants
//-----------------------------------------------

// window
const char CLASS_NAME[] = "Spacewar";
const char GAME_TITLE[] = "Game Engine";
const bool FULLSCREEN = true;              // windowed or fullscreen
const UINT  GAME_WIDTH = 640;              // width of game in pixels
const UINT  GAME_HEIGHT = 480;              // height of game in pixels

											// key mappings
											// In this game simple constants are used for key mappings. If variables were used
											// it would be possible to save and restore key mappings from a data file.
const byte ESC_KEY = VK_ESCAPE;         // escape key

#endif
