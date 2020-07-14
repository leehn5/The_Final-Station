#include "stdafx.h"
#include "TextButton.h"

void TextButton::Init()
{
	Object::Init();

	_sprite = AddComponent<Sprite>();
	_sprite->Init();
	//_sprite->SetShowRect(true);
}

void TextButton::Update()
{
	Object::Update();

	if (PtInRect(&_trans->GetRect(), MOUSEPOINTER->GetMouseWorldPosition().Vector2ToPOINT()))
	{
		if (_leftBtnDown) _dir = TxtBtnDir::Down;
		else if (not _leftBtnDown and _dir == TxtBtnDir::Down)
		{
			_dir = TxtBtnDir::Up;
			if (_callback != nullptr) _callback();
			else if (_obj != nullptr) _callbackParameter(_obj);
		}
		else _dir = TxtBtnDir::PlaceOn;
	}
	else _dir = TxtBtnDir::None;
}

void TextButton::Render()
{
	Object::Render();

	ColorF textColor = ColorF::Black;

	switch (_dir)
	{
	case TxtBtnDir::Up: case TxtBtnDir::None: textColor = _upColor; break;
	case TxtBtnDir::Down: textColor = _downColor; break;
	case TxtBtnDir::PlaceOn: textColor = _downColor; break;
	}

	GRAPHICMANAGER->Text(_trans->GetPos() - Vector2(_trans->GetScale().x/2, _trans->GetScale().y/2), _txt, _fontSize, _trans->GetScale().x, _trans->GetScale().y, textColor, TextPivot::LEFT_BOTTOM, _fontName);
}

void TextButton::Setting(string txt, Vector2 pos, float fontSize, Vector2 maxSize, ColorF upColor, ColorF downColor, wstring fontName, Callback cb)
{
	Object::Init();

	_callback = static_cast<Callback>(cb);
	_callbackParameter = nullptr;
	_dir = TxtBtnDir::None;

	_obj = nullptr;

	string text = txt;
	_txt.assign(text.begin(), text.end());

	_fontSize = fontSize;

	_trans->SetPos(pos);
	_trans->SetScale(maxSize);

	_upColor = upColor;
	_downColor = downColor;

	_fontName = fontName;
}

void TextButton::Setting(string txt, Vector2 pos, float fontSize, Vector2 maxSize, ColorF upColor, ColorF downColor, wstring fontName, Callback_Parameter cbp, void* obj)
{
	Object::Init();

	_callback = nullptr;
	_callbackParameter = static_cast<Callback_Parameter>(cbp);
	_dir = TxtBtnDir::None;

	_obj = obj;

	string text = txt;
	_txt.assign(text.begin(), text.end());

	_fontSize = fontSize;

	_trans->SetPos(pos);
	_trans->SetScale(maxSize);

	_upColor = upColor;
	_downColor = downColor;

	_fontName = fontName;
}

void TextButton::ChangeText(string txt, Vector2 maxSize)
{
	string text = txt;
	_txt.assign(text.begin(), text.end());

	_trans->SetScale(maxSize);
}
