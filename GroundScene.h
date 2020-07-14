#pragma once
#include "Scene.h"

class Player;
class UI;
class Train;
class FadeInAndOut;

class GroundScene : public Scene
{
private:
	Player* _player;
	Object* _wall1;
	Object* _wall2;
	vector<Tile*> _vTiles;
	UI* _ui;
	Graphic* _cursorImg;
	Train* _train;
	FadeInAndOut* _fade;

	float _pos1 = -250.f;
	float _alpha = 1.f;

	bool _isShowRect;

public:
	virtual void Init();
	virtual void Release();
	virtual void Update();
	virtual void Render();

	void AddImage();
	void MapLoad();
	void DirectorySearch(string folderPath, int maxFrameX = 1, int maxFrameY = 1);
};

