#ifndef _GRAPHICS_H             // このファイルが複数の箇所でインクルードされる場合に、
#define _GRAPHICS_H             // 多重に定義されることを防ぎます。
#define WIN32_LEAN_AND_MEAN

#ifdef _DEBUG
#define D3D_DEBUG_INFO
#endif
#include <d3d9.h>
#include <d3dx9.h>
#include "constants.h"
#include "gameError.h"

// DirectXポインタ型
#define LP_3DDEVICE LPDIRECT3DDEVICE9
#define LP_3D       LPDIRECT3D9
#define LP_SPRITE LPD3DXSPRITE
#define LP_TEXTURE  LPDIRECT3DTEXTURE9

// 色の定義
#define COLOR_ARGB DWORD
#define SETCOLOR_ARGB(a,r,g,b) \
    ((COLOR_ARGB)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))

// ベクトル型
#define VECTOR2 D3DXVECTOR2

namespace graphicsNS
{
	// 共通で使用する色
	// ARGBの数値の範囲は0から255まで
	// A = アルファチャネル（透明度、255は不透明）
	// R = 赤、G = 緑、B = 青
	const COLOR_ARGB ORANGE = D3DCOLOR_ARGB(255, 255, 165, 0);
	const COLOR_ARGB BROWN = D3DCOLOR_ARGB(255, 139, 69, 19);
	const COLOR_ARGB LTGRAY = D3DCOLOR_ARGB(255, 192, 192, 192);
	const COLOR_ARGB GRAY = D3DCOLOR_ARGB(255, 128, 128, 128);
	const COLOR_ARGB OLIVE = D3DCOLOR_ARGB(255, 128, 128, 0);
	const COLOR_ARGB PURPLE = D3DCOLOR_ARGB(255, 128, 0, 128);
	const COLOR_ARGB MAROON = D3DCOLOR_ARGB(255, 128, 0, 0);
	const COLOR_ARGB TEAL = D3DCOLOR_ARGB(255, 0, 128, 128);
	const COLOR_ARGB GREEN = D3DCOLOR_ARGB(255, 0, 128, 0);
	const COLOR_ARGB NAVY = D3DCOLOR_ARGB(255, 0, 0, 128);
	const COLOR_ARGB WHITE = D3DCOLOR_ARGB(255, 255, 255, 255);
	const COLOR_ARGB YELLOW = D3DCOLOR_ARGB(255, 255, 255, 0);
	const COLOR_ARGB MAGENTA = D3DCOLOR_ARGB(255, 255, 0, 255);
	const COLOR_ARGB RED = D3DCOLOR_ARGB(255, 255, 0, 0);
	const COLOR_ARGB CYAN = D3DCOLOR_ARGB(255, 0, 255, 255);
	const COLOR_ARGB LIME = D3DCOLOR_ARGB(255, 0, 255, 0);
	const COLOR_ARGB BLUE = D3DCOLOR_ARGB(255, 0, 0, 255);
	const COLOR_ARGB BLACK = D3DCOLOR_ARGB(255, 0, 0, 0);
	// colorFilterで描画することを指定するために使用
	const COLOR_ARGB FILTER = D3DCOLOR_ARGB(0, 0, 0, 0);
	// 色とANDを実行して、25%アルファを取得
	const COLOR_ARGB ALPHA25 = D3DCOLOR_ARGB(64, 255, 255, 255);
	// 色とANDを実行して、50%アルファを取得
	const COLOR_ARGB ALPHA50 = D3DCOLOR_ARGB(128, 255, 255, 255);
	const COLOR_ARGB BACK_COLOR = NAVY;                         // background color of game

	enum DISPLAY_MODE { TOGGLE, FULLSCREEN, WINDOW };
}

// SpriteData：スプライトを描画するGraphics::drawSpriteが必要とするプロパティ
struct SpriteData
{
	int width;			// スプライトの幅（ピクセル単位）
	int height;			// スプライトの高さ（ピクセル単位）
	float x;			// 画面位置（スプライトの左上隅）
	float y;
	float scale;		// <1は縮小、>1は拡大
	float angle;		// 回転角度（ラジアン単位）
	RECT rect;			// 大きなテクスチャから1つの画像を選択するときに使用
	LP_TEXTURE texture;	// テクスチャへのポインタ
	bool flipHorizontal;// スプライトを水平方向に反転する場合
						// （鏡のような反転の場合）、true
	bool flipVertical;	// スプライトを垂直方向に反転する場合、true
};

class Graphics
{
private:
	// DirectXポインタなど
	LP_3D       direct3d;
	LP_3DDEVICE device3d;
	LP_SPRITE sprite;
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE pMode;

	// その他の変数
	HRESULT     result;         // 標準のWindowsリターンコード
	HWND        hwnd;
	bool        fullscreen;
	int         width;
	int         height;
	COLOR_ARGB  backColor;      // 背景色
	// （エンジンの使用時に内部的にのみ使用します。
	// 内部にはユーザーが使用するものはありません。）

	// D3Dプレゼンテーションパラメータを初期化
	void    initD3Dpp();

public:
	// コンストラクタ
	Graphics();

	// デストラクタ
	virtual ~Graphics();

	// direct3dとdevice3dを解放
	void    releaseAll();

	// DirectXグラフィックスを初期化
	// hw = ウィンドウへのハンドル
	// width = 幅（ピクセル単位）
	// height = 高さ（ピクセル単位）
	// fullscreen = 全画面表示の場合はtrue、ウィンドウの場合はfalse
	// エラー時にGameErrorをスロー
	void    initialize(HWND hw, int width, int height, bool fullscreen);

	// テクスチャをデフォルトのD3Dメモリに読み込む（通常のテクスチャで使用）
	// エンジンの使用時に内部的にのみ使用します。
	// ゲームテクスチャの読み込みには、TextureManagerクラスを使用します。
	// 実行前：finenameは、テクスチャファイルの名前
	//		   transcolorは、透明として扱う色
	// 実行後：widthとheight = テクスチャの寸法
	//		   textureは、テクスチャを指す
	// HRESULTを戻す
	HRESULT loadTexture(const char *filename, COLOR_ARGB transcolor,
		UINT &width, UINT &height, LP_TEXTURE &texture);

	// オフスクリーンバックバッファを画面に表示
	HRESULT showBackbuffer();

	// アダプターをチェックして、d3dppで指定されたBackBufferの高さ、幅、
	// リフレッシュレートに対応しているかどうかを確認します。
	// 対応するものが見つかった場合は、pMode構造体に、
	// 対応しているモードのフォーマットを設定します。
	// 実行前：d3dppが初期化されている
	// 実行後：対応しているモードが見つかり、pMode構造体にデータが
	// 設定された場合、trueを戻します。
	// 対応しているモードが見つからなかった場合、falseを戻します。
	bool isAdapterCompatible();

	// SpriteData構造体に記述されたスプライトを描画
	// color（オプション）は、フィルタのように適用される
	// デフォルトは白（変化なし）
	// 実行前：sprite->Begin()を呼び出す
	// 実行後：sprite->End()を呼び出す
	//		   spriteData.rectは、描画するspriteData.textureの部分を定義
	//		   spriteData.rect.rightは、右端+1にする必要がある
	//		   spriteData.rect.bottomは、下端+1にする必要がある
	void drawSprite(const SpriteData &spriteData, COLOR_ARGB color);

	// グラフィックスデバイスをリセット
	HRESULT reset();

	// Toggle, fullscreen or window display mode
	// Pre: All user created D3DPOOL_DEFAULT surfaces are freed.
	// Post: All user surfaces are recreated.
	void changeDisplayMode(graphicsNS::DISPLAY_MODE mode = graphicsNS::TOGGLE);

	// get functions
	// Return direct3d.
	LP_3D get3D() { return direct3d; }

	// Return device3d.
	LP_3DDEVICE get3Ddevice() { return device3d; }

	// Return sprite
	LP_SPRITE getSprite() { return sprite; }

	// Return handle to device context (window).
	HDC getDC() { return GetDC(hwnd); }

	// デバイスが消失していないかをチェック
	HRESULT getDeviceState();

	// Return fullscreen
	bool getFullscreen() { return fullscreen; }

	// Set color used to clear screen
	void setBackColor(COLOR_ARGB c) { backColor = c; }

	// バックバッファをクリアして、DirectXのBeginScene()を呼び出す
	HRESULT beginScene() {
		result = E_FAIL;
		if (device3d == NULL)
			return result;
		// バックバッファをbackColorでクリアする
		device3d->Clear(0, NULL, D3DCLEAR_TARGET, backColor, 1.0F, 0);
		result = device3d->BeginScene();	// 描画のためのシーンを開始する
		return result;
	}

	// DirectXのEndScene()を呼び出す
	HRESULT endScene() {
		result = E_FAIL;
		if (device3d)
			result = device3d->EndScene();
		return result;
	}

	//=============================================================================
	// Sprite Begin
	//=============================================================================
	void spriteBegin()
	{
		sprite->Begin(D3DXSPRITE_ALPHABLEND);
	}

	//=============================================================================
	// Sprite End
	//=============================================================================
	void spriteEnd()
	{
		sprite->End();
	}

	// ベクトルVの長さをfloatとして戻します。
	static float Veector2Length(const VECTOR2 *v)
	{
		return D3DXVec2Length(v);
	}

	// ベクトルV1とベクトルV2の内積をfloatとして戻します。
	static float Vector2Dot(const VECTOR2 *v1, const VECTOR2 *v2)
	{
		return D3DXVec2Dot(v1, v2);
	}

	// ベクトルVを単位ベクトルに変換します。
	static void Vector2Normalize(VECTOR2 *v)
	{
		D3DXVec2Normalize(v, v);
	}
};

#endif

