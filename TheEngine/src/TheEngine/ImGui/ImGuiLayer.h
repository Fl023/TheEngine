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

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
		std::unordered_map<int, ImGuiKey> m_keyMap; // Member variable
	};
}