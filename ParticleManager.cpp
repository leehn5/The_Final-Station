#include "stdafx.h"
#include "ParticleManager.h"



//파티클 풀
void ParticlePool::InssertPool(int num)
{
	ActivePool[num]->SetIsActive(false);
	if (ActivePool[num]->GetIsPhysics())
	{
		ActivePool[num]->GetPhysics()->SetBodyActive(false);
		ActivePool[num]->GetPhysics()->GetBody()->SetLinearVelocity(b2Vec2(0, 0));
		ActivePool[num]->GetPhysics()->SetSensor(true);
	}
	pool.push(ActivePool[num]);
	ActivePool.erase(ActivePool.begin() + num);
}

void ParticlePool::InssertActiveObject()
{
	pool.top()->SetIsActive(true);
	if (pool.top()->GetIsPhysics())
	{
		pool.top()->GetPhysics()->SetBodyActive(true);
		pool.top()->GetPhysics()->SetBodyPosition();
	}
	ActivePool.push_back(pool.top());
	pool.pop();
}

void ParticlePool::Init(int size, string imgKey, bool isFrame, float FPS, bool isPhysics)
{
	ActivePool.reserve(size);
	for (int i = 0; i < size; i++)
	{
		Particle* newObj = Object::CreateObject<Particle>();
		newObj->Init(RND->getFromFloatTo(_Mgr->GetMinAngle(), _Mgr->GetMaxAngle()),
			RND->getFromFloatTo(_Mgr->GetMinSpeed(), _Mgr->GetMaxSpeed()),
			_Mgr->GetPos(), _Mgr->GetScale(), imgKey, isFrame, FPS, isPhysics
		);
		newObj->SetIsActive(false);
		pool.push(newObj);

	}
}

//파티클 매니저
ParticleManager::ParticleManager()
{
	_maxAngle = 140 * DegToRad;
	_minAngle = 40 * DegToRad;
	_maxSpeed = 20;
	_minSpeed = 10;
	_range = 50;
	_angle = PI;
	_minX = 50;
	_maxX = 50;

}

void ParticleManager::Init(int size, ParticleType type, Vector2 pos, Vector2 scale, string imgKey, bool isFrame, float FPS, bool isPhysics)
{
	_type = type;
	_pos = pos;
	_scale = scale;
	_pool.SetLink(this);
	_pool.Init(size, imgKey, isFrame, FPS, isPhysics);

	//pop = GRAPHICMANAGER->FindImage("pop");

}

void ParticleManager::Update()
{
	if (!_isActive)return;
	for (Particle* p : _pool.GetActivePool())
		p->Update();
	switch (_type)
	{
	case TRIANGLE:
	{
		Triangle();
	}
		break;
	case CIRCLE:
	{
		Circle();
	}
		break;
	case POSITION:
	{
		Position();
	}
		break;
	}
	KeyCon();
}

void ParticleManager::Render()
{

	for (int i = 0; i < _pool.GetActivePool().size(); i++)
	{
		if (KEYMANAGER->isToggleKey(VK_F6))
			GRAPHICMANAGER->DrawRect(_pool.GetActivePool()[i]->GetTrans()->GetPos(), _pool.GetActivePool()[i]->GetTrans()->GetScale(), _pool.GetActivePool()[i]->GetTrans()->GetRotateDegree(), ColorF::Red);
	}
	for (Particle* p : _pool.GetActivePool())
		p->Render();
	switch (_type)
	{
	case TRIANGLE:
	{
		GRAPHICMANAGER->DrawLine(_pos, _pos + Vector2(cosf(_minAngle), -sinf(_minAngle)) * 200, D2D1::ColorF::Red, 1.f, true);
		GRAPHICMANAGER->DrawLine(_pos, _pos + Vector2(cosf(_maxAngle), -sinf(_maxAngle)) * 200, D2D1::ColorF::Red, 1.f, true);
	}
	break;
	case CIRCLE:
	{
		GRAPHICMANAGER->DrawRect(_pos, Vector2(_range, _range), 0, ColorF::Red, PIVOT::CENTER, 2);
	}
	break;
	case POSITION:
	{
		GRAPHICMANAGER->DrawLine(_pos - Vector2(cosf(_angle), -sinf(_angle)) * _minX, _pos + Vector2(cosf(_angle), -sinf(_angle)) *_maxX, D2D1::ColorF::Red, 1.f, true);
	}
	break;
	}


	//pop->Render(_pos+Vector2::down*40, Vector2(120,150),PIVOT::BOTTOM);
	wchar_t buffer[1024];
	swprintf(buffer, 128, L"minAngle : %f  maxAngle : %f", _minAngle * RadToDeg, _maxAngle * RadToDeg);
	GRAPHICMANAGER->Text(Vector2(WINSIZEX / 2+100, 0), buffer, 20, 500, 50, ColorF::Azure);

	swprintf(buffer, 128, L"minSpeed : %f  maxSpeed : %f", _minSpeed, _maxSpeed);
	GRAPHICMANAGER->Text(Vector2(WINSIZEX / 2+100, 50), buffer, 20, 500, 50, ColorF::Azure);

	swprintf(buffer, 128, L"minX : %f  maxX : %f", _minX, _maxX);
	GRAPHICMANAGER->Text(Vector2(WINSIZEX / 2+100, 100), buffer, 20, 500, 50, ColorF::Azure);

	swprintf(buffer, 128, L"angle : %f", _angle);
	GRAPHICMANAGER->Text(Vector2(20, 0), buffer, 20, 300, 50, ColorF::Azure);

	swprintf(buffer, 128, L"range : %f", _range);
	GRAPHICMANAGER->Text(Vector2(20, 50), buffer, 20, 300, 50, ColorF::Azure);

	switch (_type)
	{
	case TRIANGLE:
		swprintf(buffer, 128, L"Type : TRIANGLE");
	break;
	case CIRCLE:
		swprintf(buffer, 128, L"Type : TRIANGLE");
		break;
	case POSITION:
		swprintf(buffer, 128, L"Type : POSITION");
	break;
	}
	GRAPHICMANAGER->Text(Vector2(20, 100), buffer, 20, 300, 50, ColorF::Azure);
}

void ParticleManager::Triangle()
{
	if (_pool.GetPool().size())
	{
		_pool.GetPoolObject()->SetPos(_pos);
		_pool.GetPoolObject()->SetRangeAngleToRadian(RND->getFromFloatTo(_minAngle, _maxAngle));
		_pool.GetPoolObject()->SetRotateToRadian(RND->getFromFloatTo(0, PI2));
		_pool.GetPoolObject()->SetSpeed(RND->getFromFloatTo(_minSpeed, _maxSpeed));
		_pool.GetPoolObject()->SetAlpha(1.f);
		_pool.GetPoolObject()->SetActive(true);
		_pool.InssertActiveObject();
	}

	for (int i = 0; i < _pool.GetActivePool().size(); i++)
	{
		if (!_pool.GetActivePool()[i]->GetIsActive())
		{
			_pool.GetActivePool()[i]->SetRotateToRadian(RND->getFromFloatTo(0, PI2));
			_pool.InssertPool(i);
		}
	}

	


}

void ParticleManager::Circle()
{
	if (_pool.GetPool().size())
	{
		Vector2 randPos = Vector2(RND->getFromFloatTo(_pos.x - _range / 2, _pos.x + _range / 2), RND->getFromFloatTo(_pos.y - _range / 2, _pos.y + _range / 2));
		_pool.GetPoolObject()->SetPos(randPos);
		int rand = RND->getInt(8);
		float angle;
		switch (rand)
		{
		case 0:
			angle = 0;
			break;
		case 1:
			angle = PI;
			break;
		case 2:
			angle = PI / 2;
			break;
		case 3:
			angle = -PI / 2;
			break;
		case 4:
			angle = -PI / 4;
			break;
		case 5:
			angle = PI / 4;
			break;
		case 6:
			angle = -(3 * PI) / 4;
			break;
		case 7:
			angle = (3 * PI) / 4;
			break;
		}
		_pool.GetPoolObject()->SetRangeAngleToRadian(angle);
		_pool.GetPoolObject()->SetRotateToRadian(0);
		_pool.GetPoolObject()->SetSpeed(RND->getFromFloatTo(_minSpeed, _maxSpeed));
		_pool.GetPoolObject()->SetAlpha(1.f);
		_pool.GetPoolObject()->SetActive(true);
		_pool.InssertActiveObject();
	}

	for (int i = 0; i < _pool.GetActivePool().size(); i++)
	{
		if (!_pool.GetActivePool()[i]->GetIsActive())
		{
			_pool.GetActivePool()[i]->SetRotateToRadian(RND->getFromFloatTo(0, PI2));
			_pool.InssertPool(i);
		}
	}
}

void ParticleManager::Position()
{
	//수정 예정
	if (_pool.GetPool().size())
	{
		Vector2 randPos = Vector2(RND->getFromFloatTo(_pos.x + cosf(_angle) * _minX, _pos.x - cosf(_angle) * _maxX), _pos.y-sinf(_angle));
		_pool.GetPoolObject()->SetPos(randPos);
		_pool.GetPoolObject()->SetRangeAngleToRadian(_angle - PI / 2);
		_pool.GetPoolObject()->SetRotateToRadian(0);
		_pool.GetPoolObject()->SetSpeed(RND->getFromFloatTo(_minSpeed, _maxSpeed));
		_pool.GetPoolObject()->SetAlpha(1.f);
		_pool.GetPoolObject()->SetActive(true);
		_pool.InssertActiveObject();
	}

	for (int i = 0; i < _pool.GetActivePool().size(); i++)
	{
		if (!_pool.GetActivePool()[i]->GetIsActive())
		{
			_pool.GetActivePool()[i]->SetRotateToRadian(RND->getFromFloatTo(0, PI2));
			_pool.InssertPool(i);
		}
	}
}

void ParticleManager::KeyCon()
{
	if (KEYMANAGER->isStayKeyDown(VK_UP))	_pos += Vector2::up * 10;
	if (KEYMANAGER->isStayKeyDown(VK_LEFT))	_pos += Vector2::left * 10;
	if (KEYMANAGER->isStayKeyDown(VK_RIGHT))	_pos += Vector2::right * 10;
	if (KEYMANAGER->isStayKeyDown(VK_DOWN))	_pos += Vector2::down * 10;
	if (KEYMANAGER->isOnceKeyDown('C'))
	{
		switch (_type)
		{
		case TRIANGLE:
			_type = CIRCLE;
			break;
		case CIRCLE:
			_type = POSITION;
			break;
		case POSITION:
			_type = TRIANGLE;
			break;
		}
	}

	switch (_type)
	{
	case TRIANGLE:
	{

		if (KEYMANAGER->isStayKeyDown(VK_NUMPAD1))
		{
			//if (_maxAngle > MAX_ANGLE_MAX) _maxAngle = MAX_ANGLE_MAX;
			//else if (_maxAngle < MAX_ANGLE_MIN) _maxAngle = MAX_ANGLE_MIN;
			//
			//if (_minAngle > MIN_ANGLE_MAX) _minAngle = MIN_ANGLE_MAX;
			//else if (_minAngle < MIN_ANGLE_MIN) _minAngle = MIN_ANGLE_MIN;

			_maxAngle += 0.03;
			_minAngle -= 0.03;
		}
		if (KEYMANAGER->isStayKeyDown(VK_NUMPAD2))
		{
			//if (_maxAngle > MAX_ANGLE_MAX) _maxAngle = MAX_ANGLE_MAX;
			//else if (_maxAngle < MAX_ANGLE_MIN) _maxAngle = MAX_ANGLE_MIN;
			//
			//if (_minAngle > MIN_ANGLE_MAX) _minAngle = MIN_ANGLE_MAX;
			//else if (_minAngle < MIN_ANGLE_MIN) _minAngle = MIN_ANGLE_MIN;
			_maxAngle -= 0.03;
			_minAngle += 0.03;
		}

		if (KEYMANAGER->isStayKeyDown(VK_NUMPAD3))
		{
			_maxSpeed += 1;
			_minSpeed += 1;
		}
		if (KEYMANAGER->isStayKeyDown(VK_NUMPAD4))
		{
			_maxSpeed -= 1;
			_minSpeed -= 1;
		}

	}
	break;
	case CIRCLE:
	{
		if (KEYMANAGER->isStayKeyDown(VK_NUMPAD1))
		{

			_range += 1;
		}
		if (KEYMANAGER->isStayKeyDown(VK_NUMPAD2))
		{
			_range -= 1;
		}
		if (KEYMANAGER->isStayKeyDown(VK_NUMPAD3))
		{
			_maxSpeed += 1;
			_minSpeed += 1;
		}
		if (KEYMANAGER->isStayKeyDown(VK_NUMPAD4))
		{
			_maxSpeed -= 1;
			_minSpeed -= 1;
		}

	}
	break;
	case POSITION:
	{
		if (KEYMANAGER->isStayKeyDown(VK_NUMPAD1))
		{
			_maxX += 1;
			_minX += 1;
		}
		if (KEYMANAGER->isStayKeyDown(VK_NUMPAD2))
		{
			_maxX -= 1;
			if (_maxX <= 0)_maxX = 0;
			_minX -= 1;
			if (_minX <= 0)_minX = 0;
		}

		if (KEYMANAGER->isStayKeyDown(VK_NUMPAD3))
		{
			_maxSpeed += 1;
			_minSpeed += 1;
		}
		if (KEYMANAGER->isStayKeyDown(VK_NUMPAD4))
		{
			_maxSpeed -= 1;
			_minSpeed -= 1;
		}
		if (KEYMANAGER->isStayKeyDown(VK_NUMPAD5))
		{
			_angle += 0.05;
		}

	}
	break;
	}

}
