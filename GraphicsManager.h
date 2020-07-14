#pragma once
#include <map>

#include "Direct2D.h"

#pragma comment(lib, "d2d1.lib")
#include <d2d1.h>
#include <d2d1_1helper.h>

#pragma comment(lib, "windowscodecs.lib")
#include <wincodec.h>

#pragma comment(lib, "dwrite.lib")
#include <dwrite.h>

#include "Graphic.h"

using namespace D2D1;

enum class TextPivot
{
	LEFT_TOP,
	LEFT_CENTER,
	LEFT_BOTTOM,
	CENTER_TOP,
	CENTER,
	CENTER_BOTTOM,
	RIGHT_TOP,
	RIGHT_CENTER,
	RIGHT_BOTTOM,
};

class GraphicsManager
{
private:
	typedef map<string, Graphic*>							mapImageList;
	typedef map<string, Graphic*>::iterator					mapImageIter;
	typedef map<wstring, IDWriteTextFormat*>				mapTxtFormatList;
	typedef map<wstring, IDWriteTextFormat*>::iterator		mapTxtFormatIter;

private:
	mapImageList								_mImageList;

	// d2d 
	ID2D1Factory*								_d2dFactory;
	ID2D1HwndRenderTarget*						_renderTarget;

	//ID2D1RenderTarget*							_render;
	//ID2D1Device*								_device;
	//ID2D1DeviceContext*							_deviceContext;

	// wic
	IWICImagingFactory*							_wicFactory;

	IDWriteFactory*								_wFactory;
	mapTxtFormatList							_txtFormatList;
	IDWriteTextLayout*							_txtLayout;
	ColorF										_rtColor = ColorF::Black;

private:
	ID2D1Bitmap* CreateD2DBitmap(wstring file);

	GraphicsManager()
	{
		_d2dFactory = nullptr;
		_renderTarget = nullptr;
		_wicFactory = nullptr;
	};

public:
	static GraphicsManager* GetInstance();

	~GraphicsManager() {};

	HRESULT init();
	HRESULT initRenderTarget();
	void Release();

	Graphic* AddImage(string key, wstring file, int maxFrameX = 1, int maxFrameY = 1);

	Graphic* FindImage(string strKey);
	BOOL DeleteImage(string strKey);
	BOOL DeleteAll();

	void Reload();

	void DrawImage(string key, float x, float y, PIVOT pivot = PIVOT::CENTER);
	void DrawImage(string key, Vector2 pos, float alpha = 1.0f, bool flipX = false, PIVOT pivot = PIVOT::CENTER, bool cameraAffect = true);
	void DrawImage(string key, Vector2 pos, Vector2 scale, float angle, Vector2 angleAnchor = Vector2::zero, bool flipX = false, float alpha = 1.0f, PIVOT pivot = PIVOT::CENTER, bool cameraAffect = true);

	void DrawFrameImage(string key, Vector2 pos, float curFrameX, float curFrameY, float alpha = 1.0f, PIVOT pivot = PIVOT::CENTER, bool cameraAffect = true);
	void DrawFrameImage(string key, Vector2 pos, float curFrameX, float curFrameY, Vector2 scale, float angle = 0.0f, Vector2 angleAnchor = Vector2::zero, bool flipX = false, float alpha = 1.0f, PIVOT pivot = PIVOT::CENTER, bool cameraAffect = true);

	// ¼ö¸® Áß
	void LoopRender(string key, const LPRECT drawArea, int offsetX, int offsetY);

	void DrawLine(int startX, int startY, int destX, int destY, ColorF color = ColorF::Black, float strokeWidth = 1.0f);
	void DrawLine(float startX, float startY, float destX, float destY, ColorF color = ColorF::Black, float strokeWidth = 1.0f);
	void DrawLine(Vector2 start, Vector2 dest, ColorF color = ColorF::Black, float strokeWidth = 1.0f, bool isCameraEffect = false);

	void DrawRect(float x, float y, float width, float height, float angle = 0.0f, ColorF color = ColorF::Black);
	void DrawRect(Vector2 pos, Vector2 size, float angle = 0.0f, ColorF color = ColorF::Black, PIVOT pivot = PIVOT::CENTER, float strokeWidth = 1.0f, bool cameraAffect = true);
	void DrawSkewRect(Vector2 pos, Vector2 size, float angle = 0.0f, float strokeWidth = 1.0f, ColorF color = ColorF::Black);

	void DrawRoundRect(float x, float y, float width, float height, float radiusX, float radiusY, ColorF color = ColorF::Black, float strokeWidth = 3.0f);
	void DrawRoundRect(Vector2 pos, Vector2 size, Vector2 radius, ColorF color = ColorF::Black, float strokeWidth = 3.0f);

	void DrawEllipse(float x, float y, float radiusX, float radiusY, ColorF color = ColorF::Black, float strokeWidth = 3.0f);
	void DrawEllipse(Vector2 pos, Vector2 radius, ColorF color = ColorF::Black, float strokeWidth = 3.0f);

	void DrawFillRect(Vector2 pos, Vector2 size, float angle = 0.0f, ColorF color = ColorF::Black, PIVOT pivot = PIVOT::CENTER, bool isCameraAffect = true);
	void DrawFillEllipse(Vector2 pos, Vector2 radius, float angle = 0.0f, ColorF color = ColorF::Black);
	void DrawFillRoundRect(Vector2 pos, Vector2 size, Vector2 radius, ColorF color = ColorF::Black);

	HRESULT AddTextFormat(wstring fontName, float size);

	// txtSize : ±Û¾¾ Å©±â (±Û¾¾ ±æÀÌ X)
	void DrawTextD2D(Vector2 pos, wstring txt, int txtSize, ColorF color = ColorF::Black, TextPivot point = TextPivot::LEFT_TOP, wstring font = L"¸¼Àº°íµñ", bool cameraAffect = false);
	void DrawTextD2D(Vector2 pos, const char* txt, int txtSize, ColorF color = ColorF::Black, TextPivot point = TextPivot::LEFT_TOP, wstring font = L"¸¼Àº°íµñ", bool cameraAffect = false);
	void DrawTextD2D(Vector2 pos, const char* txt, int txtSize, float maxWidth, float maxHeight, ColorF color = ColorF::Black, TextPivot point = TextPivot::LEFT_TOP, wstring font = L"¸¼Àº°íµñ", bool cameraAffect = false);


	void Text(Vector2 pos, wstring txt, int txtSize, float maxWidth, float maxHeight, ColorF color = ColorF::Black, TextPivot point = TextPivot::LEFT_TOP, wstring font = L"¸¼Àº°íµñ", bool cameraEffect = false);
	void Text(Vector2 pos, const char* txt, int txtSize, float maxWidth, float maxHeight, ColorF color = ColorF::Black, TextPivot point = TextPivot::LEFT_TOP, wstring font = L"¸¼Àº°íµñ", bool cameraEffect = false);

	void DrawTextField(Vector2 pos, wstring txt, int txtSize, int width, int height, float alpha, ColorF::Enum color = ColorF::Black, DWRITE_TEXT_ALIGNMENT alig = DWRITE_TEXT_ALIGNMENT_LEADING, wstring font = L"¸¼Àº°íµñ");

	void SetRenderTargetColor(ColorF color) { _rtColor = color; }
	ColorF GetRenderTargetColor() { return _rtColor; }
	ID2D1HwndRenderTarget* GetRenderTarget() { return _renderTarget; }
};

