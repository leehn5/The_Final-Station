#pragma once

class Vector2
{
public:
	float x;
	float y;


	Vector2():x(0),y(0) {}
	//float������ ���� ����
	Vector2(float x, float y) : x(x), y(y) {}
	//int ������ ���� ����
	Vector2(int x, int y) :x(x), y(y) {}
	template<typename T, typename T2>
	Vector2(T x, T2 y) : x((float)x), y((float)y) {}
	Vector2(POINT pos):x(pos.x),y(pos.y){}
	Vector2(POINTFLOAT pos) :x(pos.x), y(pos.y) {}
	~Vector2() {};

	static Vector2 up;
	static Vector2 down;
	static Vector2 right;
	static Vector2 left;
	static Vector2 zero;
	static Vector2 one;

	static b2Vec2 b2Up;
	static b2Vec2 b2Down;
	static b2Vec2 b2Right;
	static b2Vec2 b2Left;
	static b2Vec2 b2Zero;
	static b2Vec2 b2One;

	static float Distance(Vector2 a, Vector2 b, bool check = true);
	static float Dot(Vector2 lhs, Vector2 rhs);
	static Vector2 Lerp(Vector2 a, Vector2 b, float i);
	static float GetAngle(Vector2 a, Vector2 b);
	static Vector2 RandPosition(Vector2 a, Vector2 b);

	float Magnitude();
	Vector2 Nomalized();
	float SqrMagnitude();
	Vector2 RotateToDegree(float angle);
	Vector2 RotateToRadian(float angle);

	Vector2 operator-(Vector2 to);
	bool operator<(const Vector2& to) const;
	bool operator>(const Vector2& to) const;
	bool operator<=(const Vector2& to) const;
	bool operator>=(const Vector2& to) const;
	float operator|(const Vector2& to) const;
	float operator^(const Vector2& to) const;

	Vector2 operator+(Vector2 to);
	Vector2 operator*(Vector2 to);
	Vector2 operator/(Vector2 to);
	Vector2 operator*(float to);
	Vector2 operator/(float to);
	Vector2 operator*=(Vector2 to);
	Vector2 operator/=(Vector2 to);
	Vector2 operator*=(float to);
	Vector2 operator/=(float to);
	void operator+=(Vector2 to);
	void operator-=(Vector2 to);
	bool operator==(Vector2 to)const;
	bool operator!=(Vector2 to) const;

	POINT Vector2ToPOINT() { return POINT{ (LONG)this->x,(LONG)this->y }; }
};

