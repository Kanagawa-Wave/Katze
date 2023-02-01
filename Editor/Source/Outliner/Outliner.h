#pragma once

#include <Katze.h>

class Outliner
{
public:
    Outliner() = default;
    Outliner(Scene* scene);

    void SetContext(Scene* scene);
    void OnImGuiRender();

private:
    void DrawEntityNode(Entity entity);
    
    Scene* m_Context = nullptr;
    Entity m_SelectedEntity;
};
