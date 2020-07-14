#pragma once
#include "Object.h"
#include "Probe.h"
#define MAP_MAX_WIDTH 5000
#define MAP_MAX_HEIGHT 3000
class Room : public Object
{
private:
	PhysicsBody* _physics;
	bool _isMainRoom;
	bool _isSubRoom;
public:
	virtual void Init();
	virtual void Update();

	PhysicsBody* GetPhysics() { return _physics; }

	bool GetIsMainRoom() { return _isMainRoom; }
	bool GetIsSubRoom() { return _isSubRoom; }
	void SetIsMainRoom(bool active) { _isMainRoom = active; }
	void SetIsSubRoom(bool active) { _isSubRoom = active; }

};

