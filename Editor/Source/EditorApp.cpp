#include <Katze.h>
#include <EntryPoint.h>

#include "EditorLayer.h"

class Editor : public Application
{
public:
    Editor()
    {
        PushLayer(new EditorLayer());
    }
};

Application* CreateApplication()
{
    return new Editor();
}
