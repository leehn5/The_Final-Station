#pragma once
#include "timer.h"

class timeManager
{
private:
	timer* _timer;

	timeManager();

public:
	static timeManager* GetInstance()
	{
		static timeManager* instance = new timeManager();
		return instance;
	}

	~timeManager();

	HRESULT init();
	void release();
	void update(float lockFPS = 0.0f);
	void render(HDC hdc);

	//DeltaŸ��
	inline float getElapsedTime() const { return _timer->getElapsedTime(); }
	inline float GetFps() const { return _timer->getFrameRate(); }
	//WorldŸ��
	inline float getWorldTime() const { return _timer->getWorldTime(); }

};

