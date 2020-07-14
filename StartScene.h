#pragma once
#include "Scene.h"
#include "TextButton.h"

class StartScene : public Scene
{
private:
	shared_ptr<Graphic> _cursorImg;
	shared_ptr<Graphic> _logoImg;
	shared_ptr<Graphic> _eagleImg;
	shared_ptr<Graphic> _mainBG;

	TextButton* _txtBtn;
	TextButton* _btnMaptool;

public:
	virtual void Init();
	virtual void Update();
	virtual void Render();
	virtual void Release();

	static void cbGround();
	static void cbQuit();
	static void cbMaptool();
};

