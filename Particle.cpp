#include "stdafx.h"
#include "Particle.h"

Particle::Particle()
{
	_tag = "Particle";
	_sprite = AddComponent<Sprite>();
}

void Particle::Init(float angle, float speed, Vector2 pos, Vector2 scale, string imgKey, bool isFrame, float FPS, bool isPhysics)
{
	Object::Init();
	_alphaSpeed = 0.4f;
	_angle = angle;
	_speed = speed;
	_trans->SetPos(pos);
	_trans->SetScale(scale);
	_alpha = 1.f;
	_sprite->SetAlpha(_alpha);
	_isPhysics = isPhysics;
	if (isPhysics)
	{
	_physics = AddComponent<PhysicsBody>();
	_physics->Init(BodyType::DYNAMIC, 0, 0, 0.6, true, true);
	_physics->SetBodyActive(false);
	_physics->GetBody()->SetFixedRotation(true);
	}
	if (imgKey != "None") 
	{
		if (isFrame)
		{
			_sprite->Init(true, true);
			_sprite->SetFPS(FPS);
		}
		else
		{
			_sprite->Init();
		}
		_sprite->SetImgName(imgKey);
	}

	_sprite->SetFillRect(true);
	_sprite->SetRectColor(ColorF(ColorF::AntiqueWhite,_alpha));
	_sprite->SetPosition(_trans->GetPos());
	_rotate = 0;
	DelParent();
}

void Particle::Update()
{
	if (!_isActive) return;

	Object::Update();

	_alpha -= _alphaSpeed * TIMEMANAGER->getElapsedTime();

	if (_alpha <= 0.f) _isActive = false;

	if (_alpha <= 0.5 && _isPhysics)_physics->SetSensor(false);

	move();

	_rotate += 0.9 * TIMEMANAGER->getElapsedTime();
	_trans->SetRotateToRadian(_rotate);
}

void Particle::Render()
{
	if (!_isActive) return;
	Object::Render();
}

void Particle::SetIsPhysics()
{
	if (_physics) return;

	_isPhysics = true;
	_physics = AddComponent<PhysicsBody>();
	_physics->Init(BodyType::DYNAMIC, 0, 0, 0.6, true, true);
	_physics->SetBodyActive(false);
	_physics->GetBody()->SetFixedRotation(true);

}

void Particle::SetAlphaSpeed(float speed)
{
	_alphaSpeed = speed / 100;
	if (_alphaSpeed < 0) _alphaSpeed = 0.f;
	if (_alphaSpeed > 1)_alphaSpeed = 1.f;
}

void Particle::move()
{
	if (_isPhysics)
	{
		if (_active)
		{
			Fire();
			_active = false;
		}
	}
	else
		_trans->SetPos(_trans->GetPos() + Vector2(cosf(_angle), -sinf(_angle)) * _speed * TIMEMANAGER->getElapsedTime());

	_sprite->SetPosition(_trans->GetPos());

	_sprite->SetAlpha(_alpha);
	
}

void Particle::Fire()
{
	_physics->ApplyForce(b2Vec2(cosf(_angle), -sinf(_angle)) * _speed);
}

void Particle::SetPhysicsOn(bool active)
{

	_isPhysics = active;

	if (_isPhysics)
	{
		_physics->SetBodyActive(false);
	}
	else
	{
		_physics->SetBodyActive(true);
	}

}
