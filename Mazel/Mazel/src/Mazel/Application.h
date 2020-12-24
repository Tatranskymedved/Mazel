#pragma once

#include "Core.h"

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
