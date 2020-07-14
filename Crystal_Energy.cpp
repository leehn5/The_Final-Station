#include "stdafx.h"
#include "Crystal_Energy.h"


Crystal_Energy::Crystal_Energy()
{
}


Crystal_Energy::~Crystal_Energy()
{
}

void Crystal_Energy::Init()
{
	super::Init();

	_name = "Crystal_Energy";

	_image = AddComponent<Sprite>();
	_image->SetImgName("\resource\img\Items\Items");
	_image->SetFrameX(2);
	_image->SetFrameY(2);
}
