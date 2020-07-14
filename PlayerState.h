#pragma once

class Player;

class PlayerState abstract
{
protected:
	Player* _obj;
	string _state;

public:
	PlayerState() {}
	PlayerState(Player* obj) : _obj(obj) {}

	virtual void Enter() {}
	virtual void Update() {}
	virtual void Exit() {}

	string GetState() { return _state; }
};

