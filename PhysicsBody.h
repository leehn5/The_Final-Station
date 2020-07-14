#pragma once
#include"DrawComponent.h"
enum class BodyType
{
	DYNAMIC,
	STATIC,
	KINEMATIC,
	CIRCLE,
};


class PhysicsBody : public DrawComponent
{
private:
	b2Body *_body;
	BodyType _type;
	Vector2 scale;
	bool _bodyActive;

public:
	PhysicsBody() { _name = "PhysicsBody"; }
	//바디 타입, 마찰력, 밀도(기본1), 반발력 (기본0), bullet인가, 뚫어도 되는 바디인가
	void Init(BodyType type, float32 friction, float32 density=1, float32 restitution = 0 , BOOL isBullet = FALSE, BOOL isSensor = FALSE);
	void SetBodyPosition();
	virtual void Render()override;

	//바디 포지션 가져오기
	Vector2 GetBodyPosition();

	Vector2 GetBodyScale();
	//바디의 Rotate 가져오기
	float GetBodyAngle() { return _body->GetAngle(); }
	bool GetBodyActive() { return _bodyActive; }
	//바디 가져오기
	inline b2Body* GetBody() { return _body; }
	//바디의 GetWorldCenter 가져오기
	inline const b2Vec2 GetWorldCenter() { return _body->GetWorldCenter(); }
	//바디의 Active 껏다 키는 세팅 (잠들건지 말건지)
	void SetBodyActive(bool sleep);
	//바디에 힘 작용 시키기
	inline void ApplyForce(b2Vec2 force, bool wake = true) { _body->ApplyForce(force, _body->GetWorldCenter(), wake); }
	//센서 세팅
	void SetSensor(bool sensor);
	Vector2 Convert(Vector2 origin);
};