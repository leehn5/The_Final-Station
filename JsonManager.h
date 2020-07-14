#pragma once
#include"singletonBase.h"
// for convenience
class JsonManager : public singletonBase<JsonManager>
{

public:

	void WriteJson(string jsonName, vector<string> strings, vector<int> integers);
};

