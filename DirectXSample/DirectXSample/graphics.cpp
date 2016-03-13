#include "graphics.h"

//=============================================================================
// コンストラクタ
//=============================================================================
Graphics::Graphics()
{
	direct3d = NULL;
	device3d = NULL;
	fullscreen = false;
	width = GAME_WIDTH;    // 幅と高さはinitialize()で置き換えられる
	height = GAME_HEIGHT;
}

//=============================================================================
// デストラクタ
//=============================================================================
Graphics::~Graphics()
{
	releaseAll();
}

//=============================================================================
// すべてを開放
//=============================================================================
void Graphics::releaseAll()
{
	SAFE_RELEASE(device3d);
	SAFE_RELEASE(direct3d);
}

//=============================================================================
// DirectXグラフィックスを初期化
// エラー時にGameErrorをスロー
//=============================================================================
void Graphics::initialize(HWND hw, int w, int h, bool full)
{
	hwnd = hw;
	width = w;
	height = h;
	fullscreen = full;

	//Direct3Dを初期化
	direct3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (direct3d == NULL)
		throw(GameError(gameErrorNS::FATAL_ERROR, 
			"Error initializing Direct3D"));

	initD3Dpp();        // D3Dプレゼンテーションパラメータを初期化
	if (fullscreen)
	{
		if (isAdapterCompatible())
			// 対応しているリフレッシュレートを設定
			d3dpp.FullScreen_RefreshRateInHz = pMode.RefreshRate;
		else
			throw(GameError(gameErrorNS::FATAL_ERROR,
				"The graphics device does not support the " \
				"specified resolution and/or format."));
	}
	
	// グラフィックスカードがハードウェアテクスチャリングとライティング、
	// 頂点シェーダをサポートしているかどうかを判定
	D3DCAPS9 caps;
	DWORD behavior;
	result = direct3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);
	// デバイスがHWテクスチャリングとライティングをサポートしていない場合、
	// または1.1頂点シェーダーをハードウェアでサポートしていない場合、
	// ソフトウェア頂点処理に切り替えます。
	if ((caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 ||
		caps.VertexShaderVersion < D3DVS_VERSION(1, 1))
		// ソフトウェアのみの処理を使用
		behavior = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	else
		// ハードウェアのみの処理を使用
		behavior = D3DCREATE_HARDWARE_VERTEXPROCESSING;
														 
	// Direct3Dデバイスを作成
	result = direct3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hwnd,
		// 一部のシステムでは、Direct3D9 (ERROR) ASSERTION FAILED!
		// が報告されますが、実際にはエラーではありません。
		behavior,
		// グラフィックスカードがハードウェア頂点処理をサポートしない場合は機能しない
		&d3dpp,
		&device3d);

	if (FAILED(result))
		throw(GameError(gameErrorNS::FATAL_ERROR,
			"Error creating Direct3D device"));

}

//=============================================================================
// D3Dプレゼンテーションパラメータを初期化
//=============================================================================
void Graphics::initD3Dpp()
{
	try {
		ZeroMemory(&d3dpp, sizeof(d3dpp));              // 構造体に0を設定
		// 必要なパラメータを設定
		d3dpp.BackBufferWidth = width;
		d3dpp.BackBufferHeight = height;
		if (fullscreen)                                // 全画面表示の場合
			d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;  // 24ビットカラー
		else
			d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;   // デスクトップ設定を使用
		d3dpp.BackBufferCount = 1;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.hDeviceWindow = hwnd;
		d3dpp.Windowed = (!fullscreen);
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	}
	catch (...)
	{
		throw(GameError(gameErrorNS::FATAL_ERROR,
			"Error initializing D3D presentation parameters"));

	}
}

//=============================================================================
// バックバッファを表示
//=============================================================================
HRESULT Graphics::showBackbuffer()
{
	result = E_FAIL;    // 失敗をデフォルトとし、成功時に置き換え
	// （この関数は、後のバージョンでは移動します）
	// バックバッファをライムグリーンにクリア
	device3d->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 255, 0), 0.0f, 0);

	// バックバッファを画面に表示
	result = device3d->Present(NULL, NULL, NULL, NULL);

	return result;
}

//=============================================================================
// アダプターをチェックして、d3dppで指定されたBackBufferの高さ、幅、
// リフレッシュレートに対応しているかどうかを確認します。
// 対応するものが見つかった場合は、pMode構造体に、
// 対応しているモードのフォーマットを設定します。
// 実行前：d3dppが初期化されている
// 実行後：対応しているモードが見つかり、pMode構造体にデータが
// 設定された場合、trueを戻します。
// 対応しているモードが見つからなかった場合、falseを戻します。
//=============================================================================
bool Graphics::isAdapterCompatible()
{
	UINT modes = direct3d->GetAdapterModeCount(D3DADAPTER_DEFAULT,
		d3dpp.BackBufferFormat);
	for (UINT i = 0; i<modes; i++)
	{
		result = direct3d->EnumAdapterModes(D3DADAPTER_DEFAULT,
			d3dpp.BackBufferFormat,
			i, &pMode);
		if (pMode.Height == d3dpp.BackBufferHeight &&
			pMode.Width == d3dpp.BackBufferWidth &&
			pMode.RefreshRate >= d3dpp.FullScreen_RefreshRateInHz)
			return true;
	}
	return false;
}