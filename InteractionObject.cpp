#include "stdafx.h"
#include "InteractionObject.h"

void InteractionObject::Init()
{
	Object::Init();

	_sprite = AddComponent<Sprite>();
}

void InteractionObject::Update()
{
	Object::Update();
}

void InteractionObject::Interaction()
{
}
