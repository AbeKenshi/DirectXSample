#define WIN32_LEAN_AND_MEAN
#include "windows.h"

// 関数プロトタイプ
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
bool CreateMainWindow(HINSTANCE, int);
LRESULT WINAPI WinProc(HWND, UINT, WPARAM, LPARAM);
bool AnotherInstance();

// グローバル変数
HINSTANCE hinst;
HDC hdc;			// デバイスコンテキストへのハンドル
TCHAR ch = ' ';		// 入力された文字	
RECT rect;			// Rectangle構造体
PAINTSTRUCT ps;		// WM_PAINTで使用される
bool vkKeys[256];	// 仮想キーの状態、trueかfalse

// 定数
const char CLASS_NAME[] = "WinMain";
const char APP_TITLE[] = "Hello World"; // タイトルバーのテキスト
const int WINDOW_WIDTH = 400; // ウィンドウの幅
const int WINDOW_HEIGHT = 400; // ウィンドウの高さ

// Windowsアプリケーションの開始点
// パラメータ
//		hInstance：アプリケーションの現在のインスタンスへのハンドル
//		hPrevInstance：常にNULL（廃止されたパラメータ）
//		lpCmdLine：コマンドライン引数のNULL終端文字へのポインタ
//		nCmdShow：ウィンドウをどのように表示するかを指定
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;

	// 複数のインスタンスの生成を防ぐ
	if (AnotherInstance())
		return false;

	// ウィンドウを作成
	if (!CreateMainWindow(hInstance, nCmdShow))
		return false;

	for (int i = 0; i < 256; ++i)	// 仮想キーの配列の初期化
	{
		vkKeys[i] = false;
	}

	// メインのメッセージループ
	int done = 0;
	while (!done)
	{
		// PeekMessageはWindowsメッセージがあるかをテストする非ブロックメソッド
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// 終了メッセージを検知
			if (msg.message == WM_QUIT)
				done = 1;
			// メッセージをデコードしてWinProcに渡す
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return msg.wParam;
}

// ウィンドウイベントコールバック関数
LRESULT WINAPI WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	short nVirtKey;								// 仮想キーコード
	const short SHIFTED = (short)0x8000;
	TEXTMETRIC tm;
	DWORD chWidth = 20;							// 文字の幅
	DWORD chHeight = 20;						// 文字の高さ

	switch (msg)
	{
	case WM_CREATE:
		// text metricsを取得
		hdc = GetDC(hWnd);
		GetTextMetrics(hdc, &tm);
		ReleaseDC(hWnd, hdc);
		chWidth = tm.tmAveCharWidth;
		chHeight = tm.tmHeight;
		return 0;

	case WM_DESTROY:
		// Windowsにプログラムを終了するように伝える
		PostQuitMessage(0);
		return 0;

	case WM_CHAR:								// キーボードから文字が入力された場合
		switch (wParam)							// 文字はwParamに格納されている
		{
		case 0x08:								// バックスペース
		case 0x09:								// タブ
		case 0x0A:								// ラインフィード
		case 0x0D:								// キャリッジリターン
		case 0x1B:								// エスケープ
			return 0;
		default:								// 表示可能な文字
			ch = (TCHAR)wParam;					// 文字を取得
			InvalidateRect(hWnd, NULL, TRUE);	// WM_PAINTを強制的に発生させる
			return 0;
		}

	case WM_KEYDOWN:							// キーが押された場合
		vkKeys[wParam] = true;
		switch (wParam)
		{
			case VK_SHIFT:							// Shiftキー
				nVirtKey = GetKeyState(VK_LSHIFT);	// 左Shiftキーの状態を取得
				if (nVirtKey & SHIFTED)				// 左Shiftキーの場合
					vkKeys[VK_LSHIFT] = true;
				nVirtKey = GetKeyState(VK_RSHIFT);	// 右Shiftキーの状態を取得
				if (nVirtKey & SHIFTED)				// 右Shiftキーの場合
					vkKeys[VK_RSHIFT] = true;
				break;
			case VK_CONTROL:						// Ctrlキー
				nVirtKey = GetKeyState(VK_LCONTROL);
				if (nVirtKey & SHIFTED)			// 左Ctrlキーの場合
					vkKeys[VK_LCONTROL] = true;
				nVirtKey = GetKeyState(VK_RCONTROL);
				if (nVirtKey & SHIFTED)			// 左Ctrlキーの場合
					vkKeys[VK_RCONTROL] = true;
				break;
		}
		InvalidateRect(hWnd, NULL, TRUE);			// WM_PAINTを強制的に発生させる
		return 0;
		break;

	case WM_KEYUP:									// キーが離された場合
		vkKeys[wParam] = false;
		switch (wParam)
		{
		case VK_SHIFT:							// Shiftキー
			nVirtKey = GetKeyState(VK_LSHIFT);	// 左Shiftキーの状態を取得
			if ((nVirtKey & SHIFTED) == 0)				// 左Shiftキーの場合
				vkKeys[VK_LSHIFT] = false;
			nVirtKey = GetKeyState(VK_RSHIFT);	// 右Shiftキーの状態を取得
			if ((nVirtKey & SHIFTED) == 0)				// 右Shiftキーの場合
				vkKeys[VK_RSHIFT] = false;
			break;
		case VK_CONTROL:						// Ctrlキー
			nVirtKey = GetKeyState(VK_LCONTROL);
			if ((nVirtKey & SHIFTED) == 0)			// 左Ctrlキーの場合
				vkKeys[VK_LCONTROL] = false;
			nVirtKey = GetKeyState(VK_RCONTROL);
			if ((nVirtKey & SHIFTED) == 0)			// 左Ctrlキーの場合
				vkKeys[VK_RCONTROL] = false;
			break;
		}
		InvalidateRect(hWnd, NULL, TRUE);			// WM_PAINTを強制的に発生させる
		return 0;
		break;

	case WM_PAINT:								// ウィンドウを再描画する必要がある場合
		hdc = BeginPaint(hWnd, &ps);			// デバイスコンテキストへのハンドルを取得
		TextOut(hdc, 0, 0, &ch, 1);				// 文字を表示

		// vkKeys配列の状態を表示
		// キーが押されていればTを表示、押されていなければFを表示
		for (int r = 0; r < 16; ++r)
		{
			for (int c = 0; c < 16; ++c)
			{
				if (vkKeys[r * 16 + c])
				{
					SetBkMode(hdc, OPAQUE);		// 文字の背景を不透明にする
					TextOut(hdc, c * chWidth + chWidth * 2, r * chHeight + chHeight * 2, "T ", 2);
				} else {
					SetBkMode(hdc, TRANSPARENT);// 文字の背景を透明にする
					TextOut(hdc, c * chWidth + chWidth * 2, r * chHeight + chHeight * 2, "F ", 2);
				}
			}
		}

		EndPaint(hWnd, &ps);
		return 0;

	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}

// ウィンドウを作成
// 戻り値：エラーの場合、false
bool CreateMainWindow(HINSTANCE hInstance, int nCmdShow)
{
	WNDCLASSEX wcx;
	HWND hwnd;

	// ウィンドウクラスの構造体をメインウィンドウを記述するパラメータで設定します。
	wcx.cbSize = sizeof(wcx);					// 構造体のサイズ
	wcx.style = CS_HREDRAW | CS_VREDRAW;		// ウィンドウサイズ変更時に再描画
	wcx.lpfnWndProc = WinProc;					// ウィンドウプロシージャを指す
	wcx.cbClsExtra = 0;							// 拡張クラスメモリなし
	wcx.cbWndExtra = 0;							// 拡張ウィンドウメモリなし
	wcx.hInstance = hInstance;					// インスタンスへのハンドル
	wcx.hIcon = NULL;
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);	// 事前定義されている矢印カーソル
	wcx.hbrBackground =							// 背景ブラシ
		(HBRUSH)GetStockObject(GRAY_BRUSH);
	wcx.lpszMenuName = NULL;					// メニューリソースの名前
	wcx.lpszClassName = CLASS_NAME;				// ウィンドウクラスの名前
	wcx.hIconSm = NULL;
	// ウィンドウクラスを登録
	// エラー時、RegisterClassExは0を戻す
	if (RegisterClassEx(&wcx) == 0)				// エラーの場合
		return false;
	// ウィンドウを作成
	hwnd = CreateWindow(
		CLASS_NAME,				// ウィンドウクラスの名前
		APP_TITLE,				// タイトルバーのテキスト
		WS_OVERLAPPEDWINDOW,	// ウィンドウのスタイル
		CW_USEDEFAULT,			// ウィンドウの水平位置のデフォルト
		CW_USEDEFAULT,			// ウィンドウの垂直位置のデフォルト
		WINDOW_WIDTH,			// ウィンドウの幅
		WINDOW_HEIGHT,			// ウィンドウの高さ
		(HWND)NULL,				// 親ウィンドウなし
		(HMENU)NULL,			// メニューなし
		hInstance,				// アプリケーションインスタンスへのハンドル
		(LPVOID)NULL);			// ウィンドウパラメータなし
	// ウィンドウの作成でエラーが発生した場合
	if (!hwnd)
		return false;
	// ウィンドウを表示
	ShowWindow(hwnd, nCmdShow);
	// ウィンドウプロシージャにWM_PAINTメッセージを送る
	UpdateWindow(hwnd);
	return true;
}

// 現在のアプリケーションの別のインスタンスがないかをチェック
// 戻り値：別のインスタンスが見つかった場合、true
//		　 自身が唯一のインスタンスである場合、false
bool AnotherInstance()
{
	HANDLE ourMutex;

	// 固有の文字列を使ってミューテックスの作成を試みる
	ourMutex = CreateMutex(NULL, true,
		"MUTEX");
	if (GetLastError() == ERROR_ALREADY_EXISTS)
		return true;	// 別のインスタンスが見つかった場合
	return false;		// 自身が唯一のインスタンスである場合
}