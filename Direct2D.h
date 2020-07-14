#pragma once
#include"singletonBase.h"

#pragma comment(lib, "d2d1.lib")
#include <d2d1.h>
#include <d2d1_1helper.h>

#pragma comment(lib, "windowscodecs.lib")
#include <wincodec.h>

#pragma comment(lib, "dwrite.lib")
#include <dwrite.h>

class Direct2D : public singletonBase<Direct2D>
{
private:
	ID2D1Factory* _d2dFactory;
	IDWriteFactory* _wFactory;
	ID2D1HwndRenderTarget* _renderTarget;
	IWICImagingFactory* _wicFactory;
	Direct2D();

public:
	static Direct2D* GetInstance();

	ID2D1Bitmap* CreateBitmap(wstring imgPath);
	IDWriteTextLayout* CreateTextLayout(wstring txt, wstring fontName, float fontSize, float maxWidth, float maxHeight, wstring locale);
	ID2D1HwndRenderTarget* GetRenderTarger() { return _renderTarget; }
};

