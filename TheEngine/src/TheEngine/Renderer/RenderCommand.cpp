#include "tepch.h"
#include "RenderCommand.h"

#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"


namespace TheEngine {

	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
	
}