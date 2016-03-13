#include "input.h"

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
Input::Input()
{
	// キーが押されている状態を示す配列をクリア
	for (size_t i = 0; i < inputNS::KEYS_ARRAY_LEN; ++i)
		keysDown[i] = false;
	for (size_t i = 0; i < inputNS::KEYS_ARRAY_LEN; ++i)
		keysPressed[i] = false;
	newLine = true;			// 新しい行を開始
	textIn = "";			// textInをクリア
	charIn = 0;				// charInをクリア
	// マウスデータ
	mouseX = 0;				// 画面X
	mouseY = 0;				// 画面Y
	mouseRawX = 0;			// 高精細X
	mouseRawY = 0;			// 高精細Y
	mouseLButton = false;	// 左マウスボタンが押されている場合にtrue
	mouseRButton = false;	// 右マウスボタンが押されている場合にtrue
	mouseMButton = false;	// 中央マウスボタンが押されている場合にtrue
	mouseX1Button = false;	// X1マウスボタンが押されている場合にtrue
	mouseX2Button = false;	// X2マウスボタンが押されている場合にtrue
	for (int i = 0; i < MAX_CONTROLLERS; ++i)
	{
		controllers[i].vibrateTimeLeft = 0;
		controllers[i].vibrateTimeRight = 0;
	}
}

//=============================================================================
// デストラクタ
//=============================================================================
Input::~Input()
{
	if (mouseCaptured)
		ReleaseCapture();	// マウスを解放
}

//=============================================================================
// マウスとコントローラーの入力を初期化
// マウスをキャプチャする場合、capture=trueを設定
// GameErrorをスロー
//=============================================================================
void Input::initialize(HWND hwnd, bool capture)
{
	try {
		mouseCaptured = capture;
		// 高精細マウスを登録
		Rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
		Rid[0].usUsage = HID_USAGE_GENERIC_MOUSE;
		Rid[0].dwFlags = RIDEV_INPUTSINK;
		Rid[0].hwndTarget = hwnd;
		RegisterRawInputDevices(Rid, 1, sizeof(Rid[0]));
		if (mouseCaptured)
			SetCapture(hwnd);	// マウスをキャプチャ
		// コントローラーの状態をクリア
		ZeroMemory(
			controllers, sizeof(ControllerState) * MAX_CONTROLLERS);
		checkControllers();		// 接続されているコントローラーをチェック
	}
	catch (...)
	{
		throw(GameError(gameErrorNS::FATAL_ERROR,
			"ERROR initializing input system"));
	}
}

//=============================================================================
// このキーについて、keysDown配列とkeysPressed配列にtrueを設定
// 実行前：wParamに、仮想キーコード（0~255）が格納されている
//=============================================================================
void Input::keyDown(WPARAM wParam)
{
	// キーコードが、バッファ範囲内にあることを確認
	if (wParam < inputNS::KEYS_ARRAY_LEN)
	{
		keysDown[wParam] = true;	// keysDown配列を更新
		// キーが既に押されていた、clear()で消去されていた
		keysPressed[wParam] = true;	// keysPressed配列を更新
	}
}

//=============================================================================
// このキーについて、keysDown配列にfalseを設定
// 実行前：wParamに、仮想キーコード（0~255）が格納されている
//=============================================================================
void Input::keyUp(WPARAM wParam)
{
	// キーコードが、バッファ範囲内にあることを確認
	if (wParam < inputNS::KEYS_ARRAY_LEN)
		// 状態テーブルを更新
		keysDown[wParam] = false;
}

//=============================================================================
// 入力された文字をtextIn文字列に保存
// 実行前：wParamに、文字が格納されている
//=============================================================================
void Input::keyIn(WPARAM wParam)
{
	if (newLine)							// 新しい行の開始の場合
	{
		textIn.clear();
		newLine = false;
	}
	if (wParam == '\b')						// バックスペースキーの場合
	{
		if (textIn.length() > 0)			// 文字が存在する場合
			textIn.erase(textIn.size() - 1);// 最後に入力された文字を消去
	}
	else
	{
		textIn += wParam;					// 文字をtextInに追加
		charIn = wParam;					// 最後に入力された文字を消去
	}
	if ((char)wParam == '\r')				// リターンキーの場合
		newLine = true;
}

//=============================================================================
// 指定された仮想キーが押されている場合はtrue、それ以外の場合はfalseを戻す
//=============================================================================
bool Input::iskeyDown(UCHAR vkey) const
{
	if (vkey < inputNS::KEYS_ARRAY_LEN)
		return keysDown[vkey];
	else
		return false;
}

//=============================================================================
// 直近のフレームにおいて、指定の仮想キーが押されたことがある場合、trueを戻します。
// キーの押し下げの状態は、各フレームの終了時に消去されます。
//=============================================================================
bool Input::wasKeyPressed(UCHAR vkey) const
{
	if (vkey < inputNS::KEYS_ARRAY_LEN)
		return keysPressed[vkey];
	else
		return false;
}

//=============================================================================
// 直近のフレームにおいて、何らかのキーが押された場合、trueを戻します。
// キーの押し下げの状態は、各フレームの終了時に消去されます。
//=============================================================================
bool Input::anyKeyPressed() const
{
	for (size_t i = 0; i < inputNS::KEYS_ARRAY_LEN; ++i)
		if (keysPressed[i] == true)
			return true;
	return false;
}

//=============================================================================
// 指定されたキーの押し下げをクリア
//=============================================================================
void Input::clearKeyPress(UCHAR vkey)
{
	if (vkey < inputNS::KEYS_ARRAY_LEN)
		keysPressed[vkey] = false;
}

//=============================================================================
// 指定の入力バッファをクリア
// whatの値についてはinput.hを参照
//=============================================================================
void Input::clear(UCHAR what)
{
	// キーが押されている状態かどうかをクリアする場合
	if (what & inputNS::KEYS_ARRAY_LEN)
	{
		for (size_t i = 0; i < inputNS::KEYS_ARRAY_LEN; ++i)
			keysDown[i] = false;
	}
	// キーが押されたかどうかをクリアする場合
	if (what & inputNS::KEYS_PRESSED)
	{
		for (size_t i = 0; i < inputNS::KEYS_ARRAY_LEN; ++i)
			keysPressed[i] = false;
	}
	// マウスをクリアする場合
	if (what & inputNS::MOUSE)
	{
		mouseX = 0;
		mouseY = 0;
		mouseRawX = 0;
		mouseRawY = 0;
	}
	if (what & inputNS::TEXT_IN)
		clearTextIn();
}

//=============================================================================
// マウスの両面位置を読み取り、mouseXとmouseYに保存
//=============================================================================
void Input::mouseIn(LPARAM lParam)
{
	mouseX = GET_X_LPARAM(lParam);
	mouseY = GET_Y_LPARAM(lParam);
}

//=============================================================================
// マウスからのローデータを読み取り、mouseRawXとmouseRawYに保存
// このルーチンは、高精細マウスに対応しています。
//=============================================================================
void Input::mouseRawIn(LPARAM lParam)
{
	UINT dwSize = 40;
	static BYTE lpb[40];
	GetRawInputData((HRAWINPUT)lParam, RID_INPUT,
		lpb, &dwSize, sizeof(RAWINPUTHEADER));
	RAWINPUT* raw = (RAWINPUT*)lpb;
	if (raw->header.dwType == RIM_TYPEMOUSE)
	{
		mouseRawX = raw->data.mouse.lLastX;
		mouseRawY = raw->data.mouse.lLastY;
	}
}

//=============================================================================
// Check for connected controllers
//=============================================================================
void Input::checkControllers()
{
	DWORD result;
	for (DWORD i = 0; i <MAX_CONTROLLERS; i++)
	{
		result = XInputGetState(i, &controllers[i].state);
		if (result == ERROR_SUCCESS)
			controllers[i].connected = true;
		else
			controllers[i].connected = false;
	}
}

//=============================================================================
// Read state of connected controllers
//=============================================================================
void Input::readControllers()
{
	DWORD result;
	for (DWORD i = 0; i <MAX_CONTROLLERS; i++)
	{
		if (controllers[i].connected)
		{
			result = XInputGetState(i, &controllers[i].state);
			if (result == ERROR_DEVICE_NOT_CONNECTED)    // if controller disconnected
				controllers[i].connected = false;
		}
	}
}

//=============================================================================
// 接続されているコントローラーを振動させる
//=============================================================================
void Input::vibrateControllers(float frameTime)
{
	for (int i = 0; i < MAX_CONTROLLERS; i++)
	{
		if (controllers[i].connected)
		{
			controllers[i].vibrateTimeLeft -= frameTime;
			if (controllers[i].vibrateTimeLeft < 0)
			{
				controllers[i].vibrateTimeLeft = 0;
				controllers[i].vibration.wLeftMotorSpeed = 0;
			}
			controllers[i].vibrateTimeRight -= frameTime;
			if (controllers[i].vibrateTimeRight < 0)
			{
				controllers[i].vibrateTimeRight = 0;
				controllers[i].vibration.wRightMotorSpeed = 0;
			}
			XInputSetState(i, &controllers[i].vibration);
		}
	}
}