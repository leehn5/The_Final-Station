#pragma once
#include"Sprite.h"

template<typename T>
void ObjectPool::Init(int size, Object obj)
{
	ActivePool.reserve(size);
	for (int i = 0; i < size; i++)
	{
		Object* newObj = Object::CreateObject<T>();
		newObj->Init();
		newObj->SetTag(obj.GetTag());
		newObj->SetName(obj.GetName());
		newObj->SetIsActive(false);
		pool.push(newObj);

	}
}