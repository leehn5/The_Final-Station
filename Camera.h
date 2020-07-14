#pragma once

#define ZOOM_MIN 0.01f
#define ZOOM_MAX 2.5f

class Camera
{
private:
	Vector2				_shakeUpPos;
	Vector2				_shakeDownPos;

	Vector2				_prevPos;
	Vector2				_lastPos;

	float				_shakingTime;
	float				_shakeCount;
	float				_amount;
	bool				_isShaking;

private:
	//D2D1_MATRIX_3X2_F	_matrix;
	Matrix3x3			_matrix;

	//Matrix3x2F			_scaleMatrix;
	Matrix3x3			_scaleMatrix;
	Matrix3x3			_rotationMatrix;
	Matrix3x3			_translationMatrix;
	//Matrix3x3			_originTransMatrix;
	//Matrix3x3			_inverseMatrix;
	Vector2				_scale;
	Vector2				_pos;
	Vector2				_origin;
	Vector2				_startPos;
	Vector2				_endPos;

	float				_angle;
	float				_speed;
	float				_lerpCount;
	float				_moveTime;
	float				_prevDistance;
	bool				_isMoving;
	bool				_isMove2;

private:
	Camera()
	{
		//_scaleMatrix = Matrix3x2F::Identity();
		_scaleMatrix = Matrix3x3::identity;
		_rotationMatrix = Matrix3x3::identity;
		_translationMatrix = Matrix3x3::identity;
		//_originTransMatrix = Matrix3x3::identity;
		//_inverseMatrix = Matrix3x3::identity;

		_scale = Vector2(1, 1);
		_pos = Vector2().zero;
		_origin = Vector2().zero;
		_prevPos = Vector2().zero;
		_lastPos = Vector2().zero;
		_angle = 0.0f;
		_speed = 300.f;
		_lerpCount = 0;

		_shakingTime = 0.0f;
		_shakeCount = 0.0f;
		_amount = 0.0f;


		_isMoving = false;
		_isShaking = false;
	};
	void ShakingCamera();

public:
	static Camera* GetInstance()
	{
		static Camera* instance = new Camera();
		return instance;
	}

public:
	~Camera() {};

	HRESULT Init();
	void Update();
	void Control();
	void UpdateMatrix();
	
	void ShakingSetting(float time, float amount);
	void SetScale(Vector2 scale);
	void SetScaleValue(Vector2 scale) { _scale = scale; }
	//void SetAngle(float angle) { _angle = angle; };
	void SetPosition(Vector2 pos, string key);
	void SetPositionX(Vector2 pos, string key);
	void SetPosition(Vector2 pos);
	void SetPos(Vector2 pos);
	void SlowFollow(Vector2 pos, float speed);
	void MoveTo(Vector2 endPos, float time, bool isCenter = true);

	Vector2 GetPosition();
	Vector2 GetScale() { return _scale; }
	bool GetIsMoving() { return _isMoving; }
	bool GetIsShaking() { return _isShaking; }

	//D2D1_MATRIX_3X2_F GetMatrix() { return _matrix; }
	D2D1_MATRIX_3X2_F GetMatrix() { return _matrix.To_D2D1_Matrix_3x2_F(); }
	Matrix3x3 GetMatrix3x3() { return _matrix; }

	Matrix3x3 GetTransMatrix() { return _translationMatrix; }

	//Matrix3x2F GetScaleMatrix() { return _scaleMatrix; }
	Matrix3x3 GetScaleMatrix() { return _scaleMatrix; }

	Vector2 GetWorldToCamera(Vector2 pos);
	Vector2 GetCameraToWorld(Vector2 pos);
};

