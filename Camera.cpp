#include "stdafx.h"
#include "Camera.h"
#include <math.h>

HRESULT Camera::Init()
{
	UpdateMatrix();

	return S_OK;
}

void Camera::Update()
{
	ShakingCamera();

	if (!_isMoving)
	{
		UpdateMatrix();
		return;
	}

	_lerpCount += 1.0f / 60 / _moveTime;
	_pos = Vector2::Lerp(_startPos, _endPos, _lerpCount);

	if (_lerpCount >= 1)
	{
		_isMoving = false;
		_lerpCount = 0;
	}

	UpdateMatrix(); 
}

void Camera::UpdateMatrix()
{
	//_matrix = Matrix3x2F::Identity();
	_matrix = Matrix3x3::identity;

	//_matrix = _matrix * _scaleMatrix * Matrix3x2F::Translation(-_pos.x, -_pos.y);



	_translationMatrix = Matrix3x3( 1, 0, 0,
									0, 1, 0,
									-_pos.x, -_pos.y, 1);

	_rotationMatrix = Matrix3x3(cosf(0.0f), -sinf(0.0f), 0, 
								sinf(0.0f), cosf(0.0f), 0, 
								0, 0, 1);

	_scaleMatrix = Matrix3x3(	_scale.x, 0, 0,
								0, _scale.y, 0,
								0, 0, 1);

	//_originTransMatrix = Matrix3x3(_origin.x, 0, 0, 0, _origin.y, 0, 0, 0, 1);
	
	_matrix = _scaleMatrix *_rotationMatrix * _translationMatrix;
	//
	//_inverseMatrix = _matrix.GetInverseMatrix();
}

void Camera::SetPosition(Vector2 pos, string key)
{
	_pos.x = pos.x - WINSIZEX / 2;
	_pos.y = pos.y - WINSIZEY / 2;

	if (_pos.x <= 0) _pos.x = 0;
	if (_pos.y <= 0) _pos.y = 0;

	if (_pos.x + WINSIZEX >= GRAPHICMANAGER->FindImage(key)->GetWidth()) _pos.x = GRAPHICMANAGER->FindImage(key)->GetWidth() - WINSIZEX;
	if (_pos.y + WINSIZEY >= GRAPHICMANAGER->FindImage(key)->GetHeight()) _pos.y = GRAPHICMANAGER->FindImage(key)->GetHeight() - WINSIZEY;
	UpdateMatrix();
}

void Camera::SetPositionX(Vector2 pos, string key)
{
	_pos.x = pos.x - WINSIZEX / 2;
	_pos.y = pos.y - WINSIZEY / 2;

	if (_pos.x <= 0) _pos.x = 0;

	if (_pos.x + WINSIZEX >= GRAPHICMANAGER->FindImage(key)->GetWidth()) _pos.x = GRAPHICMANAGER->FindImage(key)->GetWidth() - WINSIZEX;
	UpdateMatrix();
}

void Camera::SetPosition(Vector2 pos)
{
	pos *= _scale;
	_pos.x = pos.x - WINSIZEX / 2;
	_pos.y = pos.y - WINSIZEY / 2;
	UpdateMatrix();
}

void Camera::SetPos(Vector2 pos)
{
	//_pos += pos;
	//_pos += pos * _scale;
	_pos = pos * _scale;
}

void Camera::SlowFollow(Vector2 pos, float speed)
{
	//if (10 < Vector2::Distance(_pos, pos))
	//{
	//	float angle = Vector2::GetAngle(_pos, pos);
	//	_pos += Vector2(cosf(angle), -sinf(angle)) * speed * TIMEMANAGER->getElapsedTime();
	//}

	//if (0 <= fabs(Vector2::Distance(_pos, pos)))
	//{
	//	_pos = Vector2::Lerp(_pos, pos, 1.0f);
	//}

	if (_isMove2)
	{
		float angle = Vector2::GetAngle(_pos, pos);
		_pos += Vector2(cosf(angle), -sinf(angle)) * speed * TIMEMANAGER->getElapsedTime();
		if (0 < fabs(Vector2::Distance(_pos, pos))) _isMove2 = false;
	}

	if (5 < Vector2::Distance(_pos, pos)) _isMove2 = true;
}

void Camera::Control()
{
	if (KEYMANAGER->isStayKeyDown('A'))
	{
		_pos += Vector2(-_speed, 0.f) * TIMEMANAGER->getElapsedTime();
		UpdateMatrix();
	}
	else if (KEYMANAGER->isStayKeyDown('D'))
	{
		_pos += Vector2(_speed, 0.f) * TIMEMANAGER->getElapsedTime();
		UpdateMatrix();
	}

	if (KEYMANAGER->isStayKeyDown('W'))
	{
		_pos += Vector2(0.f, -_speed) * TIMEMANAGER->getElapsedTime();
		UpdateMatrix();

	}
	else if (KEYMANAGER->isStayKeyDown('S'))
	{
		_pos += Vector2(0.f, _speed) * TIMEMANAGER->getElapsedTime();
		UpdateMatrix();
	}
}

void Camera::ShakingCamera()
{
#pragma region Shaking Way 1
	/*if (!_isShaking) return;

	if (_shakingTime >= 0)
	{
		_pos.x = RND->getFromFloatTo(-30.f, 30.f) * _amount + _prevPos.x;
		_pos.y = RND->getFromFloatTo(-30.f, 30.f) * _amount + _prevPos.y;

		_shakingTime -= TIMEMANAGER->getElapsedTime();
	}
	else
	{
		_isShaking = false;
		_pos = _prevPos;
	}*/
#pragma endregion

#pragma region Shaking Way 2
	if (!_isShaking) return;

	if (_shakingTime >= 0)
	{
		//_pos.x = RND->getFromFloatTo(-3.f, 3.f) * _amount + _prevPos.x;
		//_pos.y = RND->getFromFloatTo(-3.f, 3.f) * _amount + _prevPos.y;

		_lastPos.x = RND->getFromFloatTo(-1.f, 1.f) * _amount + _pos.x;
		_lastPos.y = RND->getFromFloatTo(-1.f, 1.f) * _amount + _pos.y;

		_pos = Vector2::Lerp(_pos, _lastPos, 1.0f);

		_shakingTime -= TIMEMANAGER->getElapsedTime();
	}
	else
	{
		_isShaking = false;
		//_pos = _prevPos;
	}
#pragma endregion
}

void Camera::ShakingSetting(float time, float amount)
{
	_isShaking = true;
	_shakingTime = time;
	_amount = amount;
}

void Camera::SetScale(Vector2 scale)
{

	_scale += scale;

	float a = Clamp(_scale.x, ZOOM_MIN, ZOOM_MAX);
	_scale = Vector2(a, a);
	
	_scaleMatrix = Matrix3x3(_scale.x,  0,		0,
								0,	_scale.y,	0,
								0,		0,		1);

	//_scaleMatrix = Matrix3x2F::Scale(_scale.x, _scale.y, Point2F(_pos.x + WINSIZEX / 2, _pos.y + WINSIZEY / 2));

	//_pos *= _scale;

	UpdateMatrix();
}

void Camera::MoveTo(Vector2 endPos, float time, bool isCenter)
{
	_startPos = _pos;
	if (isCenter) _endPos = Vector2(endPos.x - WINSIZEX / 2, endPos.y - WINSIZEY / 2);
	else _endPos = endPos;
	_moveTime = time;
	_isMoving = true;
}

Vector2 Camera::GetPosition()
{
	return _pos / _scale;
}

Vector2 Camera::GetWorldToCamera(Vector2 pos)
{
	pos *= _scale;
	pos.x -= _pos.x;
	pos.y -= _pos.y;
	return std::move(pos);
}

Vector2 Camera::GetCameraToWorld(Vector2 pos)
{
	pos.x += _pos.x;
	pos.y += _pos.y;
	pos /= _scale.x;
	return std::move(pos);
}
