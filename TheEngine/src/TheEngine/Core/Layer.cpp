#include "tepch.h"
#include "Layer.h"

namespace TheEngine {
	Layer::Layer(const std::string& name)
		: m_DebugName(name)
	{
	}
	Layer::~Layer()
	{
	}
}