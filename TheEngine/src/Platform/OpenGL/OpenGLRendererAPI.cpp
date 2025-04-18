#include "tepch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace TheEngine
{
	void OpenGLRendererAPI::Init()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);

		//// Enables Cull Facing
		//glEnable(GL_CULL_FACE);
		//// Keeps front faces
		//glCullFace(GL_BACK);
		//// Uses counter clock-wise standard
		//glFrontFace(GL_CCW);
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::SetDepthFunction(uint32_t depthFunc)
	{
		switch (depthFunc)
		{
		case 0:
			glDepthFunc(GL_LESS);
			break;
		case 1:
			glDepthFunc(GL_LEQUAL);
			break;
		case 2:
			glDepthFunc(GL_EQUAL);
			break;
		case 3:
			glDepthFunc(GL_GREATER);
			break;
		case 4:
			glDepthFunc(GL_GEQUAL);
			break;
		case 5:
			glDepthFunc(GL_NOTEQUAL);
			break;
		case 6:
			glDepthFunc(GL_ALWAYS);
			break;
		default:
			glDepthFunc(GL_LESS);
			break;
		}
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}
	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
