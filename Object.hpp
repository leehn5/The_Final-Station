#pragma once
#include "Object.h"
template<typename T>
T* Object::AddComponent()
{
    shared_ptr <Component> component = make_shared<T>();

    component->setGameObject(this);
    component->Init();
    if (dynamic_cast<DrawComponent*>(component.get()))
        _draw.push_back((DrawComponent*)component.get());
    _components.push_back(component);

    return (T*)component.get();
}
template<typename T>
vector<T*> Object::GetComponents()
{
    vector<T*> componentsList;
    for (auto c : _components)
    {
        if (dynamic_cast<T*>(c.get()))
            componentsList.push_back((T*)c.get());
    }
    return componentsList;
}


template<typename T>
T* Object::GetComponent()
{
    for (auto c : _components)
    {
        if (dynamic_cast<T*>(c.get()))
            return (T*)c.get();
    }
    return nullptr;
}

template<typename T>
T* Object::CreateObject(Object* parent)
{
    Object* Obj = new T();

    if (parent == nullptr)
        //Obj->SetParent(SCENEMANAGER->GetNowScene());
        Obj->SetParent(sceneManager::GetInstance()->GetNowScene());
    else
        Obj->SetParent(parent);

    return (T*)Obj;
}