#pragma once

#include "../../../vendor/imgui/imgui.h"

#include "TheEngine/Layer.h"

#include "TheEngine/Events/ApplicationEvent.h"
#include "TheEngine/Events/KeyEvent.h"
#include "TheEngine/Events/MouseEvent.h"

namespace TheEngine {
	class THEENGINE_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach();
		void OnDetach();
		void OnUpdate();
		void OnEvent(Event& event);
	private:
		ImGuiKey MapGLFWKeyToImGuiKey(int keycode);
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
		bool OnMouseMovedEvent(MouseMovedEvent& e);
		bool OnMouseScrolledEvent(MouseScrolledEvent& e);
		bool OnKeyPressedEvent(KeyPressedEvent& e);
		bool OnKeyReleasedEvent(KeyReleasedEvent& e);
		bool OnKeyTypedEvent(KeyTypedEvent& e);
		bool OnWindowResizeEvent(WindowResizeEvent& e);
	private:
		float m_Time = 0.0f;
		std::unordered_map<int, ImGuiKey> m_keyMap; // Member variable
	};
}