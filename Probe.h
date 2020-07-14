#pragma once
#include "Object.h"
#include "Tile.h"
class ProceduralTest;
class Probe : public Object
{
private:
	PhysicsBody* _physics;
	list <Vector2> path;
	float speed;
	vector<Tile*> tiles;
public:
	virtual void Init(Vector2 pos);
	virtual void Update();
	virtual void Release();
	void SetPath(list<Vector2> path);
	void Move();
	void SetTileAttribute();
	void SetTiles(vector<Tile*>* tiles) { this->tiles = *tiles; }
};

