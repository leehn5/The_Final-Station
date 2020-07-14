#pragma once
#include "Object.h"
class Scene : public Object
{
protected:
    b2World* _b2World;
    bool  _allowRelease = false;
    float32 timeStep;
    int32 velocityIterations;
    int32 positionIterations;

public:
    Scene();
    virtual ~Scene();

    //마이크로소프트 전용 디버깅 반환자인데
    //S_OK, E_FAIL, SUCCDED 등으로 초기화가 잘됐는지 아닌지 출력창에 호출함.

    virtual void Init();         //초기화 전용 함수
    virtual void Release();         //메모리 해제 함수
    virtual void Update();         //연산 전용 함수
    virtual void PhysicsUpdate();         //연산 전용 함수
    virtual void Render();

    b2World* GetWorld() { return _b2World; }

};
