#pragma once

#ifdef TE_PLATFORM_WINDOWS

extern TheEngine::Application* TheEngine::CreateApplication();

int main(int argc, char** argv)
{
	TheEngine::Log::Init();
	TE_CORE_WARN("Initialized Log!");
	auto app = TheEngine::CreateApplication();
	app->Run();
	delete app;
}

#endif