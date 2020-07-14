#include "stdafx.h"
#include "Maptool.h"
#include "Tile.h"
#include <filesystem>
#include "PaletteBtn.h"
#include "UndergroundScene.h"
#include "StartScene.h"

//using namespace filesystem;

//void setWindowsSize(int x, int y, int width, int height);

void Maptool::Init()
{
	Scene::Init();

	//setWindowsSize(WINSTARTX, WINSTARTY, 720, 500);

	GRAPHICMANAGER->AddImage("sky", L"Resource/Map/sky.png");
	GRAPHICMANAGER->AddImage("ground", L"Resource/Map/ground.png");
	GRAPHICMANAGER->AddImage("txtBackground", L"Resource/UI/txtBackground.png");
	GRAPHICMANAGER->AddImage("cursor", L"Resource/UI/cursor.png");
	GRAPHICMANAGER->AddImage("blank", L"Resource/Blank.png");

	SCENEMANAGER->addScene("underground", new UndergroundScene);
	SCENEMANAGER->addScene("startScene", new StartScene);

	SOUNDMANAGER->addSound("ArpPianoProgression", "Resource/Sound/ArpPianoProgression.mp3", true, true);

	ClassificationAttribute();

	_tileNumX = TILE_NUM_X;
	_tileNumY = TILE_NUM_Y;

	SetUp();

	_page = SamplePage::Terrain_1;
	SetPage();

	_eraser = EraserType::Terrain;
	_drawType = DrawType::Single;

	_rcLoad = RectMakeCenter(WINSIZEX - 100, WINSIZEY - 100, 130, 34);
	_rcSave = RectMakeCenter(WINSIZEX - 100, WINSIZEY - 150, 130, 34);
	_rcEraserType = RectMakeCenter(WINSIZEX - 100, WINSIZEY - 200, 130, 34);
	_rcClear = RectMakeCenter(WINSIZEX - 100, WINSIZEY - 250, 130, 34);

	_isShowRect = false;


	_cursorImg = GRAPHICMANAGER->FindImage("cursor");
	CAMERA->SetPos(Vector2(0, 0));

	SOUNDMANAGER->play("ArpPianoProgression");
}

void Maptool::Release()
{
	CAMERA->SetScaleValue(Vector2(1, 1));

	_vSetTer_1.clear();
	_vSetLadder.clear();
	_vSetDoor.clear();
	_vSetObj.clear();
	_vSetPos.clear();

	//SAFE_OBJECT_RELEASE(_cursorImg);
	//SAFE_DELETE(_cursorImg);

	SOUNDMANAGER->stop("ArpPianoProgression");

	Scene::Release();
}

void Maptool::Update()
{
	CAMERA->Control();

	//if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
	//{
	//	//for (int i = _tileNumX - 1; i < _tileNumY; i += _tileNumX)
	//	//{
	//	//	_vTiles[i]->SetIsActive(false);
	//	//}
	//
	//	for (int i = 0; i < _tileNumY; i++)
	//	{
	//		_vTiles[(_tileNumX-1) + i * _tileNumX]->SetIsActive(false);
	//	}
	//
	//	for (int i = _vTiles.size()-1; i >=0; i--)
	//	{
	//		if (_vTiles[i]->GetIsActive()) continue;
	//
	//		_vDeActiveTiles.push_back(_vTiles[i]);
	//
	//		_vTiles.erase(_vTiles.begin() + i);
	//	}
	//
	//	_tileNumX -= 1;
	//	//for (int i = 0; i < _tileNumY; ++i)
	//	//{
	//	//	for (int j = 0; j < _tileNumX; ++j)
	//	//	{
	//	//		_vTiles[i]->SetIdX(j);
	//	//		_vTiles[i]->SetIdY(i);
	//	//		_vTiles[i]->SetIndex(j + _tileNumX * i);
	//	//	}
	//	//}
	//}

	Scene::Update();

	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		if (PtInRect(&_rcLoad, _ptMouse.Vector2ToPOINT())) Load();
		if (PtInRect(&_rcSave, _ptMouse.Vector2ToPOINT())) Save();
		if (PtInRect(&_rcEraserType, _ptMouse.Vector2ToPOINT()))
			_eraser = (EraserType)(((int)_eraser + 1) % (int)EraserType::End);
		if (PtInRect(&_rcClear, _ptMouse.Vector2ToPOINT()))
		{
			if (MessageBox(_hWnd, "정말 전부 다 지울건가요?", "Clear Button", MB_YESNO) == IDYES)
			{
				for (Tile* t : _vTiles)
				{
					t->SetImgName("None");
					t->GetSprite()->SetImgName(t->GetImgName());
					t->GetSprite()->SetDepth(0);
					t->SetAttribute(TAttribute::NONE);

					t->GetTileObject()->SetImgName("None");
					t->GetTileObject()->GetSprite()->SetImgName("None");
					t->GetTileObject()->GetSprite()->SetPosition(t->GetTileObject()->GetTrans()->GetPos());
					t->GetTileObject()->SetAttribute(TAttribute::NONE);
					t->GetTileObject()->GetSprite()->SetDepth(0);
					t->SetTileParent(-1);
					t->ClearTileChildren();

					t->GetTileObject()->SetIsActive(false);
				}
			}
		}

		ClickSetTile();

		_startPos = { ((int)MOUSEPOINTER->GetMouseWorldPosition().x / TILE_WIDTH), ((int)MOUSEPOINTER->GetMouseWorldPosition().y / TILE_HEIGHT) };
	}

	if (KEYMANAGER->isStayKeyDown(VK_LBUTTON))
	{
		_endPos = { ((int)MOUSEPOINTER->GetMouseWorldPosition().x / TILE_WIDTH), ((int)MOUSEPOINTER->GetMouseWorldPosition().y / TILE_HEIGHT) };

		if (_drawType == DrawType::Single)
		{
			if((_vTiles[0]->GetTrans()->GetPos().y - TILE_HEIGHT / 2) <= MOUSEPOINTER->GetMouseWorldPosition().y &&
				(_vTiles[0]->GetTrans()->GetPos().x - TILE_WIDTH / 2) <= MOUSEPOINTER->GetMouseWorldPosition().x &&
				(_vTiles[_tileNumX - 1]->GetTrans()->GetPos().x + TILE_WIDTH / 2) >= MOUSEPOINTER->GetMouseWorldPosition().x &&
				(_vTiles[_tileNumX * _tileNumY - 1]->GetTrans()->GetPos().y + TILE_HEIGHT / 2) >= MOUSEPOINTER->GetMouseWorldPosition().y)
				SetMap();
		}
	}

	if (KEYMANAGER->isOnceKeyUp(VK_LBUTTON))
	{
		if (_ptMouse.x > WINSIZEX - 300) return;

		if ((_vTiles[0]->GetTrans()->GetPos().y - TILE_HEIGHT / 2) <= MOUSEPOINTER->GetMouseWorldPosition().y &&
			(_vTiles[0]->GetTrans()->GetPos().x - TILE_WIDTH / 2) <= MOUSEPOINTER->GetMouseWorldPosition().x &&
			(_vTiles[_tileNumX - 1]->GetTrans()->GetPos().x + TILE_WIDTH / 2) >= MOUSEPOINTER->GetMouseWorldPosition().x &&
			(_vTiles[_tileNumX * _tileNumY - 1]->GetTrans()->GetPos().y + TILE_HEIGHT / 2) >= MOUSEPOINTER->GetMouseWorldPosition().y)
		{
			if (_drawType == DrawType::Drag)
			{
				POINT max = { max(_startPos.x, _endPos.x), max(_startPos.y, _endPos.y) };
				POINT min = { min(_startPos.x, _endPos.x), min(_startPos.y, _endPos.y) };

				int start = (_startPos.x + _tileNumX * _startPos.y);
				int index;

				for (int i = 0; i <= max.y - min.y; ++i)
				{
					for (int j = 0; j <= max.x - min.x; ++j)
					{
						if (_startPos.x <= _endPos.x and _startPos.y <= _endPos.y) index = start + j + (_tileNumX * i);
						else if (_startPos.x <= _endPos.x and _startPos.y >= _endPos.y) index = start + j - (_tileNumX * i);
						else if (_startPos.x >= _endPos.x and _startPos.y <= _endPos.y) index = start - j + (_tileNumX * i);
						else if (_startPos.x >= _endPos.x and _startPos.y >= _endPos.y) index = start - j - (_tileNumX * i);

						_vTiles[index]->SetAttribute(_currentTile->GetAttribute());
						_vTiles[index]->SetImgName(_currentTile->GetImageKey());
						_vTiles[index]->GetSprite()->SetImgName(_currentTile->GetImageKey());

						if (_vTiles[index]->GetImgName().compare("Wall_None") == 0 || _vTiles[index]->GetImgName().compare("Wall_LB_2") == 0 ||
							_vTiles[index]->GetImgName().compare("Wall_RB_2") == 0 || _vTiles[index]->GetImgName().compare("Wall_RT_2") == 0 ||
							_vTiles[index]->GetImgName().compare("Wall_LT_2") == 0)
						{
							_vTiles[index]->GetSprite()->SetRectColor(ColorF::SkyBlue);
							_vTiles[index]->GetSprite()->SetFillRect(true);
						}
					}
				}
			}
			if (_currentTile->GetPaletteAttributeType() == PAT::Ladder)
			{
				POINT rageY = { max(_startPos.y, _endPos.y), min(_startPos.y, _endPos.y) };

				int start = (_startPos.x + _tileNumX * _startPos.y);
				int index;

				if ((rageY.x - rageY.y) < 3)
				{
					_vTiles[start]->AddTileChildren(_vTiles[start + _tileNumX]);
					_vTiles[start]->AddTileChildren(_vTiles[start + _tileNumX * 2]);

					for (int i = 0; i <= _vTiles[start]->GetTileChildren().size(); ++i)
					{
						index = start + (_tileNumX * i);

						_vTiles[index]->GetTileObject()->SetImgName(_currentTile->GetImageKey());
						_vTiles[index]->GetTileObject()->GetSprite()->SetIsFrame(true);
						_vTiles[index]->GetTileObject()->GetSprite()->SetImgName(_currentTile->GetImageKey());
						_vTiles[index]->GetTileObject()->GetSprite()->SetFrameY(1);
						_vTiles[index]->GetTileObject()->SetAttribute(_currentTile->GetAttribute());

						_vTiles[index]->GetTileObject()->GetSprite()->SetRectColor(ColorF::DarkCyan);
						_vTiles[index]->GetTileObject()->GetSprite()->SetFillRect(true);

						_vTiles[index]->GetTileObject()->SetIsActive(true);
					}
					_vTiles[start]->GetTileObject()->GetSprite()->SetFrameY(0);
					_vTiles[start + _tileNumX * 2]->GetTileObject()->GetSprite()->SetFrameY(2);

					return;
				}

				for (int i = 0; i <= rageY.x - rageY.y; ++i)
				{
					if (_startPos.y <= _endPos.y) index = start + (_tileNumX * i);
					else if (_startPos.y >= _endPos.y) index = start - (_tileNumX * i);

					_vTiles[index]->GetTileObject()->SetImgName(_currentTile->GetImageKey());
					_vTiles[index]->GetTileObject()->GetSprite()->SetIsFrame(true);
					_vTiles[index]->GetTileObject()->GetSprite()->SetImgName(_currentTile->GetImageKey());
					_vTiles[index]->GetTileObject()->GetSprite()->SetFrameY(1);
					_vTiles[index]->GetTileObject()->SetAttribute(_currentTile->GetAttribute());

					_vTiles[index]->GetTileObject()->GetSprite()->SetRectColor(ColorF::DarkCyan);
					_vTiles[index]->GetTileObject()->GetSprite()->SetFillRect(true);

					_vTiles[index]->GetTileObject()->SetIsActive(true);

					if (i == 0) continue;
					_vTiles[start]->AddTileChildren(_vTiles[index]);
				}

				if (_vTiles[start]->GetTileChildren().size())
				{
					int lastChildIndex = _vTiles[start]->GetTileChildren().size() - 1;
					if (_startPos.y <= _endPos.y)
					{
						_vTiles[start]->GetTileObject()->GetSprite()->SetFrameY(0);
						_vTiles[_vTiles[start]->GetTileChildren()[lastChildIndex]]->GetTileObject()->GetSprite()->SetFrameY(2);
					}
					else if (_startPos.y >= _endPos.y)
					{
						_vTiles[start]->GetTileObject()->GetSprite()->SetFrameY(2);
						_vTiles[_vTiles[start]->GetTileChildren()[lastChildIndex]]->GetTileObject()->GetSprite()->SetFrameY(0);
					}
				}
			}
		}
	}


	if (KEYMANAGER->isOnceKeyDown(VK_RBUTTON))
	{
		_startPos = { ((int)MOUSEPOINTER->GetMouseWorldPosition().x / TILE_WIDTH), ((int)MOUSEPOINTER->GetMouseWorldPosition().y / TILE_HEIGHT) };
	}

	if (KEYMANAGER->isStayKeyDown(VK_RBUTTON))
	{
		_endPos = { ((int)MOUSEPOINTER->GetMouseWorldPosition().x / TILE_WIDTH), ((int)MOUSEPOINTER->GetMouseWorldPosition().y / TILE_HEIGHT) };

		if (_drawType == DrawType::Single)
		{
			RemoveObject();
		}
	}

	if (KEYMANAGER->isOnceKeyUp(VK_RBUTTON))
	{
		if (_ptMouse.x > WINSIZEX - 300) return;

		if (_drawType == DrawType::Drag)
		{
			if ((_vTiles[0]->GetTrans()->GetPos().y - TILE_HEIGHT / 2) <= MOUSEPOINTER->GetMouseWorldPosition().y &&
				(_vTiles[0]->GetTrans()->GetPos().x - TILE_WIDTH / 2) <= MOUSEPOINTER->GetMouseWorldPosition().x &&
				(_vTiles[_tileNumX - 1]->GetTrans()->GetPos().x + TILE_WIDTH / 2) >= MOUSEPOINTER->GetMouseWorldPosition().x &&
				(_vTiles[_tileNumX * _tileNumY - 1]->GetTrans()->GetPos().y + TILE_HEIGHT / 2) >= MOUSEPOINTER->GetMouseWorldPosition().y)
			{
				POINT max = { max(_startPos.x, _endPos.x), max(_startPos.y, _endPos.y) };
				POINT min = { min(_startPos.x, _endPos.x), min(_startPos.y, _endPos.y) };

				int start = (_startPos.x + _tileNumX * _startPos.y);
				int index;

				for (int i = 0; i <= max.y - min.y; ++i)
				{
					for (int j = 0; j <= max.x - min.x; ++j)
					{
						if (_startPos.x <= _endPos.x and _startPos.y <= _endPos.y) index = start + j + (_tileNumX * i);
						else if (_startPos.x <= _endPos.x and _startPos.y >= _endPos.y) index = start + j - (_tileNumX * i);
						else if (_startPos.x >= _endPos.x and _startPos.y <= _endPos.y) index = start - j + (_tileNumX * i);
						else if (_startPos.x >= _endPos.x and _startPos.y >= _endPos.y) index = start - j - (_tileNumX * i);

						if (_eraser == EraserType::Terrain)
						{
							_vTiles[index]->SetImgName("None");
							_vTiles[index]->GetSprite()->SetImgName("None");
							_vTiles[index]->SetAttribute(TAttribute::NONE);
							_vTiles[index]->GetSprite()->SetRectColor(ColorF::Blue);
							_vTiles[index]->GetSprite()->SetFillRect(false);
						}
					}
				}
			}
		}
	}

	if (KEYMANAGER->isOnceKeyDown(VK_CONTROL)) _drawType = DrawType::Drag;
	else if (KEYMANAGER->isOnceKeyUp(VK_CONTROL)) _drawType = DrawType::Single;

	if (KEYMANAGER->isOnceKeyUp('1'))
	{
		_page = SamplePage::Terrain_1;
		SetPage();
	}
	else if (KEYMANAGER->isOnceKeyUp('2'))
	{
		_page = SamplePage::Ladder;
		SetPage();
	}
	else if (KEYMANAGER->isOnceKeyUp('3'))
	{
		_page = SamplePage::Door;
		SetPage();
	}
	else if (KEYMANAGER->isOnceKeyUp('4'))
	{
		_page = SamplePage::Object;
		SetPage();
	}
	else if (KEYMANAGER->isOnceKeyUp('5'))
	{
		_page = SamplePage::Position;
		SetPage();
	}

	if (KEYMANAGER->isOnceKeyUp('U'))
	{
		SCENEMANAGER->changeScene("underground");
		return;
	}
	if (KEYMANAGER->isOnceKeyUp('P'))
	{
		SCENEMANAGER->changeScene("startScene");
		return;
	}

	// tile draw rect
	if (KEYMANAGER->isOnceKeyUp(VK_F1))
	{
		_isShowRect = !_isShowRect;

		for (auto t : _vTiles)
		{
			t->GetSprite()->SetShowRect(_isShowRect);
			t->GetTileObject()->GetSprite()->SetShowRect(_isShowRect);
		}
	}

	if (MOUSEPOINTER->GetMouseWorldPosition().x < (CAMERA->GetPosition().x + WINSIZEX) - 300 &&
		(_vTiles[0]->GetTrans()->GetPos().y - TILE_HEIGHT / 2) < MOUSEPOINTER->GetMouseWorldPosition().y &&
		(_vTiles[0]->GetTrans()->GetPos().x - TILE_WIDTH / 2) < MOUSEPOINTER->GetMouseWorldPosition().x &&
		(_vTiles[_tileNumX - 1]->GetTrans()->GetPos().x + TILE_WIDTH / 2) > MOUSEPOINTER->GetMouseWorldPosition().x &&
		(_vTiles[_tileNumX * _tileNumY - 1]->GetTrans()->GetPos().y + TILE_HEIGHT / 2) > MOUSEPOINTER->GetMouseWorldPosition().y)
	{
		int index = ((int)MOUSEPOINTER->GetMouseWorldPosition().x / TILE_WIDTH) + _tileNumX * ((int)MOUSEPOINTER->GetMouseWorldPosition().y / TILE_HEIGHT);
		if (index >= 0 or index < _tileNumX * _tileNumY)
		{
			if (_currentTile->GetPaletteAttributeType() == PAT::Ladder)
			{
				_currentTile->GetTrans()->SetPos(_vTiles[index]->GetTrans()->GetPos());
				_currentTile->GetComponent<Sprite>()->SetPosition(_vTiles[index]->GetTrans()->GetPos() + Vector2(0, TILE_HEIGHT));
			}

			if (_currentTile->GetImageSize().y <= TILE_HEIGHT)
			{
				if (_currentTile->GetImageSize().x <= TILE_WIDTH)
					_currentTile->GetTrans()->SetPos(_vTiles[index]->GetTrans()->GetPos());
				else if (_currentTile->GetImageSize().x <= TILE_WIDTH * 2)
					_currentTile->GetTrans()->SetPos(_vTiles[index]->GetTrans()->GetPos() + Vector2(TILE_WIDTH / 2, 0));
			}
			else if (_currentTile->GetImageSize().y <= TILE_HEIGHT * 2)
			{
				_currentTile->GetTrans()->SetPos(_vTiles[index]->GetTrans()->GetPos());
				_currentTile->GetComponent<Sprite>()->SetPosition(_vTiles[index]->GetTrans()->GetPos() - Vector2(0, TILE_HEIGHT / 2));
			}
			else _currentTile->GetTrans()->SetPos(_vTiles[index]->GetTrans()->GetPos());
		}
		else _currentTile->GetTrans()->SetPos(MOUSEPOINTER->GetMouseWorldPosition());
	}
	else _currentTile->GetTrans()->SetPos(MOUSEPOINTER->GetMouseWorldPosition());
}


bool CompareToDepth2(Object* a, Object* b)
{
	Sprite* aS = a->GetComponent<Sprite>();
	Sprite* bS = b->GetComponent<Sprite>();

	if (!aS) return false;
	else if (!bS) return true;

	return aS->GetDepth() < bS->GetDepth();
}

void Maptool::Render()
{
	//GRAPHICMANAGER->DrawImage("map", Vector2(0, 0), Vector2(2, 2), 0.0f, false, 1.0f, PIVOT::LEFT_TOP);
	//GRAPHICMANAGER->DrawImage("sky", Vector2(0, 0), 1.0f, false, PIVOT::LEFT_TOP);
	//GRAPHICMANAGER->DrawImage("ground", Vector2(0, TILE_HEIGHT * 11), 1.0f, false, PIVOT::LEFT_TOP);

	sort(_activeList.begin(), _activeList.end(), CompareToDepth2);

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

	wchar_t buffer[128];
	ColorF textColor = ColorF::Black;

	switch (_page)
	{
	case SamplePage::Terrain_1:
		for (int i = 0; i < _vSetTer_1.size(); ++i)
		{
			textColor = ColorF::CornflowerBlue;
			swprintf(buffer, 128, L"%s", L"[ Terrain ]");
			GRAPHICMANAGER->DrawRect(Vector2(_vSetTer_1[i]->GetTrans()->GetPos().x, _vSetTer_1[i]->GetTrans()->GetPos().y), Vector2(SET_TILE_WIDTH, SET_TILE_HEIGHT), 0.0f, textColor, PIVOT::CENTER, 1.0f, false);
		}
		break;
	case SamplePage::Ladder:
		for (int i = 0; i < _vSetLadder.size(); ++i)
		{
			textColor = ColorF::CadetBlue;
			swprintf(buffer, 128, L"%s", L"[ Ladder ]");
			GRAPHICMANAGER->DrawRect(Vector2(_vSetLadder[i]->GetTrans()->GetPos().x, _vSetLadder[i]->GetTrans()->GetPos().y), Vector2(SET_TILE_WIDTH, SET_TILE_HEIGHT), 0.0f, textColor, PIVOT::CENTER, 1.0f, false);
		}
		break;
	case SamplePage::Door:
		for (int i = 0; i < _vSetDoor.size(); ++i)
		{
			textColor = ColorF::Firebrick;
			swprintf(buffer, 128, L"%s", L"[ Door ]");
			GRAPHICMANAGER->DrawRect(Vector2(_vSetDoor[i]->GetTrans()->GetPos().x, _vSetDoor[i]->GetTrans()->GetPos().y), Vector2(SET_TILE_WIDTH, SET_TILE_HEIGHT), 0.0f, textColor, PIVOT::CENTER, 1.0f, false);
		}
		break;
	case SamplePage::Object:
		for (int i = 0; i < _vSetObj.size(); ++i)
		{
			textColor = ColorF::DarkOrange;
			swprintf(buffer, 128, L"%s", L"[ Object ]");
			GRAPHICMANAGER->DrawRect(Vector2(_vSetObj[i]->GetTrans()->GetPos().x, _vSetObj[i]->GetTrans()->GetPos().y), Vector2(SET_TILE_WIDTH, SET_TILE_HEIGHT), 0.0f, textColor, PIVOT::CENTER, 1.0f, false);
		}
		break;
	case SamplePage::Position:
		for (int i = 0; i < _vSetPos.size(); ++i)
		{
			textColor = ColorF::Brown;
			swprintf(buffer, 128, L"%s", L"[ Position ]");
			GRAPHICMANAGER->DrawRect(Vector2(_vSetPos[i]->GetTrans()->GetPos().x, _vSetPos[i]->GetTrans()->GetPos().y), Vector2(SET_TILE_WIDTH, SET_TILE_HEIGHT), 0.0f, textColor, PIVOT::CENTER, 1.0f, false);
		}
		break;
	}
	GRAPHICMANAGER->Text(Vector2(WINSIZEX - 200, 10), buffer, 20, 100, 20, textColor, TextPivot::CENTER_TOP, L"맑은고딕", false);

	// draw load button
	GRAPHICMANAGER->DrawRect(Vector2(_rcLoad.left, _rcLoad.top), Vector2((_rcLoad.right - _rcLoad.left), (_rcLoad.bottom - _rcLoad.top)), 0.0f, ColorF::Blue, PIVOT::LEFT_TOP, 1.0f, false);
	GRAPHICMANAGER->Text(Vector2(_rcLoad.left, _rcLoad.top), L"Load Button", 14, _rcLoad.right - _rcLoad.left, _rcLoad.bottom - _rcLoad.top, ColorF::Black, TextPivot::CENTER);

	// draw save button
	GRAPHICMANAGER->DrawRect(Vector2(_rcSave.left, _rcSave.top), Vector2((_rcSave.right - _rcSave.left), (_rcSave.bottom - _rcSave.top)), 0.0f, ColorF::Red, PIVOT::LEFT_TOP, 1.0f, false);
	GRAPHICMANAGER->Text(Vector2(_rcSave.left, _rcSave.top), L"Save Button", 14, _rcSave.right - _rcSave.left, _rcSave.bottom - _rcSave.top, ColorF::Black, TextPivot::CENTER);

	// draw eraser button
	GRAPHICMANAGER->DrawRect(Vector2(_rcEraserType.left, _rcEraserType.top), Vector2((_rcEraserType.right - _rcEraserType.left), (_rcEraserType.bottom - _rcEraserType.top)), 0.0f, ColorF::Green, PIVOT::LEFT_TOP, 1.0f, false);
	if (_eraser == EraserType::Terrain) GRAPHICMANAGER->DrawTextD2D(Vector2(_rcEraserType.left + 8, _rcEraserType.top - 4), L"eraser type\n : Terrain", 14);
	else if (_eraser == EraserType::Object) GRAPHICMANAGER->DrawTextD2D(Vector2(_rcEraserType.left + 8, _rcEraserType.top - 4), L"eraser type\n : Object", 14);
	else if (_eraser == EraserType::Position) GRAPHICMANAGER->DrawTextD2D(Vector2(_rcEraserType.left + 8, _rcEraserType.top - 4), L"eraser type\n : Position", 14);

	// draw clear button
	GRAPHICMANAGER->DrawRect(Vector2(_rcClear.left, _rcClear.top), Vector2((_rcClear.right - _rcClear.left), (_rcClear.bottom - _rcClear.top)), 0.0f, ColorF::DarkGoldenrod, PIVOT::LEFT_TOP, 1.0f, false);
	GRAPHICMANAGER->Text(Vector2(_rcClear.left, _rcClear.top), L"Clear Button", 14, _rcClear.right - _rcClear.left, _rcClear.bottom - _rcClear.top, ColorF::Black, TextPivot::CENTER);
	
	//swprintf(buffer, 128, L"x : %f, y : %f",CAMERA->GetPosition().x, CAMERA->GetPosition().y);
	//GRAPHICMANAGER->Text((WINSIZE / 2) + Vector2(0, 100), buffer, 14, 200, 20, ColorF::AntiqueWhite, TextPivot::CENTER);

	//swprintf(buffer, 128, L"%d", index);
	//GRAPHICMANAGER->Text(Vector2(100, 100), buffer, 20, 200, 20, ColorF::White);

	_cursorImg->Render(MOUSEPOINTER->GetMouseWorldPosition().x, MOUSEPOINTER->GetMouseWorldPosition().y);
}

void Maptool::Save()
{
	//const char* fileName = "ground.map";
	const char* fileName = "test1.map";

	ofstream outFile;
	//outFile.open("test.map",ios::binary);
	outFile.open(fileName);

	for (Tile* t : _vTiles)
	{
		outFile << (int)t->GetAttribute() << endl;
		outFile << t->GetImgName() << endl;
		outFile << t->GetSprite()->GetPosition().x << endl;
		outFile << t->GetSprite()->GetPosition().y << endl;
		outFile << t->GetSprite()->GetDepth() << endl;
		outFile << t->GetIndex() << endl;

		outFile << (int)t->GetTileObject()->GetAttribute() << endl;
		outFile << t->GetTileObject()->GetImgName() << endl;
		outFile << t->GetTileObject()->GetSprite()->GetPosition().x << endl;
		outFile << t->GetTileObject()->GetSprite()->GetPosition().y << endl;
		outFile << t->GetTileObject()->GetSprite()->GetCurrentFrameX() << endl;
		outFile << t->GetTileObject()->GetSprite()->GetCurrentFrameY() << endl;
		outFile << t->GetTileObject()->GetSprite()->GetDepth() << endl;
		outFile << t->GetTileObject()->GetIndex() << endl;
		outFile << t->GetTileObject()->GetIsActive() << endl;

		outFile << t->GetTileObject()->GetSprite()->GetRectColor().r << endl;
		outFile << t->GetTileObject()->GetSprite()->GetRectColor().g << endl;
		outFile << t->GetTileObject()->GetSprite()->GetRectColor().b << endl;
		outFile << t->GetTileObject()->GetSprite()->GetRectColor().a << endl;

		outFile << t->GetTileObject()->GetSprite()->GetFillRect() << endl;

		outFile << t->GetTileParent() << endl;
		outFile << t->GetTileChildren().size() << endl;

		for (int i = 0; i < t->GetTileChildren().size(); ++i)
		{
			outFile << t->GetTileChildren()[i] << endl;
		}
	}

	outFile.close();

	MessageBox(_hWnd, "File Save!", fileName, MB_OK);
}

void Maptool::Load()
{
	//const char* fileName = "ground.map";
	const char* fileName = "test1.map";

	ifstream inFile(fileName);
	Vector2 pos;
	for (Tile* t : _vTiles)
	{
		// tile initialization
		t->SetImgName("None");
		t->GetSprite()->SetImgName(t->GetImgName());
		t->GetSprite()->SetDepth(0);
		t->SetAttribute(TAttribute::NONE);

		t->GetTileObject()->SetImgName("None");
		t->GetTileObject()->GetSprite()->SetImgName("None");
		t->GetTileObject()->GetSprite()->SetPosition(t->GetTileObject()->GetTrans()->GetPos());
		t->GetTileObject()->SetAttribute(TAttribute::NONE);
		t->GetTileObject()->GetSprite()->SetDepth(0);
		t->SetTileParent(-1);
		t->ClearTileChildren();

		t->GetTileObject()->SetIsActive(false);


		// tile setting
		char buffer[256];
		inFile.getline(buffer, 256);
		t->SetAttribute((TAttribute)atoi(buffer));

		inFile.getline(buffer, 256);
		t->SetImgName(buffer);
		t->GetSprite()->SetImgName(t->GetImgName());

		inFile.getline(buffer, 256);
		pos.x = atof(buffer);
		inFile.getline(buffer, 256);
		pos.y = atof(buffer);
		t->GetSprite()->SetPosition(pos);

		inFile.getline(buffer, 256);
		t->GetSprite()->SetDepth(atoi(buffer));

		inFile.getline(buffer, 256);
		t->SetIndex(atoi(buffer));

		// tile object setting
		inFile.getline(buffer, 256);
		t->GetTileObject()->SetAttribute((TAttribute)atoi(buffer));

		inFile.getline(buffer, 256);
		t->GetTileObject()->SetImgName(buffer);
		t->GetTileObject()->GetSprite()->SetImgName(t->GetTileObject()->GetImgName());

		inFile.getline(buffer, 256);
		pos.x = atof(buffer);

		inFile.getline(buffer, 256);
		pos.y = atof(buffer);

		t->GetTileObject()->GetSprite()->SetPosition(pos);

		inFile.getline(buffer, 256);
		t->GetTileObject()->GetSprite()->SetFrameX(atoi(buffer));

		inFile.getline(buffer, 256);
		t->GetTileObject()->GetSprite()->SetFrameY(atoi(buffer));

		inFile.getline(buffer, 256);
		t->GetTileObject()->GetSprite()->SetDepth(atoi(buffer));

		inFile.getline(buffer, 256);
		t->GetTileObject()->SetIndex(atoi(buffer));

		inFile.getline(buffer, 256);
		t->GetTileObject()->SetIsActive(atoi(buffer));

		ColorF color = ColorF::Blue;
		inFile.getline(buffer, 256);
		color.r = atof(buffer);
		inFile.getline(buffer, 256);
		color.g = atof(buffer);
		inFile.getline(buffer, 256);
		color.b = atof(buffer);
		inFile.getline(buffer, 256);
		color.a = atof(buffer);

		t->GetTileObject()->GetSprite()->SetRectColor(color);

		inFile.getline(buffer, 256);
		t->GetTileObject()->GetSprite()->SetFillRect(atoi(buffer));

		inFile.getline(buffer, 256);
		t->SetTileParent(atoi(buffer));

		inFile.getline(buffer, 256);
		int size = atoi(buffer);

		for (int i = 0; i < size; ++i)
		{
			inFile.getline(buffer, 256);
			t->AddTileChildren(_vTiles[atoi(buffer)]);
		}
	}
	inFile.close();

	MessageBox(_hWnd, "File load!", fileName, MB_OK);
}

void Maptool::SetUp()
{
	Object* paletteBg = Object::CreateObject<Object>();
	paletteBg->Init();
	paletteBg->SetName("Aquamarine");
	Sprite* ps = paletteBg->AddComponent<Sprite>();
	ps->Init();
	ps->SetFillRect(true);
	ps->SetRectColor(ColorF::Aquamarine);
	paletteBg->GetTrans()->SetPos(Vector2(WINSIZEX - 150, WINSIZEY / 2));
	paletteBg->GetTrans()->SetScale(Vector2(300, WINSIZEY));
	paletteBg->SetCameraAffect(false);
	ps->SetDepth(4);
	ps->SetShowRect(true);


	_currentTile = Object::CreateObject<PaletteBtn>();
	_currentTile->SetTag("CurrentTile");
	_currentTile->Init();
	_currentTile->GetSprite()->SetAlpha(0.7f);
	_currentTile->GetSprite()->SetDepth(6);

	for (int i = 0; i < _tileNumY; ++i)
	{
		for (int j = 0; j < _tileNumX; ++j)
		{
			Tile* tile = Object::CreateObject<Tile>();
			tile->Init();
			tile->GetTrans()->SetPos(j * TILE_WIDTH + (TILE_WIDTH / 2),
				i * TILE_HEIGHT + (TILE_HEIGHT / 2));
			tile->SetIdX(j);
			tile->SetIdY(i);
			tile->SetIndex(j + _tileNumX * i);
			tile->GetSprite()->SetDepth(0);
			tile->SetAttribute(TAttribute::NONE);
			tile->SetTileObject(Object::CreateObject<Tile>());
			tile->GetTileObject()->Init();
			tile->GetTileObject()->GetTrans()->SetPos(tile->GetTrans()->GetPos());
			tile->GetTileObject()->SetIdX(j);
			tile->GetTileObject()->SetIdY(i);
			tile->GetTileObject()->SetIndex(j + _tileNumX * i);
			tile->GetTileObject()->GetSprite()->SetImgName("None");
			tile->GetTileObject()->SetIsActive(false);

			_vTiles.push_back(tile);
		}
	}

	for (int i = 0; i < _vSetTer_1.size(); ++i)
	{
		_vSetTer_1[i]->GetTrans()->SetPos(Vector2((i % SET_TILE_NUM_X) * SET_TILE_WIDTH + WINSIZEX - 245,
			(i / SET_TILE_NUM_X) * SET_TILE_HEIGHT + 80));
		_vSetTer_1[i]->GetSprite()->SetPosition(_vSetTer_1[i]->GetTrans()->GetPos());
		_vSetTer_1[i]->GetSprite()->SetCameraEffected(false);
		_vSetTer_1[i]->SetIsActive(true);
		_vSetTer_1[i]->GetComponent<Text>()->SetPosition(_vSetTer_1[i]->GetTrans()->GetPos() + Vector2(-27.f, 10.f));
		_vSetTer_1[i]->GetTextBackground()->SetPosition(_vSetTer_1[i]->GetTrans()->GetPos() + Vector2(0.f, 18.8f));
	}
	for (int i = 0; i < _vSetLadder.size(); ++i)
	{
		_vSetLadder[i]->GetTrans()->SetPos(Vector2((i % SET_TILE_NUM_X) * SET_TILE_WIDTH + WINSIZEX - 245,
			(i / SET_TILE_NUM_X) * SET_TILE_HEIGHT + 80));
		_vSetLadder[i]->GetSprite()->SetPosition(_vSetLadder[i]->GetTrans()->GetPos());
		_vSetLadder[i]->GetSprite()->SetCameraEffected(false);
		_vSetLadder[i]->SetIsActive(false);
		_vSetLadder[i]->GetComponent<Text>()->SetPosition(_vSetLadder[i]->GetTrans()->GetPos() + Vector2(-27.f, 10.f));
		_vSetLadder[i]->GetTextBackground()->SetPosition(_vSetLadder[i]->GetTrans()->GetPos() + Vector2(0.f, 18.8f));
	}
	for (int i = 0; i < _vSetDoor.size(); ++i)
	{
		_vSetDoor[i]->GetTrans()->SetPos(Vector2((i % SET_TILE_NUM_X) * SET_TILE_WIDTH + WINSIZEX - 245,
			(i / SET_TILE_NUM_X) * SET_TILE_HEIGHT + 80));
		_vSetDoor[i]->GetSprite()->SetPosition(_vSetDoor[i]->GetTrans()->GetPos());
		_vSetDoor[i]->GetSprite()->SetCameraEffected(false);
		_vSetDoor[i]->SetIsActive(false);
		_vSetDoor[i]->GetComponent<Text>()->SetPosition(_vSetDoor[i]->GetTrans()->GetPos() + Vector2(-27.f, 10.f));
		_vSetDoor[i]->GetTextBackground()->SetPosition(_vSetDoor[i]->GetTrans()->GetPos() + Vector2(0.f, 18.8f));
	}
	for (int i = 0; i < _vSetObj.size(); ++i)
	{
		_vSetObj[i]->GetTrans()->SetPos(Vector2((i % SET_TILE_NUM_X) * SET_TILE_WIDTH + WINSIZEX - 245,
			(i / SET_TILE_NUM_X) * SET_TILE_HEIGHT + 80));
		_vSetObj[i]->GetSprite()->SetPosition(_vSetObj[i]->GetTrans()->GetPos());
		_vSetObj[i]->GetSprite()->SetCameraEffected(false);
		_vSetObj[i]->SetIsActive(false);
		_vSetObj[i]->GetComponent<Text>()->SetPosition(_vSetObj[i]->GetTrans()->GetPos() + Vector2(-27.f, 10.f));
		_vSetObj[i]->GetTextBackground()->SetPosition(_vSetObj[i]->GetTrans()->GetPos() + Vector2(0.f, 18.8f));
	}
	for (int i = 0; i < _vSetPos.size(); ++i)
	{
		_vSetPos[i]->GetTrans()->SetPos(Vector2((i % SET_TILE_NUM_X) * SET_TILE_WIDTH + WINSIZEX - 245,
			(i / SET_TILE_NUM_X) * SET_TILE_HEIGHT + 80));
		_vSetPos[i]->GetSprite()->SetPosition(_vSetPos[i]->GetTrans()->GetPos());
		_vSetPos[i]->GetSprite()->SetCameraEffected(false);
		_vSetPos[i]->SetIsActive(false);
		_vSetPos[i]->GetComponent<Text>()->SetPosition(_vSetPos[i]->GetTrans()->GetPos() + Vector2(-25.f, 10.f));
		_vSetPos[i]->GetTextBackground()->SetPosition(_vSetPos[i]->GetTrans()->GetPos() + Vector2(0.f, 18.8f));
	}
	int a = 10;
}

void Maptool::SetMap()
{
	if (_ptMouse.x > WINSIZEX - 300) return;

	int index = ((int)MOUSEPOINTER->GetMouseWorldPosition().x / TILE_WIDTH) + _tileNumX * ((int)MOUSEPOINTER->GetMouseWorldPosition().y / TILE_HEIGHT);
	if (index < 0 or index > _tileNumX* _tileNumY) return;

	SetAttribute(index, *_currentTile);
}

void Maptool::ClickSetTile()
{
	if ((int)_ptMouse.x < WINSIZEX - 278 || (int)_ptMouse.x >(WINSIZEX - 278) + (SET_TILE_WIDTH * SET_TILE_NUM_X) ||
		(int)_ptMouse.y < 50 || (int)_ptMouse.y > 50 + (SET_TILE_HEIGHT * SET_TILE_NUM_Y)) return;

	int index = (((int)_ptMouse.x - (WINSIZEX - 278)) / SET_TILE_WIDTH) + SET_TILE_NUM_X * (((int)_ptMouse.y - 50) / SET_TILE_HEIGHT);

	switch (_page)
	{
	case SamplePage::Terrain_1:
		if (index >= _vSetTer_1.size()) return;

		_currentTile->SetImageKey(_vSetTer_1[index]->GetImageKey());
		_currentTile->SetAttribute(_vSetTer_1[index]->GetAttribute());
		_currentTile->SetImageSize(_vSetTer_1[index]->GetImageSize());
		_currentTile->SetPaletteAttributeType(_vSetTer_1[index]->GetPaletteAttributeType());
		break;
	case SamplePage::Ladder:
		if (index >= _vSetLadder.size()) return;

		_currentTile->SetImageKey(_vSetLadder[index]->GetImageKey());
		_currentTile->SetAttribute(_vSetLadder[index]->GetAttribute());
		_currentTile->SetImageSize(_vSetLadder[index]->GetImageSize());
		_currentTile->SetPaletteAttributeType(_vSetLadder[index]->GetPaletteAttributeType());
		break;
	case SamplePage::Door:
		if (index >= _vSetDoor.size()) return;

		_currentTile->SetImageKey(_vSetDoor[index]->GetImageKey());
		_currentTile->SetAttribute(_vSetDoor[index]->GetAttribute());
		_currentTile->SetImageSize(_vSetDoor[index]->GetImageSize());
		_currentTile->SetPaletteAttributeType(_vSetDoor[index]->GetPaletteAttributeType());
		break;
	case SamplePage::Object:
		if (index >= _vSetObj.size()) return;

		_currentTile->SetImageKey(_vSetObj[index]->GetImageKey());
		_currentTile->SetAttribute(_vSetObj[index]->GetAttribute());
		_currentTile->SetImageSize(_vSetObj[index]->GetImageSize());
		_currentTile->SetPaletteAttributeType(_vSetObj[index]->GetPaletteAttributeType());
		break;
	case SamplePage::Position:
		if (index >= _vSetPos.size()) return;

		_currentTile->SetImageKey(_vSetPos[index]->GetImageKey());
		_currentTile->SetAttribute(_vSetPos[index]->GetAttribute());
		_currentTile->SetImageSize(_vSetPos[index]->GetImageSize());
		_currentTile->SetPaletteAttributeType(_vSetPos[index]->GetPaletteAttributeType());
		break;
	}

	_currentTile->GetSprite()->SetImgName(_currentTile->GetImageKey());
}

void Maptool::RemoveObject()
{
	if (_ptMouse.x > WINSIZEX - 300) return;

	int index = ((int)MOUSEPOINTER->GetMouseWorldPosition().x / TILE_WIDTH) + _tileNumX * ((int)MOUSEPOINTER->GetMouseWorldPosition().y / TILE_HEIGHT);

	if (index < 0 or index > (_tileNumX * _tileNumY)) return;

	switch (_eraser)
	{
	case EraserType::Terrain:
		_vTiles[index]->SetImgName("None");
		_vTiles[index]->GetSprite()->SetImgName("None");
		_vTiles[index]->GetSprite()->SetRectColor(ColorF::Blue);
		_vTiles[index]->GetSprite()->SetFillRect(false);
		_vTiles[index]->SetAttribute(TAttribute::NONE);
		break;
	case EraserType::Object: case EraserType::Position:
		if (!_vTiles[index]->GetTileObject()->GetIsActive()) return;
		
		Tile* p;
		int pIndex;

		if (_vTiles[index]->GetTileParent() > -1) pIndex = _vTiles[index]->GetTileParent();
		else pIndex = _vTiles[index]->GetIndex();
		
		p = _vTiles[pIndex];

		for (int i = 0; i < p->GetTileChildren().size(); ++i)
		{
			_vTiles[p->GetTileChildren()[i]]->GetTileObject()->SetImgName("None");
			_vTiles[p->GetTileChildren()[i]]->GetTileObject()->GetSprite()->SetImgName("None");
			_vTiles[p->GetTileChildren()[i]]->GetTileObject()->GetSprite()->SetDepth(0);
			_vTiles[p->GetTileChildren()[i]]->GetTileObject()->GetSprite()->SetRectColor(ColorF::Blue);
			_vTiles[p->GetTileChildren()[i]]->GetTileObject()->GetSprite()->SetFillRect(false);
			_vTiles[p->GetTileChildren()[i]]->GetTileObject()->SetAttribute(TAttribute::NONE);
			_vTiles[p->GetTileChildren()[i]]->GetTileObject()->SetTileParent(-1);
			_vTiles[p->GetTileChildren()[i]]->GetTileObject()->SetIsActive(false);
		}

		p->GetTileObject()->SetImgName("None");
		p->GetTileObject()->GetSprite()->SetImgName("None");
		p->GetTileObject()->GetSprite()->SetDepth(0);
		p->GetTileObject()->GetSprite()->SetRectColor(ColorF::Blue);
		p->GetTileObject()->GetSprite()->SetFillRect(false);
		p->GetTileObject()->GetSprite()->SetPosition(p->GetTileObject()->GetTrans()->GetPos());

		p->GetTileObject()->SetAttribute(TAttribute::NONE);
		p->ClearTileChildren();
		p->GetTileObject()->SetIsActive(false);

		break;
	}
}

void Maptool::SetAttribute(int curIdx, PaletteBtn& palett)
{
	if (palett.GetImageSize().y <= TILE_HEIGHT)
	{
		if (palett.GetImageSize().x <= TILE_WIDTH)
		{
			if (_currentTile->GetPaletteAttributeType() == PAT::Object)
			{
				if (_vTiles[curIdx]->GetTileObject()->GetIsActive()) return;

				SetTileParentObject(curIdx, _vTiles[curIdx]->GetTrans()->GetPos(), 1);
			}
			else if (_currentTile->GetPaletteAttributeType() == PAT::Terrain)
			{
				_vTiles[curIdx]->SetAttribute(palett.GetAttribute());
				_vTiles[curIdx]->SetImgName(_currentTile->GetImageKey());
				_vTiles[curIdx]->GetSprite()->SetImgName(_currentTile->GetImageKey());
				_vTiles[curIdx]->GetSprite()->SetPosition(_vTiles[curIdx]->GetTrans()->GetPos());

				if (_vTiles[curIdx]->GetAttribute() == TAttribute::WALL) _vTiles[curIdx]->GetTileObject()->GetSprite()->SetFillRect(true);
				if (_vTiles[curIdx]->GetImgName().compare("Wall_None") == 0 || _vTiles[curIdx]->GetImgName().compare("Wall_LB_2") == 0 ||
					_vTiles[curIdx]->GetImgName().compare("Wall_RB_2") == 0 || _vTiles[curIdx]->GetImgName().compare("Wall_RT_2") == 0 ||
					_vTiles[curIdx]->GetImgName().compare("Wall_LT_2") == 0)
				{
					_vTiles[curIdx]->GetSprite()->SetRectColor(ColorF::SkyBlue);
					_vTiles[curIdx]->GetSprite()->SetFillRect(true);
				}
			}
		}
		else if (palett.GetImageSize().x <= TILE_WIDTH * 2)
		{
			if (_currentTile->GetPaletteAttributeType() == PAT::Object)
			{
				if (curIdx - _tileNumX < 0) return;

				if (_vTiles[curIdx + 1]->GetTileObject()->GetIsActive() ||
					_vTiles[curIdx]->GetTileObject()->GetIsActive()) return;

				_vTiles[curIdx]->AddTileChildren(_vTiles[curIdx + 1]);

				int c = _vTiles[curIdx]->GetTileChildren()[0];
				_vTiles[c]->GetTileObject()->SetAttribute(_currentTile->GetAttribute());

				if (_vTiles[c]->GetTileObject()->GetAttribute() == TAttribute::INTERACTION)
				{
					_vTiles[c]->GetTileObject()->GetSprite()->SetRectColor(ColorF::YellowGreen);
					_vTiles[c]->GetTileObject()->GetSprite()->SetFillRect(true);
				}
				_vTiles[c]->GetTileObject()->SetIsActive(true);

				SetTileParentObject(curIdx, _vTiles[curIdx]->GetTrans()->GetPos() + Vector2(TILE_WIDTH / 2, 0), 1);
			}
			else if (_currentTile->GetPaletteAttributeType() == PAT::Terrain)
			{
				_vTiles[curIdx + 1]->SetAttribute(palett.GetAttribute());
				_vTiles[curIdx]->SetAttribute(palett.GetAttribute());

				_vTiles[curIdx]->SetImgName(_currentTile->GetImageKey());
				_vTiles[curIdx]->GetSprite()->SetImgName(_currentTile->GetImageKey());
				_vTiles[curIdx]->GetSprite()->SetPosition(_vTiles[curIdx]->GetTrans()->GetPos() + Vector2(TILE_WIDTH / 2, 0));
			}
		}
	}
	else if (palett.GetImageSize().y <= TILE_HEIGHT * 2)
	{
		if (_currentTile->GetPaletteAttributeType() == PAT::Object ||
			_currentTile->GetPaletteAttributeType() == PAT::Position)
		{
			if (curIdx - _tileNumX < 0) return;

			if (_vTiles[curIdx - _tileNumX]->GetTileObject()->GetIsActive() ||
				_vTiles[curIdx]->GetTileObject()->GetIsActive()) return;

			// 부모에겐 자식을 알려주고 자식에겐 부모를 알려주는 작업
			_vTiles[curIdx]->AddTileChildren(_vTiles[curIdx - _tileNumX]);

			int c = _vTiles[curIdx]->GetTileChildren()[0];
			_vTiles[c]->GetTileObject()->SetAttribute(_currentTile->GetAttribute());

			if (_vTiles[c]->GetTileObject()->GetAttribute() == TAttribute::INTERACTION)
				_vTiles[c]->GetTileObject()->GetSprite()->SetRectColor(ColorF::YellowGreen);
			
			else if (_vTiles[c]->GetTileObject()->GetAttribute() == TAttribute::DOOR)
				_vTiles[c]->GetTileObject()->GetSprite()->SetRectColor(ColorF::Lavender);

			else if (_vTiles[c]->GetTileObject()->GetAttribute() == TAttribute::NPC)
				_vTiles[c]->GetTileObject()->GetSprite()->SetRectColor(ColorF::Brown);

			else if (_vTiles[c]->GetTileObject()->GetAttribute() == TAttribute::ENEMY)
				_vTiles[c]->GetTileObject()->GetSprite()->SetRectColor(ColorF::DimGray);

			_vTiles[c]->GetTileObject()->GetSprite()->SetFillRect(true);
			_vTiles[c]->GetTileObject()->SetIsActive(true);

			SetTileParentObject(curIdx, _vTiles[curIdx]->GetTrans()->GetPos() - Vector2(0, TILE_HEIGHT / 2), 1);
		}
		else if (_currentTile->GetPaletteAttributeType() == PAT::Terrain)
		{
			_vTiles[curIdx - _tileNumX]->SetAttribute(palett.GetAttribute());
			_vTiles[curIdx]->SetAttribute(palett.GetAttribute());

			_vTiles[curIdx]->SetImgName(_currentTile->GetImageKey());
			_vTiles[curIdx]->GetSprite()->SetImgName(_currentTile->GetImageKey());
			_vTiles[curIdx]->GetSprite()->SetPosition(_vTiles[curIdx]->GetTrans()->GetPos() - Vector2(0, TILE_HEIGHT / 2));
		}
	}
	else if (palett.GetImageSize().y <= TILE_HEIGHT * 3)
	{
		if (_currentTile->GetPaletteAttributeType() == PAT::Object)
		{
			if (curIdx - _tileNumX < 0) return;

			if (_vTiles[curIdx - _tileNumX]->GetTileObject()->GetIsActive() ||
				_vTiles[curIdx + _tileNumX]->GetTileObject()->GetIsActive() ||
				_vTiles[curIdx]->GetTileObject()->GetIsActive()) return;

			_vTiles[curIdx]->AddTileChildren(_vTiles[curIdx - _tileNumX]);
			_vTiles[curIdx]->AddTileChildren(_vTiles[curIdx + _tileNumX]);

			int c;
			for (int i = 0; i < _vTiles[curIdx]->GetTileChildren().size(); ++i)
			{
				c = _vTiles[curIdx]->GetTileChildren()[i];
				//_vTiles[c]->GetTileObject()->SetImgName(_currentTile->GetImageKey());
				_vTiles[c]->GetTileObject()->SetAttribute(_currentTile->GetAttribute());

				if (_vTiles[c]->GetTileObject()->GetAttribute() == TAttribute::LADDER)
				{
					_vTiles[c]->GetTileObject()->GetSprite()->SetRectColor(ColorF::Cyan);
					_vTiles[c]->GetTileObject()->GetSprite()->SetFillRect(true);
				}
				_vTiles[c]->GetTileObject()->SetIsActive(true);
			}

			SetTileParentObject(curIdx, _vTiles[curIdx]->GetTrans()->GetPos(), 1);
		}
		else if (_currentTile->GetPaletteAttributeType() == PAT::Terrain)
		{
			_vTiles[curIdx - _tileNumX]->SetAttribute(palett.GetAttribute());
			_vTiles[curIdx]->SetAttribute(palett.GetAttribute());
			_vTiles[curIdx + _tileNumX]->SetAttribute(palett.GetAttribute());

			_vTiles[curIdx]->SetImgName(_currentTile->GetImageKey());
			_vTiles[curIdx]->GetSprite()->SetImgName(_currentTile->GetImageKey());
			_vTiles[curIdx]->GetSprite()->SetPosition(_vTiles[curIdx]->GetTrans()->GetPos());
		}
	}
}

void Maptool::SetPage()
{
	switch (_page)
	{
	case SamplePage::Terrain_1:
	{
		for (auto t2 : _vSetLadder)
			if(t2->GetIsActive()) t2->SetIsActive(false);

		for (auto obj : _vSetObj)
			if (obj->GetIsActive())	obj->SetIsActive(false);

		for (auto door : _vSetDoor)
			if (door->GetIsActive()) door->SetIsActive(false);
		for (auto pos : _vSetPos)
			if (pos->GetIsActive())	pos->SetIsActive(false);

		for (auto t1 : _vSetTer_1)
		{
			if (!t1->GetIsActive())
			{
				t1->SetIsActive(true);
				t1->SetAllowsRender(true);
			}
		}
	}
	break;
	case SamplePage::Ladder:
	{
		for (auto t1 : _vSetTer_1)
		{
			if (t1->GetIsActive())
			{
				t1->SetIsActive(false);
				t1->SetAllowsRender(false);
			}
		}
		for (auto obj : _vSetObj) 
			if (obj->GetIsActive())	obj->SetIsActive(false);
		for (auto door : _vSetDoor) 
			if (door->GetIsActive()) door->SetIsActive(false);
		for (auto pos : _vSetPos)
			if (pos->GetIsActive())	pos->SetIsActive(false);

		for (auto t2 : _vSetLadder) 
			if (!t2->GetIsActive())	t2->SetIsActive(true);
	}
	break;
	case SamplePage::Door:
	{
		for (auto t1 : _vSetTer_1)
		{
			if (t1->GetIsActive())
			{
				t1->SetIsActive(false);
				t1->SetAllowsRender(false);
			}
		}
		for (auto obj : _vSetObj) 
			if (obj->GetIsActive())	obj->SetIsActive(false);
		for (auto t2 : _vSetLadder) 
			if (t2->GetIsActive()) t2->SetIsActive(false);
		for (auto pos : _vSetPos)
			if (pos->GetIsActive())	pos->SetIsActive(false);

		for (auto door : _vSetDoor) 
			if (!door->GetIsActive())	door->SetIsActive(true);
	}
	break;
	case SamplePage::Object:
	{
		for (auto t1 : _vSetTer_1)
		{
			if (t1->GetIsActive())
			{
				t1->SetIsActive(false);
				t1->SetAllowsRender(false);
			}
		}
		for (auto t2 : _vSetLadder) 
			if (t2->GetIsActive()) t2->SetIsActive(false);
		for (auto door : _vSetDoor) 
			if (door->GetIsActive()) door->SetIsActive(false);
		for (auto pos : _vSetPos)
			if (pos->GetIsActive())	pos->SetIsActive(false);

		for (auto obj : _vSetObj) 
			if (!obj->GetIsActive())	obj->SetIsActive(true);
	}
	break;
	case SamplePage::Position:
	{
		for (auto t1 : _vSetTer_1)
		{
			if (t1->GetIsActive())
			{
				t1->SetIsActive(false);
				t1->SetAllowsRender(false);
			}
		}
		for (auto t2 : _vSetLadder)
			if (t2->GetIsActive()) t2->SetIsActive(false);
		for (auto door : _vSetDoor)
			if (door->GetIsActive()) door->SetIsActive(false);
		for (auto obj : _vSetObj)
			if (obj->GetIsActive())	obj->SetIsActive(false);

		for (auto pos : _vSetPos)
			if (!pos->GetIsActive())	pos->SetIsActive(true);
	}
	break;
	}

	int a = 0;
}

void Maptool::SetPaletteAndAddImage(string folderPath, PAT pat, TAttribute atr, vector<PaletteBtn*>& vec, Vector2 maxFrame, Vector2 scale)
{
	string imgKey;
	wstring path;
	wstring name;

	for (auto d : filesystem::directory_iterator(folderPath))
	{
		string a = d.path().string();
		path.assign(a.begin(), a.end());

		imgKey = d.path().string().substr(strlen(folderPath.c_str()), d.path().string().size() - (strlen(folderPath.c_str()) + 4));
		GRAPHICMANAGER->AddImage(imgKey, path, maxFrame.x, maxFrame.y);

		PaletteBtn* palette = Object::CreateObject<PaletteBtn>();
		palette->SetImageKey(imgKey);
		palette->Init();
		palette->SetPaletteAttributeType(pat);
		palette->SetAttribute(atr);
		palette->SetCameraAffect(false);
		palette->GetSprite()->SetScale(scale);
		palette->GetSprite()->SetDepth(5);
		palette->SetImageSize(POINT{ GRAPHICMANAGER->FindImage(imgKey)->GetFrameWidth(), GRAPHICMANAGER->FindImage(imgKey)->GetFrameHeight() });

		Sprite* s = palette->GetTextBackground();
		s->SetImgName("txtBackground");
		s->SetScale(Vector2(1.9f, 0.65f));
		s->SetAlpha(0.5f);
		s->SetDepth(6);

		name.assign(imgKey.begin(), imgKey.end());
		palette->AddComponent<Text>();
		palette->GetComponent<Text>()->CreateText(name, 10, 56, 20, ColorF::Black, L"Pixel Cyr");
		palette->GetComponent<Text>()->SetAnchor(AnchorPoint::CENTER);
		palette->GetComponent<Text>()->SetDepth(7);

		vec.push_back(palette);
	}
}

void Maptool::SetTileParentObject(int index, Vector2 imgPos, int imgDepth)
{
	Tile* pO = _vTiles[index]->GetTileObject();
	pO->SetImgName(_currentTile->GetImageKey());
	pO->SetAttribute(_currentTile->GetAttribute());
	pO->GetSprite()->SetImgName(_currentTile->GetImageKey());
	pO->GetSprite()->SetPosition(imgPos);
	pO->GetSprite()->SetDepth(imgDepth);

	if (pO->GetAttribute() == TAttribute::INTERACTION)
	{
		pO->GetSprite()->SetRectColor(ColorF::YellowGreen);
		pO->GetSprite()->SetFillRect(true);
	}

	else if (pO->GetAttribute() == TAttribute::LADDER)
	{
		pO->GetSprite()->SetRectColor(ColorF::Cyan);
		pO->GetSprite()->SetFillRect(true);
	}

	else if (pO->GetAttribute() == TAttribute::DOOR)
	{
		pO->GetSprite()->SetRectColor(ColorF::Lavender);
		pO->GetSprite()->SetFillRect(true);
	}

	else if (pO->GetAttribute() == TAttribute::NPC)
	{
		pO->GetSprite()->SetRectColor(ColorF::Brown);
		pO->GetSprite()->SetFillRect(true);
	}

	else if (pO->GetAttribute() == TAttribute::ENEMY)
	{
		pO->GetSprite()->SetRectColor(ColorF::DimGray);
		pO->GetSprite()->SetFillRect(true);
	}

	else if (pO->GetAttribute() == TAttribute::KEY)
	{
		pO->GetSprite()->SetRectColor(ColorF::Orange);
		pO->GetSprite()->SetFillRect(true);
	}
	else if (pO->GetAttribute() == TAttribute::BATTERY)
	{
		pO->GetSprite()->SetRectColor(ColorF::DarkSlateBlue);
		pO->GetSprite()->SetFillRect(true);
	}

	pO->SetIsActive(true);
}

void Maptool::ClassificationAttribute()
{
	string imgKey;
	wstring path;

	SetPaletteAndAddImage("Resource/Terrain/None/", PAT::Terrain, TAttribute::NONE, _vSetTer_1);
	SetPaletteAndAddImage("Resource/Terrain/Wall/", PAT::Terrain, TAttribute::WALL, _vSetTer_1);
	SetPaletteAndAddImage("Resource/Object/None/", PAT::Object, TAttribute::NONE, _vSetObj);
	SetPaletteAndAddImage("Resource/Object/Door/", PAT::Object, TAttribute::DOOR, _vSetDoor);
	SetPaletteAndAddImage("Resource/Object/Interaction/", PAT::Object, TAttribute::INTERACTION, _vSetObj);
	SetPaletteAndAddImage("Resource/Object/Dead_Body/", PAT::Object, TAttribute::DEAD_BODY, _vSetObj);
	SetPaletteAndAddImage("Resource/Object/Key/", PAT::Object, TAttribute::KEY, _vSetObj);
	SetPaletteAndAddImage("Resource/Object/Battery/", PAT::Object, TAttribute::BATTERY, _vSetObj);
	SetPaletteAndAddImage("Resource/NPC/Maptool/", PAT::Position, TAttribute::NPC, _vSetPos);
	SetPaletteAndAddImage("Resource/Enemy/Maptool/", PAT::Position, TAttribute::ENEMY, _vSetPos);
	SetPaletteAndAddImage("Resource/Object/Ladder/", PAT::Ladder, TAttribute::LADDER, _vSetLadder, Vector2(1, 3), Vector2(0.6f, 0.6f));

	GRAPHICMANAGER->AddImage("map", L"Resource/Map/Base.png");
}