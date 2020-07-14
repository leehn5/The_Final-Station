#include "stdafx.h"
#include "UiObject.h"

UiObject::UiObject()
{
	_sprite = AddComponent<Sprite>();
	_trans = AddComponent<Transform>();
	_cameraAffect = false;
	
}

void UiObject::SetSprite(string name, BOOL isFrame)
{
	if (isFrame)
	{
		_sprite->Init(true, true);
		_sprite->SetImgName(name);
		_trans->SetScale(_sprite->GetGraphic()->GetFrameWidth(), _sprite->GetGraphic()->GetFrameHeight());
	}
	else
	{
		_sprite->Init();
		_sprite->SetImgName(name);
		_trans->SetScale(_sprite->GetGraphic()->GetFrameWidth(), _sprite->GetGraphic()->GetFrameHeight());
	}
}
