#ifndef _TEXTUREMANAGER_H		// このファイルが複数の箇所でインクルードされる場合に、
#define _TEXTUREMANAGER_H		// 多重に定義されることを防ぎます。
#define WIN32_LEAN_AND_MEAN

#include "graphics.h"
#include "constants.h"

class TextureManager
{
private:
	// TextureManager プロパティ
	UINT width;				// テクスチャの幅（ピクセル単位）
	UINT height;			// テクスチャの高さ（ピクセル単位）
	LP_TEXTURE texture;		// テクスチャへのポインタ
	const char *file;		// ファイル名
	Graphics *graphics;		// graphicsへのポインタ
	bool initialized;		// 初期化に成功した場合、true
	HRESULT hr;				// 標準の戻り値
public:
	// コンストラクタ
	TextureManager();

	// デストラクタ
	virtual ~TextureManager();

	// テクスチャへのポインタを戻す
	LP_TEXTURE getTexture() const {
		return texture;
	}

	// テクスチャの幅を戻す
	UINT getWidth() const {
		return width;
	}

	// テクスチャの高さを戻す
	UINT getHeight() const {
		return height;
	}

	// テクスチャファイルをディスクから読み込む
	// 実行後：成功した場合はtrue、失敗した場合はfalseを戻す
	virtual bool initialize(Graphics* g, const char *f);

	// グラフィックスデバイスが消失したときに呼び出される
	virtual void onLostDevice();

	// グラフィックスデバイスがリセットされたときに呼び出される
	virtual void onResetDevice();

};
#endif // !_TEXTUREMANAGER_H
