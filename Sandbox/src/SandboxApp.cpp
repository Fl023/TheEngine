#include "TheEngine.h"

class Sandbox : public TheEngine::Application
{
public:
	Sandbox()
	{
	}
	~Sandbox()
	{
	}
};

TheEngine::Application* TheEngine::CreateApplication()
{
	return new Sandbox();
}
