#pragma once

#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace TheEngine {

	class Mesh
	{
	public:

		Ref<VertexArray> m_VertexArray;
		std::vector<Ref<TheEngine::Texture2D>> m_Textures;

		// Initializes the mesh
		Mesh(float* vertices, BufferLayout& layout, uint32_t* indices, std::vector<Ref<TheEngine::Texture2D>>& textures);
		Mesh(std::vector<float>& vertices, BufferLayout& layout, std::vector<uint32_t>& indices, std::vector<Ref<TheEngine::Texture2D>>& textures);

	};

}