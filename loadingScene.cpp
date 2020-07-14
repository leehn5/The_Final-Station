#include "stdafx.h"
#include "loadingScene.h"

loadingScene::loadingScene()
	: _background(nullptr), _currentCount(0),count(0),frameX(0)
{
}


loadingScene::~loadingScene()
{
}

HRESULT loadingScene::init()
{
	_background = GRAPHICMANAGER->AddImage("load", L"resource/img/UI/load2.png", 68, 1);

	CreateThread(
		NULL,			//스레드의 보안속성(자식윈도우가 있을때) 핸들로 자식인지 부모인지 파악
		NULL,			//스레드의 스택크기(NULL로 두면 메인쓰레드와 스택크기는 동일하다)
		threadFunction,	//스레드 사용할 함수 명
		this,			//스레드의 매개변수(this로하면 본 클래스)
		NULL,			//스레드의 특성(NULL로 두면 바로 실행한다)
		0				//스레드의 생성 후 스레드의 ID를 넘겨준다 보통은 NULL로 둠.
	);

	return S_OK;
}

void loadingScene::release()
{
}

void loadingScene::update()
{

	if (_currentCount == LOADINGMAX)
	{
		SCENEMANAGER->changeScene("Town");
	}

	count += TIMEMANAGER->getElapsedTime();
	if (count >= 0.03f)
		//if (count >= 0.06f)
	{
		if (frameX > 68) frameX = 0;
		frameX++;
		count = 0;
	}


}

void loadingScene::render()
{
	GRAPHICMANAGER->DrawFrameImage("load", Vector2(WINSIZEX - 80, WINSIZEY - 50), frameX, 0);
}

DWORD CALLBACK threadFunction(LPVOID lpParameter)
{
	loadingScene* loadHealper = (loadingScene*)lpParameter;

	while (loadHealper->_currentCount != LOADINGMAX)
	{
		//여기에 이미지 및 사운드들 추가

		Sleep(1);

		loadHealper->_currentCount++;
	}
	return 0;
}
