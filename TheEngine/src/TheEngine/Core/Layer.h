#pragma once

#include "TheEngine/Core/Core.h"
#include "TheEngine/Core/Timestep.h"
#include "TheEngine/Events/Event.h"

namespace TheEngine {
	class THEENGINE_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();
		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}
		inline const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};
} // namespace TheEngine