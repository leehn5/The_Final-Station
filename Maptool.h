#pragma once
#include "Scene.h"
#include <commdlg.h> //OPENFILENAME을 위한 헤더
#include "PaletteBtn.h"

#define SET_TILE_WIDTH 64
#define SET_TILE_HEIGHT 64

#define SET_TILE_NUM_X 4
#define SET_TILE_NUM_Y 6

enum class SamplePage : byte
{
    Terrain_1,
    Ladder,
    Door,
    Object,
    Position,
    Page_end,
};

enum class EraserType : byte
{
    Terrain,
    Object,
    Position,
    End,
};

enum class DrawType : byte
{
    Single,
    Drag,
    End,
};

class Tile;

class Maptool : public Scene
{
private:
    PaletteBtn* _currentTile;
    vector<PaletteBtn*> _vSetTer_1;
    vector<PaletteBtn*> _vSetLadder;
    vector<PaletteBtn*> _vSetDoor;
    vector<PaletteBtn*> _vSetObj;
    vector<PaletteBtn*> _vSetPos;

    vector<Tile*> _vTiles;
    vector<Tile*> _vDeActiveTiles;

    int _tileNumX;
    int _tileNumY;

    Graphic* _cursorImg;

    Vector2 _startPos;
    Vector2 _endPos;

    SamplePage _page;
    EraserType _eraser;
    DrawType _drawType;

    RECT _rcLoad;
    RECT _rcSave;
    RECT _rcEraserType;
    RECT _rcClear;

    bool _isShowRect;

private:
    void ClassificationAttribute();
    void SetAttribute(int curIdx, PaletteBtn& palett);

public:
    virtual void Init();
    virtual void Release();
    virtual void Update();
    virtual void Render();

    void Save();
    void Load();

    void SetUp();
    void SetMap();
    void ClickSetTile();
    void RemoveObject();

    void SetPage();
    void SetPaletteAndAddImage(string folderPath, PAT pat, TAttribute atr, vector<PaletteBtn*>& vec, Vector2 maxFrame = Vector2(1, 1), Vector2 scale = Vector2(1, 1));
    void SetTileParentObject(int index, Vector2 imgPos, int imgDepth);
};
