#include "pch.h"

#include "LayerStack.h"

LayerStack::LayerStack() = default;

LayerStack::~LayerStack()
{
    for (auto& layer : m_Layers)
        delete layer;
}

void LayerStack::PushLayer(Layer* layer)
{
    m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
    m_LayerInsertIndex++;
    layer->OnAttach();
}

void LayerStack::PopLayer(Layer* layer)
{
    const auto iterator = std::find(m_Layers.begin(), m_Layers.end(), layer);
    if (iterator != m_Layers.end())
    {
        layer->OnDetach();
        m_Layers.erase(iterator);
        m_LayerInsertIndex--;
    }
}

void LayerStack::PushOverlay(Layer* overlay)
{
    m_Layers.emplace_back(overlay);
}

void LayerStack::PopOverlay(Layer* overlay)
{
    auto iterator = std::find(m_Layers.begin(), m_Layers.begin() + m_LayerInsertIndex, overlay);
    if (iterator != m_Layers.end())
    {
        overlay->OnDetach();
        m_Layers.erase(iterator);
    }
}
