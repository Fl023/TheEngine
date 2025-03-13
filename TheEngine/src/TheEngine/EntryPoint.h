#pragma once

#ifdef TE_PLATFORM_WINDOWS

extern TheEngine::Application* TheEngine::CreateApplication();

int main(int argc, char** argv)
{
	printf("TheEngine");
	auto app = TheEngine::CreateApplication();
	app->Run();
	delete app;
}

#endif