#pragma once

enum class ItemType
{
	Food,
	Madkit,
	Ammo,
	Money,
	Key,
	Battery,
	None,
};

class Item
{
private:

public:
	string name;
	ItemType type;
	int count;
	int price;

public:
	Item() : name(""), type(ItemType::None), count(0), price(0) {}
	Item(string name, ItemType type, int count, int price)
		: name(name), type(type), count(count), price(price) {}
};

