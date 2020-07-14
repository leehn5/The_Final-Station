#include "stdafx.h"
#include "StartScene.h"
#include "Maptool.h"
#include "TrainScene.h"
#include "UndergroundScene.h"
#include "GroundScene.h"

void StartScene::Init()
{
	Scene::Init();

	ShowCursor(false);

	SCENEMANAGER->addScene("maptool", new Maptool);
	SCENEMANAGER->addScene("train", new TrainScene);
	SCENEMANAGER->addScene("underground", new UndergroundScene);
	SCENEMANAGER->addScene("ground", new GroundScene);

	SOUNDMANAGER->addSound("main", "Resource/Sound/main.mp3", true, true);
	SOUNDMANAGER->addSound("Door Open 2", "Resource/Sound/Door Open 2.mp3", false, false);

	_logoImg = Image::CreateImage("logo", "Resource/UI/Logo dlc.png");
	_eagleImg = Image::CreateImage("eagle", "eagle.png");
	_cursorImg = Image::CreateImage("cursor", "Resource/UI/cursor.png");
	_mainBG = Image::CreateImage("Main_BG", "Resource/UI/Main_BG2.png");

	_txtBtn = Object::CreateObject<TextButton>();
	_txtBtn->Init();
	_txtBtn->SetCameraAffect(false);
	_txtBtn->Setting("START GAME", Vector2(WINSIZEX / 2 + 40, WINSIZEY / 2 + 175), 25, Vector2(120, 23), 
		ColorF(0x8a733d, 0.5f), ColorF(0x8a733d, 1.f), L"Munro Small", cbGround);

	_btnMaptool = Object::CreateObject<TextButton>();
	_btnMaptool->Init();
	_btnMaptool->SetCameraAffect(false);
	_btnMaptool->Setting("MAPTOOL", Vector2(WINSIZEX / 2 + 40, WINSIZEY / 2 + 210), 25, Vector2(120, 23),
		ColorF(0x8a733d, 0.5f), ColorF(0x8a733d, 1.f), L"Munro Small", cbMaptool);

	TextButton* btnQuit = Object::CreateObject<TextButton>();
	btnQuit->Init();
	btnQuit->SetCameraAffect(false);
	btnQuit->Setting("QUIT", Vector2(WINSIZEX / 2 + 40, WINSIZEY / 2 + 245), 25, Vector2(120, 23),
		ColorF(0x8a733d, 0.5f), ColorF(0x8a733d, 1.f), L"Munro Small", cbQuit);

	GRAPHICMANAGER->SetRenderTargetColor(ColorF(0x12162d));
	CAMERA->SetPos(Vector2(0, 0));

	SOUNDMANAGER->play("main");
}

void StartScene::Update()
{
	Scene::Update();

	if(KEYMANAGER->isOnceKeyDown('Y')) SCENEMANAGER->changeScene("maptool");
	if(KEYMANAGER->isOnceKeyDown('1')) SCENEMANAGER->changeScene("train");
	if(KEYMANAGER->isOnceKeyDown('2')) SCENEMANAGER->changeScene("ground");
	if(KEYMANAGER->isOnceKeyDown('3')) SCENEMANAGER->changeScene("underground");
}

void StartScene::Render()
{
	_mainBG->Render(Vector2(WINSIZEX / 2 + 50, WINSIZEY / 2 + 50), 1.0f, false, PIVOT::CENTER, false);
	
	Scene::Render();

	_logoImg->Render(Vector2(WINSIZEX / 2 + 40, WINSIZEY / 2), Vector2(0.7f, 0.7f), 0.0f, Vector2::zero, false, 1.f, PIVOT::CENTER, false);
	_eagleImg->Render(Vector2(WINSIZEX - 50, WINSIZEY - 50), Vector2(0.2f, 0.2f), 0.0f, Vector2::zero, false, 1.0f, PIVOT::CENTER, false);
	_cursorImg->Render(MOUSEPOINTER->GetMouseWorldPosition().x, MOUSEPOINTER->GetMouseWorldPosition().y);
}

void StartScene::Release()
{
	SAFE_OBJECT_RELEASE(_cursorImg);
	SAFE_OBJECT_RELEASE(_logoImg);
	SAFE_OBJECT_RELEASE(_eagleImg);

	GRAPHICMANAGER->SetRenderTargetColor(ColorF::Black);

	SOUNDMANAGER->stop("main");

	Scene::Release();
}

void StartScene::cbGround()
{
	SCENEMANAGER->changeScene("ground");
	return;
}

void StartScene::cbQuit()
{
	PostQuitMessage(0);
}

void StartScene::cbMaptool()
{
	SCENEMANAGER->changeScene("maptool");
	return;
}
