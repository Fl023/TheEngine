#pragma once

#include "Camera.h"

#include "Texture.h"
#include "Mesh.h"
#include "Model.h"

namespace TheEngine {

	struct DirLight {
		glm::vec3 direction;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
	};

	struct SpotLight {
		glm::vec3 position;
		glm::vec3 direction;
		float cutOff;
		float outerCutOff;
		float constant;
		float linear;
		float quadratic;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
	};

	struct PointLight {
		glm::vec3 position;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		float constant;
		float linear;
		float quadratic;
	};


	class Renderer3D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera& camera);
		static void BeginSceneLight(const Camera& camera);
		static void BeginSceneDefault(const Camera& camera);
		static void BeginSceneSkybox(const Camera& camera);
		static void EndScene();
		static void EndSceneSkybox();

		// Primitives
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture);

		static void DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color);
		static void DrawLightCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color, const float& shininess);
		static void DrawLightCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color, const float& shininess, const Ref<Texture2D>& texture);
		static void DrawLightCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color, const float& shininess, const Ref<Texture2D>& diffuse, const Ref<Texture2D>& specular);
		static void DrawLightSource(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color);

		static void AddDirLight(const DirLight& light);
		static DirLight& GetDirLight(const int& index);
		static int GetDirLightCount();
		static void RemoveDirLight(const int& index);
		static void ClearDirLights();

		static void AddSpotLight(const SpotLight& light);
		static SpotLight& GetSpotLight(const int& index);
		static int GetSpotLightCount();
		static void RemoveSpotLight(const int& index);
		static void ClearSpotLights();

		static void AddPointLight(const PointLight& light);
		static PointLight& GetPointLight(const int& index);
		static int GetPointLightCount();
		static void RemovePointLight(const int& index);
		static void ClearPointLights();

		static void DrawSkybox(const Ref<TextureCube>& texture);
		static void DrawSkybox(const std::vector<std::string>& faces);

		static void DrawMesh
		(
			const Mesh& mesh,
			glm::mat4 matrix = glm::mat4(1.0f),
			glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f),
			glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
			glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)
		);

		static void DrawModel
		(
			const Ref<Model>& model,
			glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
			glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
			glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)
		);
	};

}