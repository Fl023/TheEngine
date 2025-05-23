#pragma once

#include "Core.h"
#include "Window.h"
#include "TheEngine/Core/LayerStack.h"
#include "TheEngine/Events/Event.h"
#include "TheEngine/Events/ApplicationEvent.h"

#include "TheEngine/ImGui/ImGuiLayer.h"

#include "TheEngine/Renderer/Shader.h"
#include "TheEngine/Renderer/Buffer.h"
#include "TheEngine/Renderer/VertexArray.h"

#include "TheEngine/Core/Timestep.h"

#include "TheEngine/Renderer/Camera.h"


namespace TheEngine {

	class THEENGINE_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;
	private:
		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}