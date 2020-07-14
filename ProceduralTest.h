#pragma once
#include "Scene.h"
#include "Tile.h"
#include "Room.h"
#include "Astar.h"
class Probe;
#define CREATE_ROOM_MAX 150
#define SELECT_ROOM 20

#define MAP_TILE_MAX_X MAP_MAX_WIDTH / TILE_WIDTH
#define MAP_TILE_MAX_Y MAP_MAX_HEIGHT / TILE_HEIGHT

class ProceduralTest : public Scene
{
private: 
	Astar* _ast;
	vector<Tile*> tiles;
	vector<Room*> rooms;
	vector<Room*> selRooms;
	vector<Room*> subRooms;
	Room* currentRoom;
	float maxY;
	float timer;
	bool endPush;
	bool endCreate;
	bool startDel;
	int count;

	vector<Object*> _vFloors;
	
public:
	virtual void Init();
	virtual void Update();
	virtual void Release();
	virtual void Render();
	void CreateRoom();
	void PushRoom();
	void SelRoom();
	void DelRoom();
	void SetTile();
	void Exploration();
	void SetSubRoom();
	void SetTileProperty();
};

