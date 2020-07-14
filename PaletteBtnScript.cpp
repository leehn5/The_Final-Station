#include "stdafx.h"
#include "PaletteBtnScript.h"
#include "PaletteBtn.h"

void PaletteBtnScript::CollisionBegin(void* obj)
{
}

void PaletteBtnScript::CollisionPreSolve(void* obj)
{
	Object* to = (Object*)obj;
	PaletteBtn* me = (PaletteBtn*)_object;

	cout << to->GetName() << endl;

	if (to->GetTag() == "CurrentTile")
	{
		if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			to->SetName(me->GetName());
			to->GetComponent<Sprite>()->SetImgName(to->GetName());

			cout << "Collision" << endl;
		}
	}
}

void PaletteBtnScript::CollisionEnd(void* obj)
{
}
