#pragma once

typedef void (*Callback)(void);
typedef void (*Callback_Parameter)(void*);

typedef enum class TextButtonDirection
{
	None,
	Up,
	Down,
	PlaceOn,
}TxtBtnDir;

class TextButton : public Object
{
private:
	TxtBtnDir _dir;
	Callback _callback;
	Callback_Parameter _callbackParameter;

	ColorF _hangOnColor = NULL;
	ColorF _upColor = NULL;
	ColorF _downColor = NULL;

	wstring _txt;
	wstring _fontName;

	Sprite* _sprite;

	float _fontSize;

	void* _obj;

public:
	TextButton() : _hangOnColor(ColorF::LightGray, 0.3f) {};
	virtual ~TextButton() {};

	virtual void Init();
	virtual void Update();
	virtual void Render();

	void Setting(string txt, Vector2 pos, float fontSize, Vector2 maxSize, ColorF upColor, ColorF downColor, wstring fontName, Callback cb);
	void Setting(string txt, Vector2 pos, float fontSize, Vector2 maxSize, ColorF upColor, ColorF downColor, wstring fontName, Callback_Parameter cbp, void* obj);

	void SetHangOnColor(ColorF color) { _hangOnColor = color; }
	void ChangeText(string txt, Vector2 maxSize);
};

