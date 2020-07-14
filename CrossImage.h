#pragma once

class CrossImage
{
private:
	Graphic* _imgLeft;
	Graphic* _imgRight;
	Vector2 _posLeft;
	Vector2 _posStart;

	Vector2 _posRight;

	RECT _rcLeft;
	
	float _speed;

public:
	void Init(string imgKey, Vector2 left, Vector2 right, float speed);
	void Release();
	void Update();
	void Render();
};

