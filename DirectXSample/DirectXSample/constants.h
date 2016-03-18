// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Chapter 6 constants.h v1.0

#ifndef _CONSTANTS_H            // Prevent multiple definitions if this 
#define _CONSTANTS_H            // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <windows.h>

//-----------------------------------------------
// Useful macros
//-----------------------------------------------
// Safely delete pointer referenced item
#define SAFE_DELETE(ptr)       { if (ptr) { delete (ptr); (ptr)=NULL; } }
// Safely release pointer referenced item
#define SAFE_RELEASE(ptr)      { if(ptr) { (ptr)->Release(); (ptr)=NULL; } }
// Safely delete pointer referenced array
#define SAFE_DELETE_ARRAY(ptr) { if(ptr) { delete [](ptr); (ptr)=NULL; } }
// Safely call onLostDevice
#define SAFE_ON_LOST_DEVICE(ptr)    { if(ptr) { ptr->onLostDevice(); } }
// Safely call onResetDevice
#define SAFE_ON_RESET_DEVICE(ptr)   { if(ptr) { ptr->onResetDevice(); } }
#define TRANSCOLOR  SETCOLOR_ARGB(0,255,0,255)  // transparent color (magenta)

//-----------------------------------------------
//                  Constants
//-----------------------------------------------

// window
const char CLASS_NAME[] = "Spacewar";
const char GAME_TITLE[] = "Spacewar";
const bool FULLSCREEN = false;              // windowed or fullscreen
const UINT GAME_WIDTH = 640;               // width of game in pixels
const UINT GAME_HEIGHT = 480;               // height of game in pixels

// game
const double PI = 3.14159265;
const float FRAME_RATE = 200.0f;                // the target frame rate (frames/sec)
const float MIN_FRAME_RATE = 10.0f;             // the minimum frame rate
const float MIN_FRAME_TIME = 1.0f / FRAME_RATE;   // minimum desired time for 1 frame
const float MAX_FRAME_TIME = 1.0f / MIN_FRAME_RATE; // maximum time used in calculations
const float GRAVITY = 6.67428e-11f;             // gravitational constant
const float MASS_PLANET = 1.0e14f;
const float MASS_SHIP = 5.0f;
const float FULL_HEALTH = 100;
const float SPACE_SCALE = 2.0f;                 // scale factor of space image
const int   SPACE_WIDTH = (int)(640 * SPACE_SCALE);  // width of scaled space image
const int   SPACE_HEIGHT = (int)(480 * SPACE_SCALE); // height of scaled space image
const int   PLANET_SIZE = 128;                  // size of planet image
const int   PLANET_FRAME = 1;                   // frame of planet
const UINT  MOON_SIZE = 64;                     // size of moon images
const UINT  MOON1_FRAME = 12;                   // frame of moon1

// graphic images
const char DASHBOARD_TEXTURES[] = "pictures\\dashboard.png";

// graphic images
const char NEBULA_IMAGE[] = "pictures\\orion.jpg";     // photo source NASA/courtesy of nasaimages.org 
const char TEXTURES_IMAGE[] = "pictures\\textures.png";  // game textures
const char FONT_IMAGE[] = "pictures\\CKfont.png";
const char SPACE_IMAGE[] = "pictures\\space.jpg";   // photo source NASA/courtesy of nasaimages.org 
const char TEXTURES1_IMAGE[] = "pictures\\textures1.png";  // game textures
const char OCEAN_IMAGE[] = "pictures\\ocean.jpg";   // background
const char PLANE_IMAGE[] = "pictures\\planeTextures.png"; // game textures
const char REFLECTION_IMAGE[] = "pictures\\reflection.png"; // game textures
const char MENU_IMAGE[] = "pictures\\menu.png";     // menu texture

// key mappings
// In this game simple constants are used for key mappings. If variables were used
// it would be possible to save and restore key mappings from a data file.
// key mappings
// In this game simple constants are used for key mappings. If variables were used
// it would be possible to save and restore key mappings from a data file.
const UCHAR CONSOLE_KEY = VK_OEM_3;         // ~キー（日本の場合は@キー）
const UCHAR ESC_KEY = VK_ESCAPE;   // escape key
const UCHAR ALT_KEY = VK_MENU;     // Alt key
const UCHAR ENTER_KEY = VK_RETURN;   // Enter key
const UCHAR SHIP1_LEFT_KEY = 'A';
const UCHAR SHIP1_RIGHT_KEY = 'D';
const UCHAR SHIP1_FORWARD_KEY = 'W';
const UCHAR SHIP1_FIRE_KEY = 'S';
const UCHAR SHIP2_LEFT_KEY = VK_LEFT; // left arrow
const UCHAR SHIP2_RIGHT_KEY = VK_RIGHT; // right arrow
const UCHAR SHIP2_FORWARD_KEY = VK_UP;   // up arrow
const UCHAR SHIP2_FIRE_KEY = VK_DOWN; // down arrow
const UCHAR LEFT_KEY = VK_LEFT;
const UCHAR RIGHT_KEY = VK_RIGHT;
const UCHAR UP_KEY = VK_UP;
const UCHAR DOWN_KEY = VK_DOWN;

// WAVE_BANKは、.xwbファイルの場所
const char WAVE_BANK[] = "audio\\Win\\WavesExample1.xwb";
// audio.cppが必要とするオーディオファイル
// WAVE_BANKは.xwbファイルの場所
const char SOUND_BANK[] = "audio\\Win\\SoundsExample1.xsb";
// サウンドキュー
const char BEEP1[] = "beep1";
const char BEEP2[] = "beep2";
const char BEEP3[] = "beep3";
const char BEEP4[] = "beep4";
const char HIT[] = "hit";
const char ENGINE1[] = "engine1";

// weapon types
enum WEAPON { TORPEDO, SHIP, PLANET };
#endif
