#pragma once
#include "Object.h"

typedef enum class PaletteAttribute : byte
{
	Terrain,
	Object,
	Ladder,
	Position,
	None,
}PAT;

class PaletteBtn : public Object
{
private:
	//PhysicsBody* _physics;
	Sprite* _sprite;
	Sprite* _txtBackground;
	TAttribute _attribute;
	PAT _pAType;
	string _imgKey;

	POINT _imgSize;

public:
	PaletteBtn() : _imgKey("None") {}
	virtual void Init();
	virtual void Release();
	virtual void Update();
	virtual void Render();

	void SetAttribute(TAttribute atribute) { _attribute = atribute; }
	void SetImageSize(POINT size) { _imgSize = size; }
	void SetImageKey(string imgKey) { _imgKey = imgKey; }
	void SetPaletteAttributeType(PAT pat) { _pAType = pat; }

	TAttribute GetAttribute() { return _attribute; }
	POINT GetImageSize() { return _imgSize; }
	Sprite* GetSprite() { return _sprite; }
	Sprite* GetTextBackground() { return _txtBackground; }
	string GetImageKey() { return _imgKey; }
	PAT GetPaletteAttributeType() { return _pAType; }
	//PhysicsBody* GetPhysics() { return _physics; }
};

