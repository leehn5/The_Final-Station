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

    //����ũ�μ���Ʈ ���� ����� ��ȯ���ε�
    //S_OK, E_FAIL, SUCCDED ������ �ʱ�ȭ�� �ߵƴ��� �ƴ��� ���â�� ȣ����.

    virtual void Init();         //�ʱ�ȭ ���� �Լ�
    virtual void Release();         //�޸� ���� �Լ�
    virtual void Update();         //���� ���� �Լ�
    virtual void PhysicsUpdate();         //���� ���� �Լ�
    virtual void Render();

    b2World* GetWorld() { return _b2World; }

};
