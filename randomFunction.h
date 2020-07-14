#pragma once
#include <time.h>

class randomFunction
{
private:
	randomFunction()
	{
		srand(GetTickCount());
	}

public:
	static randomFunction* GetInstace()
	{
		static randomFunction* instance = new randomFunction();
		return instance;
	}

public:
	~randomFunction() { }

	//===============  ������ ���� =======================

	//          0 ~ �Ű������� ���� ���� - 1 ��ŭ ���� ����
	inline int getInt(int num) { return rand() % num; }

	//        ������ ���� ���� �������� ������ ���� �̴� �Լ�
	inline int getFromIntTo(int fromNum, int toNum)
	{
		return rand() % (toNum - fromNum) + fromNum;
	}

	//================== �Ǽ��� ���� ============================
	float getFloat() { return (float)rand() / (float)RAND_MAX; }
	float getFloat(float num) { return ((float)rand() / (float)RAND_MAX) * num; }

	inline float getFromFloatTo(float fromNum, float toNum)
	{
		float rnd = (float)rand() / (float)RAND_MAX;

		return (rnd * (toNum - fromNum) + fromNum);
	}
};

