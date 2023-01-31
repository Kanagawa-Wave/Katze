#pragma once

#include "Event/Event.h"

#include "Renderer/Context.h"

struct WindowProps
{
    std::string Title;
    unsigned int Width, Height;
    WindowProps(std::string T = "Engine", unsigned int W = 1920, unsigned int H = 1080) : Title(T), Width(W), Height(H) {}
};

class Window
{
public:
    using EventCallbackFn = std::function<void(Event&)>;
    
    Window(const WindowProps& props);
    ~Window();

    void Update();

    inline unsigned int GetWidth() const { return m_data.Width; }
    inline unsigned int GetHeight() const { return m_data.Height; }

    void SetEventCallback(const EventCallbackFn& callback)
    {
        m_data.EventCallback = callback;
    }
    void SetVSync(bool enabled);
    bool IsVSync() const;
    
    inline void* GetNativeWindow() const { return m_window; }
    inline GLFWwindow* GetGLFWWindow() const { return m_window; }

private:
    void Init(const WindowProps& props);
    void Shutdown();
    
    struct WindowData
    {
        std::string Title;
        unsigned int Width{}, Height{};
        bool VSync{};
        EventCallbackFn EventCallback;
    };

    WindowData m_data{};
    GLFWwindow* m_window = nullptr;
    Context* m_context = nullptr;
};
