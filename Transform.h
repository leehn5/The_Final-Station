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

	//매트릭스 업데이트
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

	//Vector2 타입 좌표 가져오기
	Vector2 GetPos();
	Vector2 GetBottomPos();
	Vector2 GetTopPos();
	Vector2 GetPosToPivot(TransFormPIVOT pivot);

	//월드좌표 가져오기
	Vector2 GetWorldPosition();

	//Vector2 타입 사이즈 가져오기
	Vector2 GetScale();


	//디그리 값으로 회전값 가져오기
	inline float GetRotateDegree() { return _rotate * RadToDeg; }

	//라디안 값으로 회전값 가져오기
	inline float GetRotateRadian() { return _rotate; }

	//Vector2 타입으로 좌표 설정하기
	void SetPos(Vector2 pos);

	//float 타입으로 좌표 설정하기
	void SetPos(float x, float y);

	//월드좌표 세팅
	void SetWorldPos(Vector2 pos);

	//Vector2 타입으로 사이즈 설정하기
	inline void SetScale(Vector2 scale) { this->scale = scale; }

	//float 타입으로 사이즈 설정하기
	inline void SetScale(float w, float h) { scale = Vector2(w, h); }

	//라디안값으로 회전값 세팅
	inline void SetRotateToRadian(float radian) { _rotate = radian; }

	//디그리 값으로 회전값 세팅
	inline void SetRotateToDegree(float degree) { _rotate = degree * DegToRad; }

	RECT GetRect();

	inline void SetRect(RECT rc) { _rc = rc; }

	//3x3 매트릭스로 로컬 투 월드 매트릭스 가져오기
	Matrix3x3 GetLocalToWorldMatrix() { return localToWorldMatrix; };

	//D2D 3x2 매트릭스로 로컬 투 월드 매트릭스 가져오기
	D2D_MATRIX_3X2_F GetLocalToWorldMatrixTo_D2D_MATRIX() { return localToWorldMatrix.To_D2D1_Matrix_3x2_F(); };

	void SetRect();
};

