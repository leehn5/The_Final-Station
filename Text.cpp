#include "stdafx.h"
#include "Text.h"

void Text::SetLayout()
{
	if (_layout) _layout->Release();
	_layout = Direct2D::GetInstance()->CreateTextLayout(_text, _fontName, _fontSize, _maxWidth, _maxHeight, _locale);
}

void Text::CreateText(wstring text, float fontSize, float maxWidth, float maxHeight, ColorF color, wstring fontName, wstring localeName)
{
	_text = text;
	_fontName = fontName;
	_locale = localeName;
	_fontSize = fontSize;
	_maxWidth = maxWidth;
	_maxHeight = maxHeight;

	SetLayout();

	_trans = _object->GetTrans();
	_trans->SetScale(Vector2(maxWidth, maxHeight));

	_pos = _object->GetTrans()->GetPos();

	//Direct2D::GetInstance()->GetRenderTarger()->CreateSolidColorBrush(ColorF(color, alpha), &_brush);
	GRAPHICMANAGER->GetRenderTarget()->CreateSolidColorBrush(color, &_brush);
}

void Text::ChangeText(wstring text)
{
	if (_text == text) return;
	_text = text;
	SetLayout();
}

void Text::Init()
{
	DrawComponent::Init();
}

void Text::Render()
{
	//auto renderTarger = Direct2D::GetInstance()->GetRenderTarger();
	ID2D1HwndRenderTarget* renderTarger = GRAPHICMANAGER->GetRenderTarget();

	Matrix3x2F trans = Matrix3x2F::Translation(_pos.x, _pos.y);

	if (_object->GetCameraAffect()) renderTarger->SetTransform(Matrix3x2F::Identity() * trans * CAMERA->GetMatrix());
	else renderTarger->SetTransform(Matrix3x2F::Identity());

	renderTarger->DrawTextLayout(Point2F(_pos.x, _pos.y), _layout, _brush);
}

void Text::Release()
{
	_layout->Release();
	_brush->Release();
}

void Text::SetFontSize(float fontSize, int startPoint, int length)
{
	_layout->SetFontSize(fontSize, { (UINT32)startPoint, (UINT32)length });
}

void Text::SetSize(Vector2 size)
{
	_maxWidth = size.x;
	_maxHeight = size.y;
	SetLayout();
}

void Text::SetColor(ColorF color, int startPoint, int length)
{
	ID2D1SolidColorBrush* brush;
	//Direct2D::GetInstance()->GetRenderTarger()->CreateSolidColorBrush(ColorF(color.r, color.b, color.g, color.a), &brush);
	//GRAPHICMANAGER->GetRenderTarget()->CreateSolidColorBrush(ColorF(color.r, color.b, color.g, color.a), &brush);
	GRAPHICMANAGER->GetRenderTarget()->CreateSolidColorBrush(color, &brush);
	_layout->SetDrawingEffect((IUnknown*)brush, { (UINT32)startPoint, (UINT32)length });
	brush->Release();
}

void Text::SetUnderline(bool isUnderline, int startPoint, int length)
{
	_layout->SetUnderline(isUnderline, { (UINT32)startPoint, (UINT32)length });
}

void Text::SetAnchor(AnchorPoint anchor)
{
	switch (anchor)
	{
	case AnchorPoint::LEFT_TOP:
		_layout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		_layout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		break;
	case AnchorPoint::LEFT_CENTER:
		_layout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		_layout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		break;
	case AnchorPoint::LEFT_BOTTOM:
		_layout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		_layout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
		break;
	case AnchorPoint::CENTER_TOP:
		_layout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		_layout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		break;
	case AnchorPoint::CENTER:
		_layout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		_layout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		break;
	case AnchorPoint::CENTER_BOTTOM:
		_layout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		_layout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
		break;
	case AnchorPoint::RIGHT_TOP:
		_layout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
		_layout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		break;
	case AnchorPoint::RIGHT_CENTER:
		_layout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
		_layout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		break;
	case AnchorPoint::RIGHT_BOTTOM:
		_layout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
		_layout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
		break;
	}
}