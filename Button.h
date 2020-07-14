#pragma once

//typedef void (*Callback_Function)(void);
//typedef void (*Callback_Function_Parameter)(void*);

typedef enum class ButtonDirection
{
	None,
	Up,
	Down,
	PlaceOn,
}BtnDir;

class Button
{
private:
	BtnDir _dir;
	Graphic* _sprite;
	Callback_Function _callback;
	Callback_Function_Parameter _callbackParameter;
	ColorF _hangOnColor = NULL;
	POINT _upFrame;
	POINT _downFrame;

	Vector2 _pos;
	Vector2 _size;
	Vector2 _scale;

	void* _obj;

public:
	Button() : _hangOnColor(ColorF::LightGray, 0.3f){};
	virtual ~Button() {};

	virtual void Init();
	virtual void Release();
	virtual void Update();
	virtual void Render();

	void Setting(string imgKey, Vector2 pos, POINT up, POINT down, Callback_Function cb);
	void Setting(string imgKey, Vector2 pos, POINT up, POINT down, Callback_Function_Parameter cbp, void* obj);

	void SetHangOnColor(ColorF color) { _hangOnColor = color; }
	void SetScale(Vector2 scale);
};

