#pragma once
#include "Object.h"
#include "Transform.h"
class Particle : public Object
{
private:
	Sprite* _sprite;
	PhysicsBody* _physics;

	float _alphaSpeed;

	float _speed;
	float _alpha;
	float _angle;

	float _rotate;

	bool _active;
	bool _isPhysics;
public:
	Particle();
	virtual void Init(float angle, float speed, Vector2 pos, Vector2 scale, string imgKey = "None", bool isFrame = false, float FPS = 0, bool isPhysics = false);
	virtual void Update();
	virtual void Render();

	bool GetIsPhysics() { return _isPhysics; }
	void SetIsPhysics();
	float GetRotateToDegree() { return _trans->GetRotateDegree(); }
	float GetRotateToRadian() { return _trans->GetRotateRadian(); }

	void SetRotateToDegree(float degree) { _trans->SetRotateToDegree(degree); }
	void SetRotateToRadian(float radian) { _trans->SetRotateToRadian(radian); }

	void SetRangeAngleToDegree(float degree) { _angle = degree * DegToRad; }
	void SetRangeAngleToRadian(float radian) { _angle = radian; }

	void SetSpeed(float speed) { _speed = speed; }

	// 1 ~ 100 까지 (초기 값 : 40)
	void SetAlphaSpeed(float speed);
	void move();

	void Fire();
	void SetActive(bool active) { _active = active; }
	Sprite* GetSprite() { return _sprite; }
	PhysicsBody* GetPhysics() { return _physics; }
	void SetPhysicsOn(bool active);

	//Vector2 GetPos() { _trans->GetPos(); }
	void SetPos(Vector2 pos) { _trans->SetPos(pos); }
	void SetAlpha(float alpha) { _alpha = alpha; }
};

#include"Particle.hpp"