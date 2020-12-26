#include <Mazel.h>

class ExampleLayer : public Mazel::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{}
	~ExampleLayer() {};

	void OnUpdate() override
	{
		//MZ_TRACE("ExampleLayer::OnUpdate()");
	}

	void OnEvent(Mazel::Event& ev) override
	{
		MZ_INFO("{0}", ev);
	}
};

class Sandbox : public Mazel::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new Mazel::ImGuiLayer());
	}
	~Sandbox()
	{}

};

Mazel::Application* Mazel::CreateApplication()
{
	return new Sandbox();
}