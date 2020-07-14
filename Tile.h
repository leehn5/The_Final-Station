#pragma once
#include"Object.h"

#define TILE_WIDTH 32
#define TILE_HEIGHT 32

#define TILE_NUM_X 100
#define TILE_NUM_Y 100

typedef enum class TileAttribute : byte
{
    START,
    END,
    WALL,
    INTERACTION,
    NPC_NONE_MOVE,
    DEAD_BODY,
    LADDER,
    DOOR,
    NONE,
    NPC,
    ENEMY,
    KEY,
    BATTERY,
}TAttribute;

class Tile : public Object
{
private:
    int _idX;
    int _idY;
    int _id;
    int _index;

    //F = G + H
    float _f;      //총 비용
    float _g;   //시작위치로부터 현재 노드
    float _h;      //현재 노드로부터 도착점까지 경로비용

    bool _isOpen;
    bool _isClose;
    bool _isFrame;

    Tile* _node;
    
    Tile* _tileObj;

    //Tile* _tileParent;
    //vector<Tile*> _vTileChild;

    int _tileParentIdx;
    vector<int> _vTileChildIdx;

    TAttribute _attribute;   //타일속성
    string _imgName;   //타일속성
    PIVOT _pivot;
    PhysicsBody* _physics;
    Sprite* _sprite;
    Sprite* _backSprite;

public:
    Tile() :_f(0), _g(0),
        _h(0), _idX(0), _idY(0) 
    {
        _sprite = AddComponent<Sprite>();
        _backSprite = AddComponent<Sprite>();
        //_tileParent = nullptr;
        _tileParentIdx = -1;

        _tileObj = nullptr;
    }

    ~Tile() {};
    //virtual void Init(int idX, int idY);
    virtual void Init();
    virtual void Update() { return; }
    
    virtual void Render();

    static Vector2 tileSize;

    void SetParentNode(Tile* p) { _node = p; }
    Tile* GetParentNode() { return _node; }

    int GetIdX() { return _idX; }
    void SetIdX(int idx) { _idX = idx; }
    int GetIdY() { return _idY; }
    void SetIdY(int idy) { _idY = idy; }

    int GetIndex() { return _index; }
    void SetIndex(int idx) { _index = idx; }

    void SetCenter(Vector2 center) { _trans->SetPos(center); }
    Vector2 GetCenter() { return _trans->GetPos(); }

    void SetAttribute(TAttribute str) { _attribute = str; }
    TAttribute GetAttribute() { return _attribute; }

    void SetCostF(float totalCost) { _f = totalCost; }
    float GetCostF() { return _f; }

    void SetCostG(float costFromStart) { _g = costFromStart; }
    float GetCostG() { return _g; }

    void SetCostH(float costToGoal) { _h = costToGoal; }
    float GetCostH() { return _h; }

    void SetIsOpen(bool isOpen) { _isOpen = isOpen; }
    bool GetIsOpen() { return _isOpen; }

    void SetIsClose(bool isClose) { _isClose = isClose; }
    bool GetIsClose() { return _isClose; }

    void SetPhysics();
    PhysicsBody* GetPhysics() { return _physics; }


    void SetId(int id) { _id = id; }
    int GetId() { return _id; }

    void SetImgName(string name);
    string GetImgName() { return _imgName; }

    void SetIsFrame(bool active) { _isFrame = active; }
    bool GetIsFrame() { return _isFrame; }

    void SetPivot(PIVOT pivot) { _pivot = pivot; }
    PIVOT GetPivot() { return _pivot; }

    Sprite* GetSprite() { return _sprite; }
    Sprite* GetBackSprite() { return _backSprite; }

    /*void SetTileParent(Tile* parent) { _tileParent = parent; }
    Tile* GetTileParent() { return _tileParent; }*/
    void SetTileParent(int parent) { _tileParentIdx = parent; }
    int GetTileParent() { return _tileParentIdx; }

    //vector<Tile*> GetTileChildren() { return _vTileChild; }
    //void AddTileChildren(Tile* child);
    vector<int> GetTileChildren() { return _vTileChildIdx; }
    void AddTileChildren(Tile* child);

    Tile* GetTileObject() { return _tileObj; }
    void SetTileObject(Tile* obj) { _tileObj = obj; }
    //void ClearTileChildren() { _vTileChild.clear(); }
    void ClearTileChildren() { _vTileChildIdx.clear(); }
};
