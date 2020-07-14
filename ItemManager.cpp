#include "stdafx.h"
#include "ItemManager.h"
#include "Item.h"

void ItemManager::Init()
{
	_mItem.clear();
	_mItemList.clear();

	_mItem.insert(make_pair(0, new Item("GOLD KEY", ItemType::Key, 1, 1)));
	_mItem.insert(make_pair(1, new Item("BATTERY", ItemType::Battery, 1, 1)));
	_mItem.insert(make_pair(2, new Item("PISTOL AMMO", ItemType::Ammo, 1, 15)));
	_mItem.insert(make_pair(3, new Item("FOOD", ItemType::Food, 1, 10)));
	_mItem.insert(make_pair(4, new Item("SPOON", ItemType::Money, 1, 1)));
	_mItem.insert(make_pair(5, new Item("CLOTHING", ItemType::Money, 1, 1)));
	_mItem.insert(make_pair(6, new Item("BOTTLE OF BEER", ItemType::Money, 1, 1)));
	_mItem.insert(make_pair(7, new Item("MEDKIT", ItemType::Madkit, 1, 1)));

	_rndStart = 2;
}

void ItemManager::Release()
{
}

void ItemManager::AddItem(int index)
{
	vector<Item*> vItem;

	Item* item = _mItem[index];
	vItem.push_back(item);

	_mItemList.insert(make_pair(0, vItem));
}

void ItemManager::RandomAddItem()
{
	vector<Item*> vItem;

	int count = RND->getInt(3);

	if (count == 0)
	{
		int rnd = RND->getFromIntTo(_rndStart, _mItem.size());
		Item* item = _mItem[rnd];
		vItem.push_back(item);
	}
	else if (count == 1)
	{
		int rnd = RND->getFromIntTo(_rndStart, _mItem.size());
		int rnd1;
		Item* item = _mItem[rnd];
		vItem.push_back(item);

		while (true)
		{
			rnd1 = RND->getFromIntTo(_rndStart, _mItem.size());
			if (rnd == rnd1) continue;
			break;
		}

		item = _mItem[rnd1];
		vItem.push_back(item);
	}
	else if (count == 2)
	{
		int rnd = RND->getFromIntTo(_rndStart, _mItem.size());
		int rnd1, rnd2;
		Item* item = _mItem[rnd];
		vItem.push_back(item);

		while (true)
		{
			rnd1 = RND->getFromIntTo(_rndStart, _mItem.size());
			if (rnd == rnd1) continue;

			rnd2 = RND->getFromIntTo(_rndStart, _mItem.size());
			if (rnd2 == rnd || rnd2 == rnd1) continue;
			break;
		}

		item = _mItem[rnd1];
		vItem.push_back(item);

		item = _mItem[rnd2];
		vItem.push_back(item);
	}

	_mItemList.insert(make_pair(0, vItem));

	//for (int i = 0; i < _mItemList[0].size(); ++i)
	//{
	//	cout << _mItemList[0][i]->name << endl;
	//	cout << _mItemList[0][i]->count << endl << endl;
	//}
}
