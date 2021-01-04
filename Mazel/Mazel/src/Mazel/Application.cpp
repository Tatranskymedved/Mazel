#include "mzpch.h"
#include "Application.h"

#include "Mazel/Core.h"
#include "Mazel/Log.h"
#include "Mazel/Input.h"
#include "Mazel/Renderer/Buffer.h"
#include "Mazel/Renderer/Renderer.h"
#include "Mazel/Renderer/OrtographicCamera.h"

#include "Mazel/KeyCodes.h"
#include <glm/ext/matrix_transform.hpp>

namespace Mazel
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
		: m_Camera(-1.0f, 1.0f, -1.0f, 1.0f)
	{
		MZ_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(MZ_BIND_EVENT_FN(Application::OnEvent));

		m_ImGuiLayer = new ImGuiLayer(); //Is deleted from ~LayerStack
		PushOverlay(m_ImGuiLayer);

		m_VertexArray.reset(VertexArray::Create());

		//Describe the data and upload them
		float vertices[3 * 7] = {
			//  COORD of the vert  |   COLOR of the vert     ---- see layout below
				-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
				 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
				 0.0f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f
		};

		std::shared_ptr<VertexBuffer> m_VertexBuffer;
		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		BufferLayout buffer_layout = {
			{ ShaderDataType::Float3, "a_Position"},
			{ ShaderDataType::Float4, "a_Color"}
		};
		m_VertexBuffer->SetLayout(buffer_layout);
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);



		float squareVertices[3 * 4] = {
				-0.75f, -0.75f, 0.0f,
				 0.75f, -0.75f, 0.0f,
				 0.75f,  0.75f, 0.0f,
				-0.75f,  0.75f, 0.0f,
		};

		m_SquareVA.reset(VertexArray::Create());
		std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ ShaderDataType::Float3, "a_Position"},
			});
		m_SquareVA->AddVertexBuffer(squareVB);
		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexWithColorSrc = R"(
			#version 330 core
            
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
            {
				v_Position = a_Position;
				v_Color = a_Color;
                gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
            }
		)";
		std::string fragmentWithColorSrc = R"(
			#version 330 core
            
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
            {
                color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
            }
		)";


		std::string vertexWithBlueColorSrc = R"(
			#version 330 core
            
			layout(location = 0) in vec3 a_Position;
			out vec3 v_Position;

			uniform mat4 u_ViewProjection;

			void main()
            {
				v_Position = a_Position;
                gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
            }
		)";
		std::string fragmentWithBlueColorSrc = R"(
			#version 330 core
            
			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			void main()
            {
				color = vec4(0.2f, 0.3f, 0.8f, 1.0);
            }
		)";

		m_Shader.reset(new Shader(vertexWithColorSrc, fragmentWithColorSrc));
		m_ShaderBlue.reset(new Shader(vertexWithBlueColorSrc, fragmentWithBlueColorSrc));

		//All drawing is done without shader specified. Graphics card has it own default shader that is used for rendering this.
		//Also there is no transform matrix to viewport, that is why we are setting it to those coords.
	}

	void Application::Run()
	{
		float angle = 0;
		float ud = .0f, lr = .0f;

		float rotateSpeed = 1;
		float moveSpeed = 0.01;

		while (m_Running)
		{
			if (Input::IsKeyPressed(MZ_KEY_Q))
				angle -= rotateSpeed;
			if (Input::IsKeyPressed(MZ_KEY_E))
				angle += rotateSpeed;

			if (Input::IsKeyPressed(MZ_KEY_W))
				ud += moveSpeed;
			if (Input::IsKeyPressed(MZ_KEY_S))
				ud -= moveSpeed;
			if (Input::IsKeyPressed(MZ_KEY_A))
				lr -= moveSpeed;
			if (Input::IsKeyPressed(MZ_KEY_D))
				lr += moveSpeed;

			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
			RenderCommand::Clear();

			m_Camera.SetPosition({ lr, ud, 0.0f });
			m_Camera.SetRotation(angle);

			Renderer::BeginScene(m_Camera);

			Renderer::Submit(m_ShaderBlue, m_SquareVA);
			Renderer::Submit(m_Shader, m_VertexArray);

			Renderer::EndScene();



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
