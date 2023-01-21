#pragma once

#include "Event.h"

class MouseMovedEvent : public Event
{
public:
	MouseMovedEvent(float mouseX, float mouseY) : m_mouseX(mouseX), m_mouseY(mouseY) {}

	inline float GetX() { return m_mouseX; }
	inline float GetY() { return m_mouseY; }

	virtual std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseMovedEvent: " << m_mouseX << ", " << m_mouseY;
		return ss.str();
	}

	EVENT_CLASS_TYPE(MouseMoved)
	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	
private:
	float m_mouseX, m_mouseY;
};

class MouseScrolledEvent : public Event
{
public:
	MouseScrolledEvent(float offsetX, float offsetY) : m_offsetX(offsetX), m_offsetY(offsetY) {}

	inline float GetOffsetX() { return m_offsetX; }
	inline float GetOffsetY() { return m_offsetY; }

	virtual std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseScrolledEvent: " << m_offsetX << ", " << m_offsetY;
		return ss.str();
	}

	EVENT_CLASS_TYPE(MouseScrolled)
	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	
private:
	float m_offsetX, m_offsetY;
};

class MouseButton : public Event
{
public:
	inline int GetButton() { return m_button; }
	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
protected:
	MouseButton(int button) : m_button(button) {}
	int m_button;
};

class MouseButtonPressedEvent : public MouseButton
{
public:
	MouseButtonPressedEvent(int button) : MouseButton(button) {}

	virtual std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseButtonPressedEvent: " << m_button;
		return ss.str();
	}

	EVENT_CLASS_TYPE(MouseButtonPressed)
};

class  MouseButtonReleasedEvent : public MouseButton
{
public:
	MouseButtonReleasedEvent(int button) : MouseButton(button) {}

	virtual std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseButtonReleasedEvent: " << m_button;
		return ss.str();
	}

	EVENT_CLASS_TYPE(MouseButtonReleased)
};