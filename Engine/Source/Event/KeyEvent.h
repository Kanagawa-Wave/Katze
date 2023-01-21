#pragma once

#include "Event.h"

/// Base class for KeyPressedEvent & KeyReleasedEvent
class KeyEvent : public Event
{
public:
    inline int GetKeyCode() { return m_keycode; }

    // "|" stands for Bitwise OR
    // For example here, keyboard = 1 << 2 = 4 (100), Input = 1 << 1 = 2 (010)
    // Thus, Keyboard | Input = 110 = 6
    EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
protected:
    // The Constructor is in protected, thus this class cannot be instantiated, can only be called from derived classes
    KeyEvent(int keycode) : m_keycode(keycode) {}
	
    int m_keycode;
};

class KeyPressedEvent : public KeyEvent
{
public:
    KeyPressedEvent(int keycode, int repeatcout) : KeyEvent(keycode), m_repeatcount(repeatcout) {}
    inline int GetRepeatCount() { return m_repeatcount; }

    virtual std::string ToString() const override
    {
        std::stringstream ss;
        ss << "KeyPressedEvent: " << m_keycode << " (" << m_repeatcount << " repeats)";
        return ss.str();
    }

    EVENT_CLASS_TYPE(KeyPressed)
		
private:
    int m_repeatcount;
};

class KeyReleasedEvent : public KeyEvent
{
public:
    KeyReleasedEvent(int keycode) : KeyEvent(keycode) {}

    virtual std::string ToString() const override
    {
        std::stringstream ss;
        ss << "KeyReleasedEvent: " << m_keycode;
        return ss.str();
    }

    EVENT_CLASS_TYPE(KeyReleased)
};

class KeyTypedEvent : public KeyEvent
{
public:
    KeyTypedEvent(int keycode) : KeyEvent(keycode) {}

    virtual std::string ToString() const override
    {
        std::stringstream ss;
        ss << "KeyTypedEvent: " << m_keycode;
        return ss.str();
    }

    EVENT_CLASS_TYPE(KeyTyped)
};