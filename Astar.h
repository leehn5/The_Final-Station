#pragma once
#include"Tile.h"
class Astar
{
public:
	enum dir
	{
		Left = -1,
		Right = 1,
		Up = -1,
		Down = 1,

	};
private:

	int maxX;
	int maxY;

	vector<Tile*>_vTotalList;
	vector<Tile*>::iterator _viTotalList;

	vector <Tile*> _OpenList;
	vector <Tile*>::iterator _iOpenList;

	vector <Tile*> _ClosedList;
	vector <Tile*>::iterator _iClosedList;

	list<Vector2> _pathList;

	Tile* _startTile;
	Tile* _endTile;
	Tile* _currentTile;

	//요건 나중에 테스트할때 쓸것.
	bool _start;
	int _count;

public:
	Astar();
	~Astar();

	void Init(vector<Tile*> tiles, int x, int y);
	void SetTiles(vector<Tile*> tiles);

	void InitTotalList();

	vector <Tile*> GetDirList(Vector2 idx);
	vector <Tile*> GetDirListFor4Way(Vector2 idx);

	list<Vector2> PathFinder(Vector2 start, Vector2 end);
	list<Vector2> PathFinderFor4Way(Vector2 start, Vector2 end);
	list<Vector2> PathFinderForIndex(Vector2 start, Vector2 end);
	list<Vector2> PathFinderForIndexFor4Way(Vector2 start, Vector2 end);

	bool CanOpenLeft(Vector2 idx);
	bool CanOpenRight(Vector2 idx);
	bool CanOpenUp(Vector2 idx);
	bool CanOpenDown(Vector2 idx);

	bool SetCost(Tile* node, float cost, Tile* parent);

	void AddOpenList(Tile* node);
	void AddCloseList(Tile* node);

	Tile* GetMinFNode();

	void SetPathcList();
};

