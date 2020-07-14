#pragma once
#include "Scene.h"
#include "FadeInAndOut.h"

class Player;
class Tile;
class UI;
class EnemyManager;
class ItemManager;

class UndergroundScene : public Scene
{
private:
	Player* _player;
	vector<Tile*> _vTiles;

	Graphic* _cursorImg;
	UI* _ui;
	EnemyManager* _enemyMgr;
	ItemManager* _itemMgr;
	FadeInAndOut* _fade;

	bool _isShowRect;
	int _tileIndex;

	float _timer;

private:
	void MapLoad();
	void AddImage();
	void DirectorySearch(string folderPath, int maxFrameX = 1, int maxFrameY = 1);

public:
	virtual void Init();
	virtual void Release();
	virtual void Update();
	virtual void Render();

	vector<Tile*> GetTiles() { return _vTiles; }
	void SetTileIndex(int index) { _tileIndex = index; }
	int GetTileIndex() { return _tileIndex; }
};

