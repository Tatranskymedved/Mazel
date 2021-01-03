#include "mzpch.h"
#include "Application.h"

#include "Mazel/Core.h"
#include "Mazel/Log.h"
#include "Mazel/Input.h"
#include "Mazel/Renderer/Buffer.h"

#include <glad/glad.h>

namespace Mazel
{
	Application* Application::s_Instance = nullptr;

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case Mazel::ShaderDataType::Float:    return GL_FLOAT;
			case Mazel::ShaderDataType::Float2:   return GL_FLOAT;
			case Mazel::ShaderDataType::Float3:   return GL_FLOAT;
			case Mazel::ShaderDataType::Float4:   return GL_FLOAT;
			case Mazel::ShaderDataType::Mat3:     return GL_FLOAT;
			case Mazel::ShaderDataType::Mat4:     return GL_FLOAT;
			case Mazel::ShaderDataType::Int:      return GL_INT;
			case Mazel::ShaderDataType::Int2:     return GL_INT;
			case Mazel::ShaderDataType::Int3:     return GL_INT;
			case Mazel::ShaderDataType::Int4:     return GL_INT;
			case Mazel::ShaderDataType::Bool:     return GL_BOOL;
		}

		MZ_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	Application::Application()
	{
		MZ_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(MZ_BIND_EVENT_FN(Application::OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		//Generate arrays/buffers in OpenGL
		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		//Describe the data and upload them
		float vertices[3 * 7] = {
			//  COORD of the vert  |   COLOR of the vert     ---- see layout below
				-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
				 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
				 0.0f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f
		};

		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		BufferLayout buf_layout = {
			{ ShaderDataType::Float3, "a_Position"},
			{ ShaderDataType::Float4, "a_Color"}
		};
		m_VertexBuffer->SetLayout(buf_layout);

		uint32_t index = 0;
		const auto& layout = m_VertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset
			);
			index++;
		}

		//Create buffers with "indexes", that point to existing "data" (vertices). OpenGL will go over each index and draw it.
		uint32_t indices[3] = { 0, 1, 2 };
		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		//m_IndexBuffer->Bind();

		std::string vertexSrc = R"(
			#version 330 core
            
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
			out vec3 v_Position;
			out vec4 v_Color;

			void main()
            {
				v_Position = a_Position;
				v_Color = a_Color;
                gl_Position = vec4(a_Position, 1.0);
            }
		)";
		std::string fragmentSrc = R"(
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

		m_Shader.reset(new Shader(vertexSrc, fragmentSrc));

		//All drawing is done without shader specified. Graphics card has it own default shader that is used for rendering this.
		//Also there is no transform matrix to viewport, that is why we are setting it to those coords.
	}

	Application::~Application()
	{}

	void Application::Run()
	{
		while (m_Running)
		{
			//Needs to be called to clear buffer, on which we can draw. Once we draw things, we can swap it with visible one.
			glClear(GL_COLOR_BUFFER_BIT);

			m_Shader->Bind();
			//To draw elements from the buffers
			glBindVertexArray(m_VertexArray);
			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

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
