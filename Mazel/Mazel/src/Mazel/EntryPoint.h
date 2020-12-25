#pragma once


#ifdef MZ_PLATFORM_WINDOWS
	
extern Mazel::Application* Mazel::CreateApplication();

int main(int argc, char** argv)
{
	Mazel::Log::Init();

	MZ_CORE_CRITICAL("Initialized log");
	int a = 5;
	MZ_INFO("Hello, var={0}", a);

	auto app = Mazel::CreateApplication();
	app->Run();
	delete app;
}


#else
	#error Mazel only supports Windows!
#endif //MZ_PLATFORM_WINDOWS