#include "stdafx.h"
#include "Direct2D.h"

Direct2D::Direct2D()
{
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &_d2dFactory);

	// wic 이미지 팩토리 생성
	CoInitialize(0);
	CoCreateInstance(CLSID_WICImagingFactory, 0, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&_wicFactory));
	//assert(hr == S_OK);

	RECT rc;
	GetClientRect(_hWnd, &rc);
	D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

	_d2dFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(_hWnd, size), &_renderTarget);

	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(*&_wFactory), (IUnknown**)&_wFactory);
	//AddTextFormat(L"맑은고딕", 20);
}

Direct2D* Direct2D::GetInstance()
{
	static Direct2D* instance = new Direct2D();
	return instance;
}

ID2D1Bitmap* Direct2D::CreateBitmap(wstring imgPath)
{
	// 임시
	_renderTarget = GRAPHICMANAGER->GetRenderTarget();

	// 디코더 생성
	IWICBitmapDecoder* decoder = nullptr;
	_wicFactory->CreateDecoderFromFilename(imgPath.c_str(), NULL, GENERIC_READ,
		WICDecodeMetadataCacheOnDemand, &decoder);

	// 디코더에서 프레임 얻음
	IWICBitmapFrameDecode* frame = nullptr;
	decoder->GetFrame(0, &frame);
	//frame->GetSize()

	// 프레임을 기반으로 포맷 컨버터 만듬
	IWICFormatConverter* converter;
	_wicFactory->CreateFormatConverter(&converter);
	converter->Initialize(frame, GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeCustom);

	// 컨버트된 데이터를 기반으로 실제 비트맵 만듬
	ID2D1Bitmap* bitmap = nullptr;
	_renderTarget->CreateBitmapFromWicBitmap(converter, NULL, &bitmap);

	decoder->Release();
	converter->Release();
	frame->Release();

	return bitmap;
}

IDWriteTextLayout* Direct2D::CreateTextLayout(wstring txt, wstring fontName, float fontSize, float maxWidth, float maxHeight, wstring locale)
{
	IDWriteTextFormat* format = nullptr;
	_wFactory->CreateTextFormat(fontName.c_str(), 0, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, fontSize, locale.c_str(), &format);

	IDWriteTextLayout* layout = nullptr;
	_wFactory->CreateTextLayout(txt.c_str(), txt.length(), format, maxWidth, maxHeight, &layout);

	return layout;
}
