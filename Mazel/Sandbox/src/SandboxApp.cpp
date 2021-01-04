#include <Mazel.h>
#include <imgui/imgui.h>

class ExampleLayer : public Mazel::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.0f, 1.0f, -1.0f, 1.0f)
	{
		m_VertexArray.reset(Mazel::VertexArray::Create());

		//Describe the data and upload them
		float vertices[3 * 7] = {
			//  COORD of the vert  |   COLOR of the vert     ---- see layout below
				-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
				 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
				 0.0f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f
		};

		std::shared_ptr<Mazel::VertexBuffer> m_VertexBuffer;
		m_VertexBuffer.reset(Mazel::VertexBuffer::Create(vertices, sizeof(vertices)));
		Mazel::BufferLayout buffer_layout = {
			{ Mazel::ShaderDataType::Float3, "a_Position"},
			{ Mazel::ShaderDataType::Float4, "a_Color"}
		};
		m_VertexBuffer->SetLayout(buffer_layout);
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<Mazel::IndexBuffer> m_IndexBuffer;
		m_IndexBuffer.reset(Mazel::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		float squareVertices[3 * 4] = {
				-0.75f, -0.75f, 0.0f,
				 0.75f, -0.75f, 0.0f,
				 0.75f,  0.75f, 0.0f,
				-0.75f,  0.75f, 0.0f,
		};

		m_SquareVA.reset(Mazel::VertexArray::Create());
		std::shared_ptr<Mazel::VertexBuffer> squareVB;
		squareVB.reset(Mazel::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ Mazel::ShaderDataType::Float3, "a_Position"},
			});
		m_SquareVA->AddVertexBuffer(squareVB);
		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Mazel::IndexBuffer> squareIB;
		squareIB.reset(Mazel::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
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

		m_Shader.reset(new Mazel::Shader(vertexWithColorSrc, fragmentWithColorSrc));
		m_ShaderBlue.reset(new Mazel::Shader(vertexWithBlueColorSrc, fragmentWithBlueColorSrc));
	}
	~ExampleLayer() = default;

	void OnUpdate() override
	{
		//Input polling
		if (Mazel::Input::IsKeyPressed(MZ_KEY_Q))
			angle -= rotateSpeed;
		if (Mazel::Input::IsKeyPressed(MZ_KEY_E))
			angle += rotateSpeed;

		if (Mazel::Input::IsKeyPressed(MZ_KEY_W))
			ud += moveSpeed;
		if (Mazel::Input::IsKeyPressed(MZ_KEY_S))
			ud -= moveSpeed;
		if (Mazel::Input::IsKeyPressed(MZ_KEY_A))
			lr -= moveSpeed;
		if (Mazel::Input::IsKeyPressed(MZ_KEY_D))
			lr += moveSpeed;

		Mazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Mazel::RenderCommand::Clear();

		m_Camera.SetPosition({ lr, ud, 0.0f });
		m_Camera.SetRotation(angle);

		Mazel::Renderer::BeginScene(m_Camera);

		Mazel::Renderer::Submit(m_ShaderBlue, m_SquareVA);
		Mazel::Renderer::Submit(m_Shader, m_VertexArray);

		Mazel::Renderer::EndScene();
	}

	void OnEvent(Mazel::Event& ev) override
	{
		//Handling movemenet by events
		Mazel::EventDispatcher dispatcher(ev);
		dispatcher.Dispatch<Mazel::KeyPressedEvent>(MZ_BIND_EVENT_FN(ExampleLayer::OnKeyPressedEvent));
	}

	bool OnKeyPressedEvent(Mazel::KeyPressedEvent& ev)
	{
		if (ev.GetKeyCode() == MZ_KEY_Q)
			angle -= rotateSpeed;
		if (ev.GetKeyCode() == MZ_KEY_E)
			angle += rotateSpeed;

		if (ev.GetKeyCode() == MZ_KEY_W)
			ud += moveSpeed;
		if (ev.GetKeyCode() == MZ_KEY_S)
			ud -= moveSpeed;
		if (ev.GetKeyCode() == MZ_KEY_A)
			lr -= moveSpeed;
		if (ev.GetKeyCode() == MZ_KEY_D)
			lr += moveSpeed;

		return false;
	}

	void OnImGuiRender() override
	{}

private:
	float angle = 0.0f;
	float ud = .0f, lr = .0f;

	const float rotateSpeed = 1.0f;
	const float moveSpeed = 0.01f;
private:
	std::shared_ptr<Mazel::Shader> m_Shader;
	std::shared_ptr<Mazel::Shader> m_ShaderBlue;
	std::shared_ptr<Mazel::VertexArray> m_VertexArray;
	std::shared_ptr<Mazel::VertexArray> m_SquareVA;

	Mazel::OrtographicCamera m_Camera;
};

class Sandbox : public Mazel::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}
	~Sandbox() = default;
};

Mazel::Application* Mazel::CreateApplication()
{
	return new Sandbox();
}