#pragma once
#include "Component.h"

class Item;

class ItemManager : public Component
{
private:
	typedef map<int, Item*>				mItemList;
	typedef map<int, Item*>::iterator	mItemIter;

	map<int, vector<Item*>> _mItemList;

private:
	mItemList _mItem;
	int _rndStart;

public:
	virtual void Init();
	virtual void Release();

	void AddItem(int index);
	void RandomAddItem();

	map<int, vector<Item*>> GetItemList() { return _mItemList; }
};

