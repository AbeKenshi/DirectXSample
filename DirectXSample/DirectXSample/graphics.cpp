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
	backColor = graphicsNS::BACK_COLOR;
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
	SAFE_RELEASE(sprite);
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

	result = D3DXCreateSprite(device3d, &sprite);
	if (FAILED(result))
		throw(GameError(gameErrorNS::FATAL_ERROR,
			"Error creating Direct3D sprite"));

	// プリミティブのアルファブレンド用の構成
	device3d->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	device3d->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device3d->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

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
// 頂点バッファを作成
// 実行前: verts[]に頂点データが格納されている
//      size = verts[]のサイズ
// 実行後：成功した場合、&vertexBufferがバッファを指す
//=============================================================================
HRESULT Graphics::createVertexBuffer(VertexC verts[], UINT size, LP_VERTEXBUFFER &vertexBuffer)
{
	// 標準のWindows戻り値
    HRESULT result = E_FAIL;

	// 頂点バッファを作成
    result = device3d->CreateVertexBuffer(size, D3DUSAGE_WRITEONLY, D3DFVF_VERTEX,
                                            D3DPOOL_DEFAULT, &vertexBuffer, NULL);
    if(FAILED(result))
        return result;

    void *ptr;
	// データが転送されてくる前に、バッファをロックする必要がある
    result = vertexBuffer->Lock(0, size, (void**)&ptr, 0);
    if(FAILED(result))
        return result;
    memcpy(ptr, verts, size);   // 頂点データをバッファにコピー
    vertexBuffer->Unlock();     // バッファのロックを解除

    return result;
}

//=============================================================================
// 三角形ファンを使って、アルファ透過性を持つ四角形を表示
// 実行前：createVertexBufferを使ってvertexBufferを作成し、
//		   四角形を時計回りの順序で定義する4つの頂点を格納しておく
// 実行後：四角形が描画される
//=============================================================================
bool Graphics::drawQuad(LP_VERTEXBUFFER vertexBuffer)
{

	HRESULT result = E_FAIL;    // 標準のWindows戻り値

	if (vertexBuffer == NULL)
		return false;
	// アルファブレンドを有効化
	device3d->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	device3d->SetStreamSource(0, vertexBuffer, 0, sizeof(VertexC));
	device3d->SetFVF(D3DFVF_VERTEX);
	result = device3d->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);

	// アルファブレンドを無効化
	device3d->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

	if (FAILED(result))
		return false;

	return true;
}

//=============================================================================
// バックバッファを表示
//=============================================================================
HRESULT Graphics::showBackbuffer()
{
	result = E_FAIL;    // 失敗をデフォルトとし、成功時に置き換え

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

//=============================================================================
// デバイスが消失していないかをチェック
//=============================================================================
HRESULT Graphics::getDeviceState()
{
	result = E_FAIL;	// 失敗をデフォルトとし、成功時に置き換え
	if (device3d == NULL)
		return result;
	result = device3d->TestCooperativeLevel();
	return result;
}


//=============================================================================
// グラフィックスデバイスをリセット
//=============================================================================
HRESULT Graphics::reset()
{
	result = E_FAIL;	// 失敗をデフォルトとし、成功時に置き換え
	initD3Dpp();		// D3Dプレゼンテーションパラメータを初期化
	sprite->OnLostDevice();
	// グラフィックスデバイスのリセットを試みる
	result = device3d->Reset(&d3dpp);
	// プリミティブのアルファブレンド用の構成
	// Configure for alpha blend of primitives
	device3d->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	device3d->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device3d->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	sprite->OnResetDevice();
	return result;
}


//=============================================================================
// テクスチャをデフォルトのD3Dメモリに読み込む（通常のテクスチャで使用）
// エンジンの使用時に内部的にのみ使用します。
// ゲームテクスチャの読み込みには、TextureManagerクラスを使用します。
// 実行前：finenameは、テクスチャファイルの名前
//		   transcolorは、透明として扱う色
// 実行後：widthとheight = テクスチャの寸法
//		   textureは、テクスチャを指す
// HRESULTを戻す
//=============================================================================
HRESULT Graphics::loadTexture(const char *filename, COLOR_ARGB transcolor,
	UINT &width, UINT &height, LP_TEXTURE &texture)
{
	// ファイル情報を読み取るための構造体
	D3DXIMAGE_INFO info;
	result = E_FAIL;
	try {
		if (filename == NULL)
		{
			texture = NULL;
			return D3DERR_INVALIDCALL;
		}

		// 幅と高さをファイルから取得
		result = D3DXGetImageInfoFromFile(filename, &info);

		if (result != D3D_OK)
			return result;
		width = info.Width;
		height = info.Height;

		// ファイルを読み込んで、新しいテクスチャを作成
		result = D3DXCreateTextureFromFileEx(
			device3d,		// 3Dデバイス
			filename,		// 画像ファイルの名前
			info.Width,		// テクスチャの幅
			info.Height,	// テクスチャの高さ
			1,				// ミップマップのレベル（チェーンなしの場合は1）
			0,				// 使用方法（Usage）
			D3DFMT_UNKNOWN,	// サーフェイスフォーマット（デフォルト）
			D3DPOOL_DEFAULT,// テクスチャ用のメモリの種類
			D3DX_DEFAULT,	// 画像フィルタ
			D3DX_DEFAULT,	// ミップフィルタ
			transcolor,		// 透明用の色キー
			&info,			// ビットマップファイル情報（読み込んだファイルから）
			NULL,			// カラーパレット
			&texture);		// 目的のテクスチャ
	}
	catch (...)
	{
		throw(GameError(gameErrorNS::FATAL_ERROR,
			"Error in Graphics::loadTexture"));
	}
	return result;
}

//=============================================================================
// SpriteData構造体に記述されたスプライトを描画
// color（オプション）は、フィルタのように適用される
// デフォルトは白（変化なし）
// 実行前：sprite->Begin()を呼び出す
// 実行後：sprite->End()を呼び出す
//		   spriteData.rectは、描画するspriteData.textureの部分を定義
//		   spriteData.rect.rightは、右端+1にする必要がある
//		   spriteData.rect.bottomは、下端+1にする必要がある
//=============================================================================
void Graphics::drawSprite(const SpriteData &spriteData, COLOR_ARGB color)
{
	if (spriteData.texture == NULL)	// テクスチャがない場合
		return;
	// スプライトの中心を特定
	D3DXVECTOR2 spriteCenter = D3DXVECTOR2(
		(float)(spriteData.width / 2 * spriteData.scale),
		(float)(spriteData.height / 2 * spriteData.scale)); 
	// スプライトの画面位置
	D3DXVECTOR2 translate = D3DXVECTOR2((float)spriteData.x, (float)spriteData.y);
	// X方向とY方向の拡大縮小
	D3DXVECTOR2 scaling(spriteData.scale, spriteData.scale);
	if (spriteData.flipHorizontal)	// 水平方向に反転する場合
	{
		scaling.x *= -1;			// Xスケールを負にして反転
		// 反転された画像の中心を取得
		spriteCenter.x -= (float)(spriteData.width * spriteData.scale);
		// 左端を中心に反転が起きるので、反転された画像を
		// 元と同じ位置に配置するために、右へ移動します。
		translate.x += (float)(spriteData.width * spriteData.scale);
	}
	if (spriteData.flipVertical)	// 垂直方向に反転する場合
	{
		scaling.y *= -1;			// Yスケールを負にして反転
		// 反転された画像の中心を取得
		spriteCenter.y -= (float)(spriteData.height * spriteData.scale);
		// 上端を中心に反転が起きるので、反転された画像を
		// 元と同じ位置に配置するために、下へ移動します。
		translate.y += (float)(spriteData.height * spriteData.scale);
	}
	// スプライトの回転、拡大縮小、配置を行うための行列を作成
	D3DXMATRIX matrix;
	D3DXMatrixTransformation2D(
		&matrix,					// 行列
		NULL,						// 拡大縮小を行うとき、原点は左上のまま
		0.0f,						// 拡大縮小時の回転なし
		&scaling,					// 拡大縮小の倍率
		&spriteCenter,				// 回転の中心
		(float)(spriteData.angle),	// 回転の角度
		&translate);				// X、Y位置
	// 行列をスプライトに適用
	sprite->SetTransform(&matrix);
	// スプライトを描画
	sprite->Draw(spriteData.texture, &spriteData.rect, NULL, NULL, color);
}

//=============================================================================
// Toggle window or fullscreen mode
// Pre: All user created D3DPOOL_DEFAULT surfaces are freed.
// Post: All user surfaces are recreated.
//=============================================================================
void Graphics::changeDisplayMode(graphicsNS::DISPLAY_MODE mode)
{
	try {
		switch (mode)
		{
		case graphicsNS::FULLSCREEN:
			if (fullscreen)      // if already in fullscreen mode
				return;
			fullscreen = true; break;
		case graphicsNS::WINDOW:
			if (fullscreen == false) // if already in window mode
				return;
			fullscreen = false; break;
		default:        // default to toggle window/fullscreen
			fullscreen = !fullscreen;
		}
		reset();
		if (fullscreen)  // fullscreen
		{
			SetWindowLong(hwnd, GWL_STYLE, WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP);
		}
		else            // windowed
		{
			SetWindowLong(hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
			SetWindowPos(hwnd, HWND_TOP, 0, 0, GAME_WIDTH, GAME_HEIGHT,
				SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

			// Adjust window size so client area is GAME_WIDTH x GAME_HEIGHT
			RECT clientRect;
			GetClientRect(hwnd, &clientRect);   // get size of client area of window
			MoveWindow(hwnd,
				0,                                           // Left
				0,                                           // Top
				GAME_WIDTH + (GAME_WIDTH - clientRect.right),    // Right
				GAME_HEIGHT + (GAME_HEIGHT - clientRect.bottom), // Bottom
				TRUE);                                       // Repaint the window
		}

	}
	catch (...)
	{
		// An error occured, try windowed mode 
		SetWindowLong(hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
		SetWindowPos(hwnd, HWND_TOP, 0, 0, GAME_WIDTH, GAME_HEIGHT,
			SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

		// Adjust window size so client area is GAME_WIDTH x GAME_HEIGHT
		RECT clientRect;
		GetClientRect(hwnd, &clientRect);   // get size of client area of window
		MoveWindow(hwnd,
			0,                                           // Left
			0,                                           // Top
			GAME_WIDTH + (GAME_WIDTH - clientRect.right),    // Right
			GAME_HEIGHT + (GAME_HEIGHT - clientRect.bottom), // Bottom
			TRUE);                                       // Repaint the window
	}
}

//=============================================================================
// テクスチャをシステムメモリに読み込む（システムメモリはロック可能）
// ピクセルデータへの直接アクセスを可能にします。
// Texturemanagerクラスを使って、表示するテクスチャを読み込みます。
// 実行前：filenameは、テクスチャファイルの名前
//		   transcolorは、透明として扱う色
// 実行後：widthとheight = テクスチャの寸法
//		   textureは、テクスチャを指す
// HRESULTを戻し、TextureData構造体にデータを格納する
//=============================================================================
HRESULT Graphics::loadTextureSystemMem(const char *filename, COLOR_ARGB
	transcolor, UINT &width, UINT &height, LP_TEXTURE &texture)
{
	// ビットマップファイル情報を読み取るための構造体
	D3DXIMAGE_INFO info;
	result = E_FAIL;        // 標準のWindows戻り値

	try {
		if (filename == NULL)
		{
			texture = NULL;
			return D3DERR_INVALIDCALL;
		}

		// 幅と高さをビットマップファイルから取得
		result = D3DXGetImageInfoFromFile(filename, &info);
		if (result != D3D_OK)
			return result;
		width = info.Width;
		height = info.Height;

		// ビットマップ画像ファイルを読み込んで、新しいテクスチャを作成
		result = D3DXCreateTextureFromFileEx(
			device3d,           // 3Dデバイス
			filename,           // ビットマップファイルの名前
			info.Width,         // ビットマップ画像の幅
			info.Height,        // ビットマップ画像の高さ
			1,                  // ミップマップのレベル（チェーンなしの場合は1）
			0,                  // 使用方法（Usage）
			D3DFMT_UNKNOWN,     // サーフェイスフォーマット（デフォルト）
			D3DPOOL_SYSTEMMEM,  // システムメモリはロック可能
			D3DX_DEFAULT,       // 画像フィルタ
			D3DX_DEFAULT,       // ミップフィルタ
			transcolor,         // 透明色の色キー
			&info,              // ビットマップファイル情報（読み込んだファイルから）
			NULL,               // カラーパレット
			&texture);          // 目的のテクスチャ

	}
	catch (...)
	{
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error in Graphics::loadTexture"));
	}
	return result;
}