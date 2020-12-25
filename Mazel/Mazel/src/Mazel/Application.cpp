#include "mzpch.h"
#include "Application.h"

#include "Mazel/Events/ApplicationEvent.h"
#include "Mazel/Log.h"

namespace Mazel {

	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (true)
		{
			m_Window->OnUpdate();
		}
	}
}
