#pragma once

#ifdef TE_PLATFORM_WINDOWS

extern TheEngine::Application* TheEngine::CreateApplication();

int main(int argc, char** argv)
{
	TheEngine::Log::Init();
	TE_CORE_WARN("Initialized Log!");
	int a = 5;
	TE_INFO("Hello! Var={0}", a);
	auto app = TheEngine::CreateApplication();
	app->Run();
	delete app;
}

#endif