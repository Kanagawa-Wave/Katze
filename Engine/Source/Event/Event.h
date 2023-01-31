#pragma once

#include "pch.h"

#include "Core.h"

/*  Events in Katze are currently blocking, meaning when an event occurs it
    immediately gets dispatched and must be dealt with right then an there.
    For the future, a better strategy might be to buffer events in an event
    bus and process them during the "event" part of the update stage.		*/

enum class EventType
{
    None = 0,
    WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
    Tick, Update, Render,
    KeyPressed, KeyReleased, KeyTyped,
    MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
};

enum EventCategory
{
    None = 0,
    EventCategoryApplication = BIT(0),
    EventCategoryInput = BIT(1),
    EventCategoryKeyboard = BIT(2),
    EventCategoryMouse = BIT(3),
    EventCategoryMouseButton = BIT(4),
};

/// The EVENT_CLASS_TYPE() macro is responsible for the implementation of the three abstract (virtual) functions declared
/// in the Event class as they must be defined in the derived classes of Event. This macro implements GetStaticType() and
/// GetName() and defines a new method GetEventType() for returning the EventType when using a pointer of the base class
/// Event.
#define EVENT_CLASS_TYPE(type)\
static EventType GetStaticType() { return EventType::##type; }\
virtual EventType GetEventType() const override { return GetStaticType(); }\
virtual const char* GetName() const override { return #type; }

/// The EVENT_CLASS_CATEGORY() macro is responsible for the implementation of abstract function GetCategoryFlags()
#define EVENT_CLASS_CATEGORY(category)\
virtual int GetCategoryFlags() const override { return category; }		

// Event class is the base class for all Events
class Event
{
    // friend classes can access each others' private & protected members
    friend class EventDispatcher;
public:
    virtual EventType GetEventType() const = 0;
    virtual const char* GetName() const = 0;
    virtual int GetCategoryFlags() const = 0;
    virtual std::string ToString() const { return GetName(); }

    inline bool IsInCategory(EventCategory Category)
    {
        return GetCategoryFlags() & Category;
    }
// protected:
    bool m_handled = false;
};

class EventDispatcher
{
    template<typename T> // T can be any class derived from Event base class
    using EventFn = std::function<bool(T&)>; // EventFn (as a function class) returns a bool and takes in a reference to a Event
public:
    EventDispatcher(Event& event) : m_event(event) {}
    
    template<typename T>
    bool Dispatch(EventFn<T> func)
    {
        if (m_event.GetEventType() == T::GetStaticType())
        {
            m_event.m_handled = func(*(T*)&m_event); // Cast the reference of m_event to the input Event class pointer type
            return true;
        }
        return false;
    }
private:
    Event& m_event;
};

inline std::ostream& operator<<(std::ostream& os, const Event& event)
{
    return os << event.ToString();
}