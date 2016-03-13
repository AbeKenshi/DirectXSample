#ifndef _GRAPHICS_H             // このファイルが複数の箇所でインクルードされる場合に、
#define _GRAPHICS_H             // 多重に定義されることを防ぎます。
#define WIN32_LEAN_AND_MEAN

#ifdef _DEBUG
#define D3D_DEBUG_INFO
#endif
#include <d3d9.h>
#include "constants.h"
#include "gameError.h"

// DirectXポインタ型
#define LP_3DDEVICE LPDIRECT3DDEVICE9
#define LP_3D       LPDIRECT3D9

// 色の定義
#define COLOR_ARGB DWORD
#define SETCOLOR_ARGB(a,r,g,b) \
    ((COLOR_ARGB)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))

class Graphics
{
private:
	// DirectXポインタなど
	LP_3D       direct3d;
	LP_3DDEVICE device3d;
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE pMode;

	// その他の変数
	HRESULT     result;         // 標準のWindowsリターンコード
	HWND        hwnd;
	bool        fullscreen;
	int         width;
	int         height;
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
};

#endif

