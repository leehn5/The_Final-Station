#pragma once
class BoxWorldManager
{
private:
	b2World* _world;

	BoxWorldManager();

public:
	static BoxWorldManager* GetInstance()
	{
		static BoxWorldManager* instance = new BoxWorldManager();
		return instance;
	}

	~BoxWorldManager();

	virtual void Release();
	void CreateWorld(b2Vec2 gravity = b2Vec2_zero);
	void RemoveBody(b2Body* body);
	void SetWorld();
	void SetWorld(b2World* world);
	inline b2World* GetWorld() { return _world; }
};

