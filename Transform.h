#pragma once
#include "Component.h"
#include"Matrix3x3.h"
class Matrix3x3;

typedef enum class TransFormPIVOT
{
	LEFT_TOP,
	LEFT_BOTTOM,
	RIGHT_TOP,
	RIGHT_BOTTOM,
	LEFT,
	RIGHT,
	TOP,
	BOTTOM
}TF_PIVOT;

class Transform : public Component
{
private:
	
	float _rotate;
	RECT _rc;

	Matrix3x3 scaleMatrix;
	Matrix3x3 rotateMatrix;
	Matrix3x3 translateMatrix;
	Matrix3x3 localToWorldMatrix;

	//��Ʈ���� ������Ʈ
	void UpdateMatrix();

public:
	typedef Component super;
	Vector2 pos;
	Vector2 bottomPos;
	Vector2 topPos;
	Vector2 scale;
	Transform();

	virtual void Init() override;

	virtual void Update() override;

	//Vector2 Ÿ�� ��ǥ ��������
	Vector2 GetPos();
	Vector2 GetBottomPos();
	Vector2 GetTopPos();
	Vector2 GetPosToPivot(TransFormPIVOT pivot);

	//������ǥ ��������
	Vector2 GetWorldPosition();

	//Vector2 Ÿ�� ������ ��������
	Vector2 GetScale();


	//��׸� ������ ȸ���� ��������
	inline float GetRotateDegree() { return _rotate * RadToDeg; }

	//���� ������ ȸ���� ��������
	inline float GetRotateRadian() { return _rotate; }

	//Vector2 Ÿ������ ��ǥ �����ϱ�
	void SetPos(Vector2 pos);

	//float Ÿ������ ��ǥ �����ϱ�
	void SetPos(float x, float y);

	//������ǥ ����
	void SetWorldPos(Vector2 pos);

	//Vector2 Ÿ������ ������ �����ϱ�
	inline void SetScale(Vector2 scale) { this->scale = scale; }

	//float Ÿ������ ������ �����ϱ�
	inline void SetScale(float w, float h) { scale = Vector2(w, h); }

	//���Ȱ����� ȸ���� ����
	inline void SetRotateToRadian(float radian) { _rotate = radian; }

	//��׸� ������ ȸ���� ����
	inline void SetRotateToDegree(float degree) { _rotate = degree * DegToRad; }

	RECT GetRect();

	inline void SetRect(RECT rc) { _rc = rc; }

	//3x3 ��Ʈ������ ���� �� ���� ��Ʈ���� ��������
	Matrix3x3 GetLocalToWorldMatrix() { return localToWorldMatrix; };

	//D2D 3x2 ��Ʈ������ ���� �� ���� ��Ʈ���� ��������
	D2D_MATRIX_3X2_F GetLocalToWorldMatrixTo_D2D_MATRIX() { return localToWorldMatrix.To_D2D1_Matrix_3x2_F(); };

	void SetRect();
};

