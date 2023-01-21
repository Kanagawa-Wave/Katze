#pragma once

#include "Event.h"

class WindowResizeEvent : public Event
{
public:
    WindowResizeEvent(unsigned int width, unsigned int height) : m_width(width), m_height(height) {}

    inline unsigned int GetWidth() const { return m_width; }
    inline unsigned int GetHeight() const { return m_height; }

    virtual std::string ToString() const override
    {
        std::stringstream ss;
        ss << "WindowResizeEvent: " << m_width << ", " << m_height;
        return ss.str();
    }
		
    EVENT_CLASS_TYPE(WindowResize)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
private:
    unsigned int m_width;
    unsigned int m_height;
};

class WindowCloseEvent : public Event
{
public:
    WindowCloseEvent() {}

    EVENT_CLASS_TYPE(WindowClose)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class TickEvent : public Event
{
public:
    TickEvent() {}

    EVENT_CLASS_TYPE(Tick)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class UpdateEvent : public Event
{
public:
    UpdateEvent() {}

    EVENT_CLASS_TYPE(Update)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class RenderEvent : public Event
{
public:
    RenderEvent() {}

    EVENT_CLASS_TYPE(Render)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
};