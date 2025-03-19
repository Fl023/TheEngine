#include "TheEngine.h"


class ExampleLayer : public TheEngine::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}
	void OnUpdate() override
	{
		//TE_INFO("ExampleLayer::Update");
		if (TheEngine::Input::IsKeyPressed(TE_KEY_TAB))
			TE_TRACE("Tab key is pressed (poll)!");
	}
	void OnEvent(TheEngine::Event& event) override
	{
		// TE_TRACE("{0}", event);
		if (event.GetEventType() == TheEngine::EventType::KeyPressed)
		{
			TheEngine::KeyPressedEvent& e = (TheEngine::KeyPressedEvent&)event;
			if (e.GetKeyCode() == TE_KEY_TAB)
				TE_TRACE("Tab key is pressed (event)!");
			TE_TRACE("{0}", (char)e.GetKeyCode());
		}
	}
};

class Sandbox : public TheEngine::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new TheEngine::ImGuiLayer());
	}
	~Sandbox()
	{
	}
};

TheEngine::Application* TheEngine::CreateApplication()
{
	return new Sandbox();
}
