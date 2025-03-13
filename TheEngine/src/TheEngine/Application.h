#pragma once

#include "Core.h"	

namespace TheEngine {

	class THEENGINE_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}


