#include <Mazel.h>
#include <imgui/imgui.h>

class ExampleLayer : public Mazel::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{}
	~ExampleLayer() = default;

	void OnUpdate() override
	{
		if (Mazel::Input::IsMouseButtonPressed(MZ_MOUSE_BUTTON_1))
			MZ_INFO("Left mouse clicked.");
		//MZ_TRACE("ExampleLayer::OnUpdate()");
	}

	void OnEvent(Mazel::Event& ev) override
	{
		if (ev.GetEventType() == Mazel::EventType::KeyPressed)
		{
			Mazel::KeyPressedEvent& e = (Mazel::KeyPressedEvent&)ev;
			if (e.GetKeyCode() == MZ_KEY_TAB)
				MZ_INFO("Tab pressed!");
			MZ_TRACE("{0}", (char)e.GetKeyCode());
		}
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Test wind");
		if (ImGui::Button("Press me!"))
		{
			ImGui::OpenPopup("ImportAssetDialog");
		}
		if (ImGui::BeginPopupModal("ImportAssetDialog"))
		{
			ImGui::Text("test");
			if (ImGui::Button("Close"))
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::End();
	}
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