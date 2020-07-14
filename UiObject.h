#pragma once
class UiObject : public Object
{
protected:
	UiObject();
	Sprite* _sprite;

public:

	template <typename T>
	static T* CreateUiObject(Object* parent = nullptr);

	Sprite* GetSprite() { return _sprite; }
	void SetSprite(string name, BOOL isFrame = FALSE);
};
#include"UiObject.hpp"
