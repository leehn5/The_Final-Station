#include "stdafx.h"
#include "Transform.h"

void Transform::UpdateMatrix()
{
	scaleMatrix.m[0][0] = scale.x;
	scaleMatrix.m[1][1] = scale.y;

	rotateMatrix.m[0][0] = cos(_rotate*Deg2Rad);
	rotateMatrix.m[0][1] = -sin(_rotate*Deg2Rad);
	rotateMatrix.m[1][0] = sin(_rotate*Deg2Rad);
	rotateMatrix.m[1][1] = cos(_rotate*Deg2Rad);

	translateMatrix.m[2][0] = pos.x;
	translateMatrix.m[2][1] = pos.y;

	localToWorldMatrix = scaleMatrix * rotateMatrix * translateMatrix;

	Object* parent = _object->GetParent();
	while (true)
	{
		if (parent == nullptr)
			break;
		localToWorldMatrix = localToWorldMatrix * parent->GetTrans()->GetLocalToWorldMatrix();
		parent = parent->GetParent();
	}
}

Transform::Transform()
{
	pos = Vector2::zero;
	scale = Vector2(1, 1);

	scaleMatrix = Matrix3x3::identity;
	rotateMatrix = Matrix3x3::identity;
	translateMatrix = Matrix3x3::identity;
	localToWorldMatrix = Matrix3x3::identity;
}

void Transform::Init()
{
	_rotate = 0;
	SetRect();
	UpdateMatrix();

}

void Transform::Update()
{
	SetRect();
	UpdateMatrix();	 
}

Vector2 Transform::GetPos()
{
	return pos;
}

Vector2 Transform::GetBottomPos()
{
	return bottomPos;
}

Vector2 Transform::GetTopPos()
{
	return topPos;
}

Vector2 Transform::GetPosToPivot(TransFormPIVOT pivot)
{
	switch (pivot)
	{
	case TransFormPIVOT::LEFT_TOP:
	{
		return Vector2(pos - scale / 2);
	}
		break;
	case TransFormPIVOT::LEFT_BOTTOM:
	{

		return Vector2(pos.x - scale.x / 2, pos.y + scale.y / 2);
	}
		break;
	case TransFormPIVOT::RIGHT_TOP:
	{

		return Vector2(pos.x + scale.x / 2, pos.y - scale.y / 2);
		
	}
		break;
	case TransFormPIVOT::RIGHT_BOTTOM:
	{

		return Vector2(pos + scale / 2);
	}
		break;
	case TransFormPIVOT::LEFT:
	{

		return Vector2(pos.x - scale.x / 2, pos.y);
	}
		break;
	case TransFormPIVOT::RIGHT:
	{

		return Vector2(pos.x + scale.x / 2, pos.y);
	}
		break;
	case TransFormPIVOT::TOP:
	{

		return Vector2(pos.x, pos.y - scale.y / 2);
	}
	break;
	case TransFormPIVOT::BOTTOM:
	{

		return Vector2(pos.x, pos.y + scale.y / 2);
	}
	break;
	}
}

void Transform::SetPos(Vector2 pos)
{
	this->pos = pos;
}
void Transform::SetPos(float x, float y)
{
	pos = Vector2(x, y);
}

void Transform::SetWorldPos(Vector2 pos)
{
	Object* parent = _object->GetParent();
	Vector2 worldPos = Vector2::zero;

	while (parent != nullptr)
	{
		worldPos += parent->GetTrans()->GetPos();
		parent = parent->GetParent();
	}
	this->pos = pos - worldPos;
}

RECT Transform::GetRect()
{

	return _rc;
}

void Transform::SetRect()
{
	_rc = RectMakeCenter(pos.x, pos.y, scale.x, scale.y);
	bottomPos = Vector2(pos.x, pos.y + scale.y / 2);
	topPos = Vector2(pos.x, pos.y - scale.y / 2);
}


Vector2 Transform::GetWorldPosition()
{
	Object* parent = _object->GetParent();
	Vector2 position = this->pos;

	while (true)
	{
		if (parent == nullptr)
			return position;
		position += parent->GetTrans()->GetPos();
		parent = parent->GetParent();
	}
}

 Vector2 Transform::GetScale()
{

	// Vector2(scale.x * CAMERA->GetScale().x, scale.y * CAMERA->GetScale().y); 

	// if (!_object->GetCameraAffect()) return scale * CAMERA->GetScale();
	return scale;
}
