#include "stdafx.h"
#include "GroundScene.h"
#include "UndergroundScene.h"
#include <filesystem>
#include "Player.h"
#include "UI.h"
#include "PlayerState.h"
#include "Train.h"
#include "TrainScene.h"
#include "FadeInAndOut.h"

void GroundScene::Init()
{
	Scene::Init();

	_name = "ground";

	GRAPHICMANAGER->AddImage("sky", L"Resource/Map/sky.png");
	GRAPHICMANAGER->AddImage("ground", L"Resource/Map/ground2.png");
	GRAPHICMANAGER->AddImage("branch", L"Resource/Map/branch.png");
	GRAPHICMANAGER->AddImage("smog", L"Resource/Map/smog.png");
	GRAPHICMANAGER->AddImage("Branch_1", L"Resource/Map/Branch 1.png");
	GRAPHICMANAGER->AddImage("blockers_stuck", L"Resource/Train/blockers_stuck.png");

	SCENEMANAGER->addScene("underground", new UndergroundScene);
	SCENEMANAGER->addScene("train", new TrainScene);

	_player = Object::CreateObject<Player>();

	_player->Init();
	if (_player->GetPrevScene() == "underground") _player->GetTrans()->SetPos(TILE_WIDTH * 35 + 16, 490);
	else _player->GetTrans()->SetPos(300, 490);
	_player->GetPhysicsBody()->SetBodyPosition();
	_player->GetSprite()->SetDepth(7);

	AddImage();
	MapLoad();

	_ui = new UI;
	_ui->Init();

	_isShowRect = false;

	_cursorImg = new Graphic;
	_cursorImg->Init(Direct2D::GetInstance()->CreateBitmap(L"Resource/UI/cursor.png"), "cursor", L"Resource/UI/cursor.png");

	_train = Object::CreateObject<Train>();
	_train->GetTrans()->SetPos(100, TILE_HEIGHT * 13 + 16);
	_train->SetTrainMode(TrainMode::Show_Out);
	_train->Init();
	_train->GetSprite()->SetDepth(2);

	Object* blockersStuck = Object::CreateObject<Object>();
	blockersStuck->GetTrans()->SetPos(556, TILE_HEIGHT * 15 - 8);
	Sprite* s = blockersStuck->AddComponent<Sprite>();
	s->Init();
	s->SetImgName("blockers_stuck");
	s->SetDepth(3);

	_fade = new FadeInAndOut;
	_fade->FadeInStart();

	SCENEMANAGER->GetNowScene()->GetWorld()->SetGravity(b2Vec2(0.0f, 15.0f));
	CAMERA->SetPos(Vector2(0, 0));
}

void GroundScene::Release()
{
	SAFE_OBJECT_RELEASE(_ui);
	SAFE_OBJECT_RELEASE(_cursorImg);
	SAFE_OBJECT_RELEASE(_train);

	SAFE_DELETE(_ui);
	SAFE_DELETE(_cursorImg);
	SAFE_DELETE(_train);
	SAFE_DELETE(_fade);

	_player->SetCollisionType(ColType::None);

	Scene::Release();
}

void GroundScene::Update()
{
	_fade->Update();

	//CAMERA->Control();
	CAMERA->SetPositionX(Vector2( _player->GetTrans()->GetPos().x, WINSIZEY / 2), "sky");

	Scene::Update();

	_ui->Update();

	if (KEYMANAGER->isOnceKeyDown('1')) SCENEMANAGER->changeScene("train");
	if (KEYMANAGER->isOnceKeyDown('3')) SCENEMANAGER->changeScene("underground");

	if (KEYMANAGER->isOnceKeyUp(VK_F1))
	{
		_isShowRect = !_isShowRect;

		for (Tile* t : _vTiles)
		{
			t->GetSprite()->SetShowRect(_isShowRect);
			t->GetTileObject()->GetSprite()->SetShowRect(_isShowRect);
		}
	}

	// Scene change
	if (KEYMANAGER->isOnceKeyDown('E') && (_player->GetState()->GetState() == "Idle" || _player->GetState()->GetState() == "Move"))
	{
		if (_player->GetCollisionType() == ColType::Door) _fade->FadeOutStart();
	}

	if (_fade->GetIsFadeOutEnd())
	{
		SCENEMANAGER->changeScene("underground");
		return;
	}

	if (_alpha >= 0.0f)
	{
		_alpha -= 0.02f * TIMEMANAGER->getElapsedTime();
		_pos1 += 5.f * TIMEMANAGER->getElapsedTime();
	}
}

bool CompareToDepth3(Object* a, Object* b)
{
	Sprite* aS = a->GetComponent<Sprite>();
	Sprite* bS = b->GetComponent<Sprite>();

	if (!aS) return false;
	else if (!bS) return true;

	return aS->GetDepth() < bS->GetDepth();
}

void GroundScene::Render()
{
	GRAPHICMANAGER->DrawImage("sky", Vector2(0, 0), 1.0f, false, PIVOT::LEFT_TOP);
	GRAPHICMANAGER->DrawImage("ground", Vector2(-(_player->GetTrans()->GetPos().x / 25), TILE_HEIGHT * 11), 1.0f, false, PIVOT::LEFT_TOP);

	//Scene::Render();

	sort(_activeList.begin(), _activeList.end(), CompareToDepth3);

	for (Object* child : _activeList)
	{
		if (child->GetTag() == "Tile")
		{
			if (child->GetTrans()->GetPos().x + 100 < CAMERA->GetPosition().x || child->GetTrans()->GetPos().x - 100 > CAMERA->GetPosition().x + WINSIZE.x / CAMERA->GetScale().x ||
				child->GetTrans()->GetPos().y + 100 < CAMERA->GetPosition().y || child->GetTrans()->GetPos().y - 100 > CAMERA->GetPosition().y + WINSIZE.y / CAMERA->GetScale().x) child->SetAllowsRender(false);

			else child->SetAllowsRender(true);
		}

		child->Render();
	}

	GRAPHICMANAGER->DrawImage("branch", Vector2(-(_player->GetTrans()->GetPos().x / 5), TILE_HEIGHT * 15 + 5), Vector2(1.5f, 2.0f), 0.0f, Vector2::zero, false, 1.0f, PIVOT::LEFT_TOP);
	GRAPHICMANAGER->DrawImage("Branch_1", Vector2(1355 -(_player->GetTrans()->GetPos().x / 10), TILE_HEIGHT * 11), Vector2(1.5f, 2.0f), 0.0f, Vector2::zero, false, 1.0f, PIVOT::LEFT_TOP);
	GRAPHICMANAGER->DrawImage("smog", Vector2(_pos1, TILE_HEIGHT * 15), _alpha, false, PIVOT::LEFT_TOP);

	_ui->Render();

	_cursorImg->Render(MOUSEPOINTER->GetMouseWorldPosition().x, MOUSEPOINTER->GetMouseWorldPosition().y);

	_fade->Render();
}

void GroundScene::MapLoad()
{
	for (int i = 0; i < TILE_NUM_Y; ++i)
	{
		for (int j = 0; j < TILE_NUM_X; ++j)
		{
			Tile* tile = Object::CreateObject<Tile>();
			tile->Init();
			tile->GetTrans()->SetPos(j * TILE_WIDTH + (TILE_WIDTH / 2),
				i * TILE_HEIGHT + (TILE_HEIGHT / 2));
			tile->SetIdX(j);
			tile->SetIdY(i);
			tile->SetIndex(j + TILE_NUM_X * i);
			tile->GetSprite()->SetDepth(0);
			tile->SetAttribute(TAttribute::NONE);
			tile->SetTileObject(Object::CreateObject<Tile>());
			tile->GetTileObject()->Init();
			tile->GetTileObject()->GetTrans()->SetPos(tile->GetTrans()->GetPos());
			tile->GetTileObject()->GetSprite()->SetImgName("None");
			tile->GetTileObject()->SetIsActive(false);

			_vTiles.push_back(tile);
		}
	}

	ifstream inFile("ground.map");
	Vector2 pos;
	for (int i = 0; i < _vTiles.size(); ++i)
	{
		char buffer[512];
		inFile.getline(buffer, 512);
		_vTiles[i]->SetAttribute((TAttribute)atoi(buffer));

		inFile.getline(buffer, 512);
		_vTiles[i]->SetImgName(buffer);
		_vTiles[i]->GetSprite()->SetImgName(_vTiles[i]->GetImgName());

		inFile.getline(buffer, 512);
		pos.x = atof(buffer);
		inFile.getline(buffer, 512);
		pos.y = atof(buffer);
		_vTiles[i]->GetSprite()->SetPosition(pos);

		inFile.getline(buffer, 512);
		_vTiles[i]->GetSprite()->SetDepth(atoi(buffer));

		inFile.getline(buffer, 512);
		_vTiles[i]->SetIndex(atoi(buffer));

		// tile object setting
		inFile.getline(buffer, 512);
		_vTiles[i]->GetTileObject()->SetAttribute((TAttribute)atoi(buffer));

		inFile.getline(buffer, 512);
		_vTiles[i]->GetTileObject()->SetImgName(buffer);
		_vTiles[i]->GetTileObject()->GetSprite()->SetImgName(_vTiles[i]->GetTileObject()->GetImgName());

		inFile.getline(buffer, 512);
		pos.x = atof(buffer);

		inFile.getline(buffer, 512);
		pos.y = atof(buffer);

		_vTiles[i]->GetTileObject()->GetSprite()->SetPosition(pos);

		inFile.getline(buffer, 512);
		_vTiles[i]->GetTileObject()->GetSprite()->SetFrameX(atoi(buffer));

		inFile.getline(buffer, 512);
		_vTiles[i]->GetTileObject()->GetSprite()->SetFrameY(atoi(buffer));

		inFile.getline(buffer, 512);
		_vTiles[i]->GetTileObject()->GetSprite()->SetDepth(atoi(buffer));

		inFile.getline(buffer, 512);
		_vTiles[i]->GetTileObject()->SetIndex(atoi(buffer));

		inFile.getline(buffer, 512);
		_vTiles[i]->GetTileObject()->SetIsActive(atoi(buffer));

		ColorF color = ColorF::Blue;
		inFile.getline(buffer, 512);
		color.r = atof(buffer);
		inFile.getline(buffer, 512);
		color.g = atof(buffer);
		inFile.getline(buffer, 512);
		color.b = atof(buffer);
		inFile.getline(buffer, 512);
		color.a = atof(buffer);

		_vTiles[i]->GetTileObject()->GetSprite()->SetRectColor(color);

		inFile.getline(buffer, 512);
		_vTiles[i]->GetTileObject()->GetSprite()->SetFillRect(atoi(buffer));

		inFile.getline(buffer, 512);
		_vTiles[i]->SetTileParent(atoi(buffer));

		inFile.getline(buffer, 512);
		int size = atoi(buffer);

		for (int j = 0; j < size; ++j)
		{
			inFile.getline(buffer, 512);
			_vTiles[i]->AddTileChildren(_vTiles[atoi(buffer)]);
		}

		if (_vTiles[i]->GetAttribute() == TAttribute::WALL)
		{
			b2Filter b;
			b.categoryBits = CATEGORY_SCENERY;
			b.maskBits = MASK_SCENERY;
		
			if (_vTiles[i]->GetImgName().compare("Wall_RB_2") == 0)
			{
				_vTiles[i]->GetTrans()->SetPos(_vTiles[i]->GetTrans()->GetPos() + Vector2(14.5f, 14.5f));
				_vTiles[i]->GetTrans()->SetScale(_vTiles[i]->GetTrans()->GetScale() + Vector2(10.0f, 10.0f));
				_vTiles[i]->SetPhysics();
				_vTiles[i]->GetPhysics()->SetBodyPosition();
				_vTiles[i]->GetPhysics()->GetBody()->SetTransform(b2Vec2(_vTiles[i]->GetPhysics()->GetBody()->GetPosition().x, _vTiles[i]->GetPhysics()->GetBody()->GetPosition().y), PI / 4);
				_vTiles[i]->GetPhysics()->GetBody()->GetFixtureList()->SetFilterData(b);
			}
			else if (_vTiles[i]->GetImgName().compare("Wall_LB_2") == 0)
			{
				_vTiles[i]->GetTrans()->SetPos(_vTiles[i]->GetTrans()->GetPos() + Vector2(-14.5f, 14.5f));
				_vTiles[i]->GetTrans()->SetScale(_vTiles[i]->GetTrans()->GetScale() + Vector2(10.0f, 10.0f));
				_vTiles[i]->SetPhysics();
				_vTiles[i]->GetPhysics()->SetBodyPosition();
				_vTiles[i]->GetPhysics()->GetBody()->SetTransform(b2Vec2(_vTiles[i]->GetPhysics()->GetBody()->GetPosition().x, _vTiles[i]->GetPhysics()->GetBody()->GetPosition().y), PI / 4);
				_vTiles[i]->GetPhysics()->GetBody()->GetFixtureList()->SetFilterData(b);
			}
			else
			{
				_vTiles[i]->SetPhysics();
				_vTiles[i]->GetPhysics()->SetBodyPosition();
				_vTiles[i]->GetPhysics()->GetBody()->GetFixtureList()->SetFilterData(b);
			}
		}

		if (_vTiles[i]->GetTileObject()->GetAttribute() == TAttribute::DOOR)
		{
			if (_vTiles[i]->GetTileParent() == -1)
			{
				Vector2 size;
				Vector2 pos;
				if (_vTiles[i]->GetTileChildren().size() >= 1)
				{
					size.y = abs(_vTiles[i]->GetTrans()->GetPos().y - _vTiles[_vTiles[i]->GetTileChildren()[0]]->GetTrans()->GetPos().y);

					if (size.y > 0.0f)
					{
						size = Vector2(TILE_WIDTH, TILE_HEIGHT * 2);
						pos = _vTiles[i]->GetTrans()->GetPos() + Vector2(0.f, -TILE_HEIGHT / 2);
					}

					_vTiles[i]->GetTileObject()->GetTrans()->SetScale(size);
					_vTiles[i]->GetTileObject()->GetTrans()->SetPos(pos);
				}

				_vTiles[i]->GetTileObject()->SetPhysics();
				_vTiles[i]->GetTileObject()->GetPhysics()->SetBodyPosition();
				_vTiles[i]->GetTileObject()->GetPhysics()->SetSensor(true);
			}
		}
	}
	inFile.close();

	for (int i = 0; i < _vTiles.size(); )
	{
		if (_vTiles[i]->GetImgName().compare("None") == 0)
		{

			_vTiles[i]->SetIsRelese();
			_vTiles.erase(_vTiles.begin() + i);
		}
		else i++;
	}

	_wall1 = Object::CreateObject<Object>();
	_wall1->Init();
	_wall1->GetTrans()->SetScale(TILE_WIDTH, TILE_HEIGHT * 16);
	_wall1->GetTrans()->SetPos(_vTiles[42]->GetTrans()->GetPos().x + TILE_WIDTH, TILE_HEIGHT * 7 + _wall1->GetTrans()->GetScale().y / 2);
	Sprite * s = _wall1->AddComponent<Sprite>();
	s->Init();
	PhysicsBody* p = _wall1->AddComponent<PhysicsBody>();
	p->Init(BodyType::STATIC, 1.f);


	_wall2 = Object::CreateObject<Object>();
	_wall2->Init();
	_wall2->GetTrans()->SetScale(TILE_WIDTH, TILE_HEIGHT * 16);
	_wall2->GetTrans()->SetPos(_vTiles[0]->GetTrans()->GetPos().x - TILE_WIDTH, TILE_HEIGHT * 7 + _wall2->GetTrans()->GetScale().y / 2);
	s = _wall2->AddComponent<Sprite>();
	s->Init();
	p = _wall2->AddComponent<PhysicsBody>();
	p->Init(BodyType::STATIC, 1.f);
}

void GroundScene::AddImage()
{
	DirectorySearch("Resource/Terrain/None/");
	DirectorySearch("Resource/Terrain/Wall/");
	DirectorySearch("Resource/Object/None/");
	DirectorySearch("Resource/Object/Door/");
	DirectorySearch("Resource/Object/Battery/");
	DirectorySearch("Resource/White/");
}

void GroundScene::DirectorySearch(string folderPath, int maxFrameX, int maxFrameY)
{
	string imgKey;
	wstring path;

	for (auto d : filesystem::directory_iterator(folderPath))
	{
		string a = d.path().string();
		path.assign(a.begin(), a.end());

		imgKey = d.path().string().substr(strlen(folderPath.c_str()), d.path().string().size() - (strlen(folderPath.c_str()) + 4));
		GRAPHICMANAGER->AddImage(imgKey, path, maxFrameX, maxFrameY);
	}
}
