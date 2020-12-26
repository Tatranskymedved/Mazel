#pragma once

#include "Core.h"
#include "Window.h"
#include "Mazel/LayerStack.h"
#include "Mazel/Layer.h"
#include "Mazel/Events/Event.h"
#include "Mazel/Events/ApplicationEvent.h"

namespace Mazel
{
	class MAZEL_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
	};

	//To be defined in a client
	Application* CreateApplication();
}
