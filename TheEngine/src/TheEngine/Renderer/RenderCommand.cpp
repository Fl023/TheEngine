#include "tepch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace TheEngine {

	Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();
	
}