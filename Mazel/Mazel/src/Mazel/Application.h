#pragma once

#include "Core.h"
#include "Events/Event.h"

namespace Mazel {

	class MAZEL_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	//Defined in a client
	Application* CreateApplication();
}
