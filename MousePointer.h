#pragma once

class MousePointer
{
private:
	Vector2 _pos;
	MousePointer() {}

public:
	static MousePointer* GetInstance();

	Vector2 GetMouseWorldPositionForDesignResolution();
	Vector2 GetMouseWorldPosition();
	Vector2 GetMouseLocalPositionForDesignResolution();
	Vector2 GetMouseLocalPosition();
};

