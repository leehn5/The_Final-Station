#pragma once
#include "DrawComponent.h"

enum class AnchorPoint
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

class Transform;

class Text : public DrawComponent
{
private:
	wstring _text;
	wstring _fontName;
	wstring _locale;

	float _fontSize;
	float _maxWidth;
	float _maxHeight;

	Vector2 _pos;

	IDWriteTextLayout* _layout;
	ID2D1SolidColorBrush* _brush;

private:
	void SetLayout();

public:
	void CreateText(wstring text, float fontSize, float maxWidth, float maxHeight, ColorF color = ColorF::Black, wstring fontName = L"µ¸¿ò", wstring localeName = L"ko");
	void ChangeText(wstring text);
	void SetFontSize(float fontSize, int startPoint, int length);
	
	// set text draw area
	void SetSize(Vector2 size);
	void SetColor(ColorF color, int startPoint, int length);
	void SetUnderline(bool isUnderline, int startPoint, int length);
	void SetAnchor(AnchorPoint anchor);
	void SetPosition(Vector2 pos) { _pos = pos; }

	virtual void Init();
	virtual void Render();
	virtual void Release();
};

