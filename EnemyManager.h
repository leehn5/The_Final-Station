#pragma once

class Enemy;

class EnemyManager
{
private:
	list<Enemy*> _vEnemy;

public:
	void Init();
	void Release();
	void Update();
	void Render();

	void SetEnemy(Vector2 pos);
};

