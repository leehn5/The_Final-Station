#pragma once
#include"UiObject.h"

template<typename T>
inline T* UiObject::CreateUiObject(Object* parent)
{
	UiObject* Obj = new T();

	if (parent == nullptr)
		Obj->SetParent(SCENEMANAGER->GetNowScene());
	else
		Obj->SetParent(parent);

	return (T*)Obj;
}
