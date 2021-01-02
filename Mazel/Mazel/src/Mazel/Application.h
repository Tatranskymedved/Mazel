#pragma once

#include "Core.h"
#include "Window.h"
#include "Mazel/LayerStack.h"
#include "Mazel/Layer.h"
#include "Mazel/Events/Event.h"
#include "Mazel/Events/ApplicationEvent.h"

#include "Mazel/ImGui/ImGuiLayer.h"

#include "Mazel/Renderer/Shader.h"

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

		inline Window& GetWindow() { return *m_Window; }

		inline static Application& Get() { return *s_Instance; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;

		unsigned int m_VertexArray, m_VertexBuffer, m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;
	private:
		static Application* s_Instance;
	};

	//To be defined in a client
	Application* CreateApplication();
}
