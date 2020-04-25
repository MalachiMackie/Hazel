#pragma once

#include "Cheezy/Core/Layer.h"
#include "Cheezy/Events/ApplicationEvent.h"
#include "Cheezy/Events/KeyEvent.h"
#include "Cheezy/Events/MouseEvent.h"

namespace Cheezy
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void Begin();
		void End();
		
	private:
		float m_Time = 0.0f;
	};
}

