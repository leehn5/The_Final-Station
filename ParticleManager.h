#pragma once
#include "Particle.h"
#include "ObjectPool.h"
class ParticleManager;
enum ParticleType
{
	TRIANGLE,
	CIRCLE,
	POSITION
};

//#define MAX_ANGLE_MAX PI
//#define MAX_ANGLE_MIN 2.1f
//
//#define MIN_ANGLE_MAX 1.1f
//#define MIN_ANGLE_MIN 0

class ParticlePool
{
	priority_queue<Particle*> pool;
	//queue<Object*> pool;
	vector<Particle*> ActivePool;
	ParticleManager* _Mgr;
public:
	ParticlePool() {};
	~ParticlePool() {};
	void InssertPool(int num);
	void InssertActiveObject();

	void Init(int size,string imgKey = "None",bool isFrame = false, float FPS = 0, bool isPhysics = false);
	inline Particle* GetPoolObject() { return pool.top(); }
	inline Particle* GetActivePoolObject(int num) { return ActivePool[num]; }
	inline int GetPoolSize() { return pool.size(); }
	inline int GetActivePoolSize() { return ActivePool.size(); }
	inline priority_queue<Particle*> GetPool() { return pool; }
	inline vector<Particle*> GetActivePool() { return ActivePool; }
	inline void SetLink(ParticleManager* mgr) { _Mgr = mgr; }
};

class ParticleManager
{
private:
	ParticleType _type;
	ParticlePool _pool;
	float _maxAngle;
	float _minAngle;
	float _maxSpeed;
	float _minSpeed;
	float _range;
	Vector2 _pos;
	Vector2 _scale;
	Graphic* pop;
	float _maxX;
	float _minX;
	float _angle;
	bool _isActive;
public:
	ParticleManager();
	~ParticleManager() {};
	void Init(int size,ParticleType type, Vector2 pos, Vector2 scale, string imgKey = "None", bool isFrame = false, float FPS = 0,bool isPhysics = false);

	void Update();
	void Render();

	void Triangle();

	void Circle();

	void Position();

	void SetRange(float range) { _range = range; }

	void SetIsActive(bool active) { _isActive = active; }
	bool GetIsActive() { return _isActive; }

	void KeyCon();
	//제발 라디안으로.... 주세요 
	void SetMaxAngle(float radian) { _maxAngle = radian; }

	//제발 라디안으로.... 주세요
	void SetMinAngle(float radian) { _minAngle = radian; }

	void SetMaxSpeed(float speed) { _maxSpeed = speed; }

	void SetMinSpeed(float speed) { _minSpeed = speed; }
	//POSITION타입 전용
	void SetAngle(float radian) { _angle = radian; }

	void SetPos(float x, float y) { _pos = Vector2(x, y); }
	void SetPos(Vector2 pos) { _pos = pos; }

	void SetMinPos(float x) { _minX = x; }
	void SetMaxPos(float x) { _maxX = x; }

	Vector2 GetPos() { return _pos; }

	void SetScale(float x, float y) { _scale = Vector2(x, y); }
	void SetScale(Vector2 scale) { _scale = scale; }

	Vector2 GetScale() { return _scale; }

	void SetAlphaSpeed(float speed);
	void SetPhysicsOn(bool active);

	float GetMaxAngle() { return _maxAngle; }
	float GetMinAngle() { return _minAngle; }
	float GetMaxSpeed() { return _maxSpeed; }
	float GetMinSpeed() { return _minSpeed; }

};

