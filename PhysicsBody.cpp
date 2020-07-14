#include "stdafx.h"
#include "PhysicsBody.h"

void PhysicsBody::Init(BodyType type, float32 friction, float32 density, float32 restitution,BOOL isBullet, BOOL isSensor)
{
	_trans = _object->GetTrans();
	scale = _object->GetTrans()->GetScale();
	_type = type;
	_bodyActive = true;
	switch (_type)
	{
	case BodyType::DYNAMIC:
		{
			Vector2 bodyPosition = { _trans->GetPos().x, _trans->GetPos().y };
			bodyPosition = Convert(bodyPosition);

			b2BodyDef bodyDef;
			bodyDef.type = b2BodyType::b2_dynamicBody; //static은 무조건 고정,dynamic은 움직임
			bodyDef.bullet = isBullet;
			bodyDef.userData = _object;
			bodyDef.position.Set(bodyPosition.x, bodyPosition.y);
			_body = SCENEMANAGER->GetNowScene()->GetWorld()->CreateBody(&bodyDef);  //bodyDef의 내용을 바탕으로 body를 만듬
			
			Vector2 bodySize = { _trans->GetScale().x / 2.f,_trans->GetScale().y / 2.f };
			bodySize = Convert(bodySize);

			b2PolygonShape shape;
			shape.SetAsBox(bodySize.x, bodySize.y);

			b2FixtureDef fixture;
			fixture.isSensor = isSensor;//충돌함수는 실행하지만 박스를 밀어낼것인가 안밀어내것인가?
			fixture.shape = &shape;
			fixture.density = density;
			fixture.friction = friction;
			fixture.restitution = restitution;
			_body->CreateFixture(&fixture);

			break;
		}

		case BodyType::STATIC:
		{
			//Vector2 bodyPosition = { _trans->GetPos().x / 2.f, _trans->GetPos().y / 2.f };
			Vector2 bodyPosition = { _trans->GetPos().x , _trans->GetPos().y  };
			bodyPosition = Convert(bodyPosition);

			b2BodyDef bodyDef;
			bodyDef.userData = _object;
			bodyDef.position.Set(bodyPosition.x, bodyPosition.y);
			_body = SCENEMANAGER->GetNowScene()->GetWorld()->CreateBody(&bodyDef);  //bodyDef의 내용을 바탕으로 body를 만듬

			Vector2 bodySize = { _trans->GetScale().x / 2.f,_trans->GetScale().y / 2.f };
			bodySize = Convert(bodySize);

			b2PolygonShape shape;
			shape.SetAsBox(bodySize.x, bodySize.y);

			b2FixtureDef fixture;
			fixture.isSensor = isSensor;//충돌함수는 실행하지만 박스를 밀어낼것인가 안밀어내것인가?
			fixture.shape = &shape;
			fixture.density = density;
			fixture.friction = friction;
			_body->CreateFixture(&fixture);

			break;
		}

		case BodyType::KINEMATIC:
		{
			Vector2 bodyPosition = { _trans->GetPos().x , _trans->GetPos().y };
			bodyPosition = Convert(bodyPosition);

			b2BodyDef bodyDef;
			bodyDef.type = b2BodyType::b2_kinematicBody; //static은 무조건 고정,dynamic은 움직임
			bodyDef.userData = _object;
			bodyDef.position.Set(bodyPosition.x, bodyPosition.y);
			_body = SCENEMANAGER->GetNowScene()->GetWorld()->CreateBody(&bodyDef);  //bodyDef의 내용을 바탕으로 body를 만듬

			Vector2 bodySize = { _trans->GetScale().x / 2.f,_trans->GetScale().y / 2.f };
			bodySize = Convert(bodySize);

			b2PolygonShape shape;
			shape.SetAsBox(bodySize.x, bodySize.y);

			b2FixtureDef fixture;
			fixture.isSensor = isSensor;//충돌함수는 실행하지만 박스를 밀어낼것인가 안밀어내것인가?
			fixture.shape = &shape;
			fixture.density = density;
			fixture.friction = friction;
			_body->CreateFixture(&fixture);

			break;
		}
		case BodyType::CIRCLE:
		{
			Vector2 bodyPosition = { _trans->GetPos().x, _trans->GetPos().y };
			bodyPosition = Convert(bodyPosition);

			b2BodyDef bodyDef;
			bodyDef.type = b2BodyType::b2_dynamicBody; //static은 무조건 고정,dynamic은 움직임
			bodyDef.bullet = isBullet;
			bodyDef.userData = _object;
			bodyDef.position.Set(bodyPosition.x, bodyPosition.y);
			_body = SCENEMANAGER->GetNowScene()->GetWorld()->CreateBody(&bodyDef);  //bodyDef의 내용을 바탕으로 body를 만듬

			Vector2 bodySize = { _trans->GetScale().x / 2.f,_trans->GetScale().y / 2.f };
			bodySize = Convert(bodySize);

			b2CircleShape shape;
			//shape.SetAsBox(bodySize.x, bodySize.y);

			shape.m_p.Set(bodySize.x, bodySize.y);
			shape.m_radius = bodySize.x / 2;

			b2FixtureDef fixture;
			fixture.isSensor = isSensor;//충돌함수는 실행하지만 박스를 밀어낼것인가 안밀어내것인가?
			fixture.shape = &shape;
			fixture.density = density;
			fixture.friction = friction;
			fixture.restitution = restitution;
			_body->CreateFixture(&fixture);

			break;
		}
	}
}



void PhysicsBody::SetBodyPosition()
{

	Vector2 bodyPosition = { _object->GetTrans()->GetPos().x, _object->GetTrans()->GetPos().y };
	bodyPosition = Convert(bodyPosition);

	_body->SetTransform(b2Vec2(bodyPosition.x, bodyPosition.y), _body->GetAngle());
}

void PhysicsBody::Render()
{
	if (KEYMANAGER->isToggleKey(VK_F2) && _bodyActive)
	{
		if(_type != BodyType::CIRCLE)
			GRAPHICMANAGER->DrawRect(GetBodyPosition(), scale, _body->GetAngle() * RadToDeg, ColorF::Enum::Green, PIVOT::CENTER, 3.f);
		else
			GRAPHICMANAGER->DrawEllipse(GetBodyPosition().x, GetBodyPosition().y, scale.x, scale.y, ColorF::Enum::Green, 3.f);

	}
}


Vector2 PhysicsBody::GetBodyPosition()
{
	return Vector2(_body->GetPosition().x * 100.f, _body->GetPosition().y * 100.f);
}

Vector2 PhysicsBody::GetBodyScale()
{
	return scale;
}

void PhysicsBody::SetBodyActive(bool sleep)
{
	_body->SetActive(sleep);
	_bodyActive = sleep;
}

void PhysicsBody::SetSensor(bool sensor)
{
	_body->GetFixtureList()->SetSensor(sensor);
}

Vector2 PhysicsBody::Convert(Vector2 origin)
{
	return Vector2(origin.x*0.01f, origin.y*0.01f);
}
