#pragma once
#include "Scene.h"
#include "UI.h"

#define GRASS_SPEED 1500.f
#define ARRIVE_TIEM 200.f

#define HEAL_HP 80.f
#define EAT_FOOD 80.f

class Train;
class Player;
class CrossImage;
class FadeInAndOut;
class NPCManager;

class TrainScene : public Scene
{
private:
	Player* _player;
	Train* _train;
	Graphic* _cursorImg;
	Graphic* _boxImg;
	CrossImage* _sky;
	CrossImage* _ground3;
	CrossImage* _ground4;
	NPCManager* _npcMgr;

	UI* _ui;

	float _grassX;
	float _grassX_2;
	float _timer;
	
	float _txtAlpha;

	string _txt;
	Vector2 _txtPos;

	FadeInAndOut* _fade;

public:
	virtual void Init();
	virtual void Release();
	virtual void Update();
	virtual void Render();

	void Background();
};

