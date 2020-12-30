#include "mzpch.h"
#include "Application.h"

#include "Mazel/Core.h"
#include "Mazel/Log.h"
#include "Mazel/Input.h"

#include <glad/glad.h>

namespace Mazel
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		MZ_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(MZ_BIND_EVENT_FN(Application::OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{}

	void Application::Run()
	{
		while (m_Running)
		{
			//Needs to be called to clear buffer, on which we can draw. Once we draw things, we can swap it with visible one.
			glClear(GL_COLOR_BUFFER_BIT);

			//Update part
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			//Render part
			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(MZ_BIND_EVENT_FN(Application::OnWindowClose));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
	}
}
