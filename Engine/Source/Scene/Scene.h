#pragma once

#include "GameObject.h"

struct Scene
{
    Camera* camera;
    GameObject::Map Objects;
    UniformBuffer ubo;
    GlobalUbo uboData;
    float DeltaTime;

    inline void SetData() { ubo.SetData(uboData); }
    inline void Init()
    {
        camera = new Camera(45.0f, 0.1f, 1000000.0f);
        ubo.Init();
    }
    inline void OnUpdate(float dt)
    {
        DeltaTime = dt;
        camera->OnUpdate(dt);
    }
};
