#pragma once

#include "Cheezy.h"

using namespace Cheezy;

class FloorScript : public ScriptComponent
{
	virtual void OnStart() override
	{
		m_Shape = m_CheezyObject->GetShape();
		SetColor(m_Color);
	}

	virtual void OnEvent(Event& e) override
	{
		EventDispatcher dispatcher{e};
		dispatcher.Dispatch<KeyPressedEvent>(CZ_BIND_EVENT_FN(FloorScript::OnKeyDown));
	}

	bool OnKeyDown(KeyPressedEvent& e)
	{
		if (e.GetRepeatCount() != 0)
			return false;

		if (e.GetKeyCode() == CZ_KEY_C)
		{
			SetColor(m_Color == s_HighlightedColor
				? s_DefaultColor
				: s_HighlightedColor);
		}
		
		return false;
	}

private:
	void SetColor(glm::vec4 color)
	{
		m_Color = color;
		m_Shape->SetColor(m_Color);
	}

private:
	Ref<Shape2D> m_Shape;
	glm::vec4 m_Color = s_HighlightedColor;

private:
	static glm::vec4 s_HighlightedColor;
	static glm::vec4 s_DefaultColor;
};

glm::vec4 FloorScript::s_HighlightedColor = glm::vec4(1.0f, 1.0f, 0.3f, 0.3f);
glm::vec4 FloorScript::s_DefaultColor = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);