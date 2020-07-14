#pragma once
#include "Object.h"

#define SPEED 170.f
#define ToString(x) string(#x)

class PlayerState;
class Ability;
class Bullet;
class BulletManager;
class Item;

enum class Mode
{
	Combat,
	Non_Combat,
	None,
};

typedef enum class CollisionType 
{
	Ladder,
	Door,
	Interaction,
	Dead_Body,
	Key,
	Battery,
	TrainStop,
	FoodBox,
	MedkitBox,
	Vent,
	NPC,
	None,
} ColType;

typedef enum class AttackType : byte
{
	Hand,
	Pistol,
	None,
} AtkType;

typedef enum class Direction
{
	Left = 180,
	Down = 270,
	Right = 0,
	Up = 90,
}Dir;

enum class CursorWhere
{
	Left = -1,
	None,
	Right,
};

class Player : public Object
{
private:
	Sprite* _sprite;
	PhysicsBody* _physics;
	PhysicsBody* _floorPhysics;

	Object* _body;
	Object* _legs;
	Object* _arms;

	Object* _judgingFloor;

	shared_ptr<PlayerState> _state;

	string _prevState;

	CursorWhere _cw;
	Dir _dir;
	AttackType _atkType;
	CollisionType _colType;
	Mode _mode;

	float _armsAngle;
	float _slopAngle;
	float _ladderDistance;

	int _ammo;						// 장전되어 있는 탄약
	int _reminingAmmo;				// 남아있는 탄약
	int _magazine;					// 최대 장전 가능 개수
	int _money;

	bool _isLadderCol;
	bool _isBattery;
	bool _isHoldItem;
	bool _isMove;

	string _prevScene;

	Vector2 _ladderPos;
	Vector2 _ladderSize;

	shared_ptr<Ability> _ability;
	BulletManager* _bulletMgr;


private:
	typedef map<string, Item*> mInvenList;
	typedef map<string, Item*>::iterator mInvenIter;

	mInvenList _mInven;

public:
	virtual void Init();
	virtual void Release();
	virtual void Update();
	virtual void Render();

	void DirectionSprite();
	void ChangeState(shared_ptr<PlayerState> state);

	Object* GetBody() { return _body; }
	Object* GetLegs() { return _legs; }
	Object* GetArms() { return _arms; }
	Object* GetJudgingFloor() { return _judgingFloor; }

	shared_ptr<PlayerState> GetState() { return _state; }

	Sprite* GetSprite() { return _sprite; }
	PhysicsBody* GetPhysicsBody() { return _physics; }

	void SetSlopAngle(float angle) { _slopAngle = angle; }
	void SetDirection(Dir dir) { _dir = dir; }
	void SetAttackType(AtkType atk) { _atkType = atk; }
	void SetCollisionType(CollisionType col) { _colType = col; }
	void SetMode(Mode mode) { _mode = mode; }

	float GetSlopAngle() { return _slopAngle; }
	CursorWhere GetCursorWhere() { return _cw; }
	Direction GetDirection() { return _dir; }
	AtkType GetAttackType() { return _atkType; }
	CollisionType GetCollisionType() { return _colType; }
	Mode GetMode() { return _mode; }

	void SetIsLadderCollider(bool isCol) { _isLadderCol = isCol; }
	bool GetIsLadderCollider() { return _isLadderCol; }

	void SetLadderPosition(Vector2 pos) { _ladderPos = pos; }
	Vector2 GetLadderPosition() { return _ladderPos; }

	void SetLadderDistance(float dis) { _ladderDistance = dis; }
	float GetLadderDistance() { return _ladderDistance; }

	float GetArmAngle() { return _armsAngle; }

	void SetLadderSize(Vector2 size) { _ladderSize = size; }
	Vector2 GetLadderSize() { return _ladderSize; }

	shared_ptr<Ability> GetAbility() { return _ability; }
	BulletManager* GetBullet() { return _bulletMgr; }

	int GetMagazine() { return _magazine; }
	int GetReminingAmmo() { return _reminingAmmo; }
	int GetAmmo() { return _ammo; }
	void FireBullet(int value) { _ammo -= value; }

	void Reload();
	void Move();
	void Non_CombatModeMove();
	void Parse();

	map<string, Item*> GetInventory() { return _mInven; }
	
	void AddItem(string item, int count);

	void SetPrevState(string state) { _prevState = state; }
	string GetPrevState() { return _prevState; }

	void SetIsBattery(bool isBattery) { _isBattery = isBattery; }
	bool GetIsBattery() { return _isBattery; }

	void SetIsHoldItem(bool isHold) { _isHoldItem = isHold; }
	bool GetIsHoldItem() { return _isHoldItem; }

	void SetIsMove(bool isMove) { _isMove = isMove; }
	bool GetIsMove() { return _isMove; }

	string GetPrevScene() { return _prevScene; }
};

