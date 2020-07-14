#pragma once
#include "Object.h"
#include "FoodObject.h"
#include "MedkitObject.h"
#include "VentObject.h"

enum TrainMode
{
	Show_In,
	Show_Out,
	None,
};

class Train : public Object
{
private:
	vector<Vector2> _vChair;
	Sprite* _sprite;
	Sprite* _wheels;
	TrainMode _mode;
	Object* _vant;
	Object* _stops;

	FoodObject* _food;
	MedkitObject* _medkit;
	VentObject* _vent;

	bool _isArrive;

	Vector2 _chairs[6];

public:
	Train() { _mode = TrainMode::Show_In; }
	virtual void Init();
	virtual void Release();
	virtual void Update();
	virtual void Render();

	Sprite* GetSprite() { return _sprite; }
	Sprite* GetWheelsSprite() { return _wheels; }

	void SetTrainMode(TrainMode mode) { _mode = mode; }
	TrainMode GetTrainMode() { return _mode; }

	void ArriveAtTheStation();
	Object* GetStops() { return _stops; }

	FoodObject* GetFoodObject() { return _food; }
	MedkitObject* GetMedkitObject() { return _medkit; }
	VentObject* GetVentObject() { return _vent; }

	Vector2 GetChairPosition(int index) { return _chairs[index]; }
};

