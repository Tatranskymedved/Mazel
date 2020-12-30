#pragma once

#include "Mazel/Layer.h"

#include "Mazel/Events/ApplicationEvent.h"
#include "Mazel/Events/KeyEvent.h"
#include "Mazel/Events/MouseEvent.h"

namespace Mazel
{
	class MAZEL_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};
}

