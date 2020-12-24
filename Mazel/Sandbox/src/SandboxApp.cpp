#include <Mazel.h>

class Sandbox : public Mazel::Application
{
public:
	Sandbox()
	{
	}
	~Sandbox()
	{
	}

};

Mazel::Application* Mazel::CreateApplication()
{
	return new Sandbox();
}