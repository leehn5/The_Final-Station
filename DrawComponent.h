#pragma once
#include "Component.h"

class Transform;

class DrawComponent : public Component
{
protected:
    Transform* _trans;
    bool _isCameraEffect = true;
    int _depth;

public:
    DrawComponent();
    ~DrawComponent();

    //virtual void Render() = 0;
    virtual void Render() { _isCameraEffect = _object->GetCameraAffect(); }
    Transform* GetTransform() { return _trans; }
    int GetDepth() { return _depth; }
    void SetDepth(int depth) { _depth = depth; }
    void SetCameraEffected(bool isEffect) { _isCameraEffect = isEffect; }
    bool GetCameraEffected() { return _isCameraEffect; }

};
