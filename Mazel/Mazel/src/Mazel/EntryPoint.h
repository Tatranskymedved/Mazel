#pragma once


#ifdef MZ_PLATFORM_WINDOWS
	
extern Mazel::Application* Mazel::CreateApplication();

int main(int argc, char** argv)
{
	auto app = Mazel::CreateApplication();
	app->Run();
	delete app;
}


#else
	#error Mazel only supports Windows!
#endif //MZ_PLATFORM_WINDOWS