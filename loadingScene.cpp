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
		NULL,			//�������� ���ȼӼ�(�ڽ������찡 ������) �ڵ�� �ڽ����� �θ����� �ľ�
		NULL,			//�������� ����ũ��(NULL�� �θ� ���ξ������ ����ũ��� �����ϴ�)
		threadFunction,	//������ ����� �Լ� ��
		this,			//�������� �Ű�����(this���ϸ� �� Ŭ����)
		NULL,			//�������� Ư��(NULL�� �θ� �ٷ� �����Ѵ�)
		0				//�������� ���� �� �������� ID�� �Ѱ��ش� ������ NULL�� ��.
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
		//���⿡ �̹��� �� ����� �߰�

		Sleep(1);

		loadHealper->_currentCount++;
	}
	return 0;
}
