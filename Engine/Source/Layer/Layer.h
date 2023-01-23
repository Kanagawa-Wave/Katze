#pragma once

#include "Core/Timestep.h"

class Layer
{
public:
    Layer(const std::string& name = "Layer");
    virtual ~Layer();

    virtual void OnAttach() {}
    virtual void OnDetach() {}
    virtual void OnUpdate(Timestep& ts) {}
    virtual void OnEvent(Event& event) {}
    virtual void OnImGuiRender() {}

    inline const std::string& GetName() const { return m_Name; }
		
protected:
    std::string m_Name;
    
};
