#include "tepch.h"
#include "Renderer3D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace TheEngine {

	struct Renderer3DStorage {
		Ref<VertexArray> QuadVertexArray;
		Ref<VertexArray> CubeVertexArray;
		Ref<VertexArray> SkyboxVertexArray;
		Ref<Shader> TextureShader;
		Ref<Shader> TextureLightShader;
		Ref<Shader> SkyboxShader;
		Ref<Shader> DefaultShader;
		Ref<Texture2D> WhiteTexture;

		glm::vec3 CameraPosition;

		std::vector <DirLight> DirLights;
		std::vector <SpotLight> SpotLights;
		std::vector<PointLight> PointLights;
	};

	static Renderer3DStorage* s_Data;

	void Renderer3D::Init()
	{
		s_Data = new Renderer3DStorage();
		s_Data->QuadVertexArray = VertexArray::Create();

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Ref<VertexBuffer> squareVB = VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		squareVB->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" }
			});
		s_Data->QuadVertexArray->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> squareIB = IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		s_Data->QuadVertexArray->SetIndexBuffer(squareIB);


		// Create the Cube Vertex Array
		s_Data->CubeVertexArray = VertexArray::Create();

		float cubeVertices[] = {
			// positions           // tex coords  // normals
			// Front face (normal: 0, 0, 1)
			-0.5f, -0.5f,  0.5f,    0.0f, 0.0f,    0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,    1.0f, 0.0f,    0.0f, 0.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,    1.0f, 1.0f,    0.0f, 0.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,    0.0f, 1.0f,    0.0f, 0.0f, 1.0f,

			// Back face (normal: 0, 0, -1)
			-0.5f, -0.5f, -0.5f,    1.0f, 0.0f,    0.0f, 0.0f, -1.0f,
			 0.5f, -0.5f, -0.5f,    0.0f, 0.0f,    0.0f, 0.0f, -1.0f,
			 0.5f,  0.5f, -0.5f,    0.0f, 1.0f,    0.0f, 0.0f, -1.0f,
			-0.5f,  0.5f, -0.5f,    1.0f, 1.0f,    0.0f, 0.0f, -1.0f,

			// Left face (normal: -1, 0, 0)
			-0.5f, -0.5f, -0.5f,    0.0f, 0.0f,    -1.0f, 0.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,    1.0f, 0.0f,    -1.0f, 0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,    1.0f, 1.0f,    -1.0f, 0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,    0.0f, 1.0f,    -1.0f, 0.0f, 0.0f,

			// Right face (normal: 1, 0, 0)
			 0.5f, -0.5f, -0.5f,    1.0f, 0.0f,    1.0f, 0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,    0.0f, 0.0f,    1.0f, 0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,    0.0f, 1.0f,    1.0f, 0.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,    1.0f, 1.0f,    1.0f, 0.0f, 0.0f,

			 // Top face (normal: 0, 1, 0)
			 -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,    0.0f, 1.0f, 0.0f,
			  0.5f,  0.5f, -0.5f,    1.0f, 1.0f,    0.0f, 1.0f, 0.0f,
			  0.5f,  0.5f,  0.5f,    1.0f, 0.0f,    0.0f, 1.0f, 0.0f,
			 -0.5f,  0.5f,  0.5f,    0.0f, 0.0f,    0.0f, 1.0f, 0.0f,

			 // Bottom face (normal: 0, -1, 0)
			 -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,    0.0f, -1.0f, 0.0f,
			  0.5f, -0.5f, -0.5f,    1.0f, 0.0f,    0.0f, -1.0f, 0.0f,
			  0.5f, -0.5f,  0.5f,    1.0f, 1.0f,    0.0f, -1.0f, 0.0f,
			 -0.5f, -0.5f,  0.5f,    0.0f, 1.0f,    0.0f, -1.0f, 0.0f,
		};

		Ref<VertexBuffer> cubeVB = VertexBuffer::Create(cubeVertices, sizeof(cubeVertices));
		cubeVB->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Float3, "a_Normal" }
			});
		s_Data->CubeVertexArray->AddVertexBuffer(cubeVB);

		uint32_t cubeIndices[] = {
			// Front face
			 0,  1,  2,  2,  3,  0,
			 // Back face
			  4,  5,  6,  6,  7,  4,
			  // Left face
			   8,  9, 10, 10, 11,  8,
			   // Right face
				12, 13, 14, 14, 15, 12,
				// Top face
				 16, 17, 18, 18, 19, 16,
				 // Bottom face
				  20, 21, 22, 22, 23, 20
		};

		Ref<IndexBuffer> cubeIB = IndexBuffer::Create(cubeIndices, sizeof(cubeIndices) / sizeof(uint32_t));
		s_Data->CubeVertexArray->SetIndexBuffer(cubeIB);

		// Create the skybox vertex array

		s_Data->SkyboxVertexArray = VertexArray::Create();

		float skyboxVertices[] =
		{
			//   Coordinates
			-1.0f, -1.0f,  1.0f,//        7--------6
			 1.0f, -1.0f,  1.0f,//       /|       /|
			 1.0f, -1.0f, -1.0f,//      4--------5 |
			-1.0f, -1.0f, -1.0f,//      | |      | |
			-1.0f,  1.0f,  1.0f,//      | 3------|-2
			 1.0f,  1.0f,  1.0f,//      |/       |/
			 1.0f,  1.0f, -1.0f,//      0--------1
			-1.0f,  1.0f, -1.0f
		};

		Ref<VertexBuffer> skyboxVB = VertexBuffer::Create(skyboxVertices, sizeof(skyboxVertices));
		skyboxVB->SetLayout({
			{ ShaderDataType::Float3, "a_Position" }
			});
		s_Data->SkyboxVertexArray->AddVertexBuffer(skyboxVB);

		unsigned int skyboxIndices[] =
		{
			// Right
			1, 2, 6,
			6, 5, 1,
			// Left
			0, 4, 7,
			7, 3, 0,
			// Top
			4, 5, 6,
			6, 7, 4,
			// Bottom
			0, 3, 2,
			2, 1, 0,
			// Back
			0, 1, 5,
			5, 4, 0,
			// Front
			3, 7, 6,
			6, 2, 3
		};

		Ref<IndexBuffer> skyboxIB = IndexBuffer::Create(skyboxIndices, sizeof(skyboxIndices) / sizeof(uint32_t));
		s_Data->SkyboxVertexArray->SetIndexBuffer(skyboxIB);


		s_Data->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		s_Data->TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetInt("u_Texture", 0);

		s_Data->TextureLightShader = Shader::Create("assets/shaders/Lighting.glsl");
		s_Data->TextureLightShader->Bind();
		s_Data->TextureLightShader->SetInt("material.diffuse", 0);
		s_Data->TextureLightShader->SetInt("material.specular", 1);

		s_Data->SkyboxShader = Shader::Create("assets/shaders/Skybox.glsl");
		s_Data->SkyboxShader->Bind();
		s_Data->SkyboxShader->SetInt("u_Skybox", 0);

		s_Data->DefaultShader = Shader::Create("assets/shaders/Default.glsl");
	}

	void Renderer3D::Shutdown()
	{
		delete s_Data;
	}

	void Renderer3D::BeginScene(const Camera& camera)
	{

		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer3D::BeginSceneLight(const Camera& camera)
	{
		s_Data->TextureLightShader->Bind();
		s_Data->TextureLightShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		s_Data->CameraPosition = camera.GetPosition();

		// Pass camera position
		s_Data->TextureLightShader->SetFloat3("u_CameraPosition", s_Data->CameraPosition);

		// Pass directional lights
		int numDirLights = s_Data->DirLights.size();
		s_Data->TextureLightShader->SetInt("u_NumDirLights", numDirLights);

		for (int i = 0; i < numDirLights; i++) {
			const DirLight& light = s_Data->DirLights[i];
			std::string base = "dirLights[" + std::to_string(i) + "].";

			s_Data->TextureLightShader->SetFloat3(base + "direction", light.direction);
			s_Data->TextureLightShader->SetFloat3(base + "ambient", light.ambient);
			s_Data->TextureLightShader->SetFloat3(base + "diffuse", light.diffuse);
			s_Data->TextureLightShader->SetFloat3(base + "specular", light.specular);
		}

		// Pass spotlights
		int numSpotLights = s_Data->SpotLights.size();
		s_Data->TextureLightShader->SetInt("u_NumSpotLights", numSpotLights);

		for (int i = 0; i < numSpotLights; i++) {
			const SpotLight& light = s_Data->SpotLights[i];
			std::string base = "spotLights[" + std::to_string(i) + "].";

			s_Data->TextureLightShader->SetFloat3(base + "position", light.position);
			s_Data->TextureLightShader->SetFloat3(base + "direction", light.direction);
			s_Data->TextureLightShader->SetFloat(base + "cutOff", light.cutOff);
			s_Data->TextureLightShader->SetFloat(base + "outerCutOff", light.outerCutOff);
			s_Data->TextureLightShader->SetFloat(base + "constant", light.constant);
			s_Data->TextureLightShader->SetFloat(base + "linear", light.linear);
			s_Data->TextureLightShader->SetFloat(base + "quadratic", light.quadratic);
			s_Data->TextureLightShader->SetFloat3(base + "ambient", light.ambient);
			s_Data->TextureLightShader->SetFloat3(base + "diffuse", light.diffuse);
			s_Data->TextureLightShader->SetFloat3(base + "specular", light.specular);
		}

		// Pass point lights
		int numPointLights = s_Data->PointLights.size();
		s_Data->TextureLightShader->SetInt("u_NumPointLights", numPointLights);

		for (int i = 0; i < numPointLights; i++) {
			const PointLight& light = s_Data->PointLights[i];
			std::string base = "pointLights[" + std::to_string(i) + "].";

			s_Data->TextureLightShader->SetFloat3(base + "position", light.position);
			s_Data->TextureLightShader->SetFloat3(base + "ambient", light.ambient);
			s_Data->TextureLightShader->SetFloat3(base + "diffuse", light.diffuse);
			s_Data->TextureLightShader->SetFloat3(base + "specular", light.specular);
			s_Data->TextureLightShader->SetFloat(base + "constant", light.constant);
			s_Data->TextureLightShader->SetFloat(base + "linear", light.linear);
			s_Data->TextureLightShader->SetFloat(base + "quadratic", light.quadratic);
		}
	}

	void Renderer3D::BeginSceneDefault(const Camera& camera)
	{
		s_Data->DefaultShader->Bind();
		s_Data->DefaultShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		s_Data->CameraPosition = camera.GetPosition();

		// Pass camera position
		s_Data->DefaultShader->SetFloat3("u_CameraPosition", s_Data->CameraPosition);

		// Pass directional lights
		int numDirLights = s_Data->DirLights.size();
		s_Data->DefaultShader->SetInt("u_NumDirLights", numDirLights);

		for (int i = 0; i < numDirLights; i++) {
			const DirLight& light = s_Data->DirLights[i];
			std::string base = "dirLights[" + std::to_string(i) + "].";

			s_Data->DefaultShader->SetFloat3(base + "direction", light.direction);
			s_Data->DefaultShader->SetFloat3(base + "ambient", light.ambient);
			s_Data->DefaultShader->SetFloat3(base + "diffuse", light.diffuse);
			s_Data->DefaultShader->SetFloat3(base + "specular", light.specular);
		}

		// Pass spotlights
		int numSpotLights = s_Data->SpotLights.size();
		s_Data->DefaultShader->SetInt("u_NumSpotLights", numSpotLights);

		for (int i = 0; i < numSpotLights; i++) {
			const SpotLight& light = s_Data->SpotLights[i];
			std::string base = "spotLights[" + std::to_string(i) + "].";

			s_Data->DefaultShader->SetFloat3(base + "position", light.position);
			s_Data->DefaultShader->SetFloat3(base + "direction", light.direction);
			s_Data->DefaultShader->SetFloat(base + "cutOff", light.cutOff);
			s_Data->DefaultShader->SetFloat(base + "outerCutOff", light.outerCutOff);
			s_Data->DefaultShader->SetFloat(base + "constant", light.constant);
			s_Data->DefaultShader->SetFloat(base + "linear", light.linear);
			s_Data->DefaultShader->SetFloat(base + "quadratic", light.quadratic);
			s_Data->DefaultShader->SetFloat3(base + "ambient", light.ambient);
			s_Data->DefaultShader->SetFloat3(base + "diffuse", light.diffuse);
			s_Data->DefaultShader->SetFloat3(base + "specular", light.specular);
		}

		// Pass point lights
		int numPointLights = s_Data->PointLights.size();
		s_Data->DefaultShader->SetInt("u_NumPointLights", numPointLights);

		for (int i = 0; i < numPointLights; i++) {
			const PointLight& light = s_Data->PointLights[i];
			std::string base = "pointLights[" + std::to_string(i) + "].";

			s_Data->DefaultShader->SetFloat3(base + "position", light.position);
			s_Data->DefaultShader->SetFloat3(base + "ambient", light.ambient);
			s_Data->DefaultShader->SetFloat3(base + "diffuse", light.diffuse);
			s_Data->DefaultShader->SetFloat3(base + "specular", light.specular);
			s_Data->DefaultShader->SetFloat(base + "constant", light.constant);
			s_Data->DefaultShader->SetFloat(base + "linear", light.linear);
			s_Data->DefaultShader->SetFloat(base + "quadratic", light.quadratic);
		}
	}

	void Renderer3D::BeginSceneSkybox(const Camera& camera)
	{
		RenderCommand::SetDepthFunction(1); //GL_LEQUAL
		s_Data->SkyboxShader->Bind();
		glm::mat4 view(glm::mat3(camera.GetViewMatrix()));
		glm::mat4 projection(camera.GetProjectionMatrix());
		s_Data->SkyboxShader->SetMat4("u_ViewProjection", projection * view);
	}


	void Renderer3D::EndScene()
	{

	}

	void Renderer3D::EndSceneSkybox()
	{
		RenderCommand::SetDepthFunction(0); //GL_LESS
	}

	void Renderer3D::AddDirLight(const DirLight& light) {
		if (s_Data->DirLights.size() < 10) {
			s_Data->DirLights.push_back(light);
		}
		else {
			TE_CORE_WARN("Max directional lights reached! Cannot add more.");
		}
	}

	DirLight& Renderer3D::GetDirLight(const int& index) {
		if (index >= 0 && index < s_Data->DirLights.size()) {
			return s_Data->DirLights[index];
		}
		else {
			TE_CORE_WARN("Invalid index for directional light retrieval.");
			throw std::out_of_range("Invalid index for directional light retrieval.");
		}
	}

	int Renderer3D::GetDirLightCount() {
		return s_Data->DirLights.size();
	}

	void Renderer3D::RemoveDirLight(const int& index) {
		if (index >= 0 && index < s_Data->DirLights.size()) {
			s_Data->DirLights.erase(s_Data->DirLights.begin() + index);
		}
		else {
			TE_CORE_WARN("Invalid index for directional light removal.");
		}
	}

	void Renderer3D::AddSpotLight(const SpotLight& light) {
		if (s_Data->SpotLights.size() < 10) {
			s_Data->SpotLights.push_back(light);
		}
		else {
			TE_CORE_WARN("Max spotlights reached! Cannot add more.");
		}
	}

	SpotLight& Renderer3D::GetSpotLight(const int& index) {
		if (index >= 0 && index < s_Data->SpotLights.size()) {
			return s_Data->SpotLights[index];
		}
		else {
			TE_CORE_WARN("Invalid index for spotlight retrieval.");
			throw std::out_of_range("Invalid index for spotlight retrieval.");
		}
	}

	int Renderer3D::GetSpotLightCount() {
		return s_Data->SpotLights.size();
	}

	void Renderer3D::RemoveSpotLight(const int& index) {
		if (index >= 0 && index < s_Data->SpotLights.size()) {
			s_Data->SpotLights.erase(s_Data->SpotLights.begin() + index);
		}
		else {
			TE_CORE_WARN("Invalid index for spotlight removal.");
		}
	}

	void Renderer3D::AddPointLight(const PointLight& light) {
		if (s_Data->PointLights.size() < 10) {
			s_Data->PointLights.push_back(light);
		}
		else {
			TE_CORE_WARN("Max point lights reached! Cannot add more.");
		}
	}

	PointLight& Renderer3D::GetPointLight(const int& index) {
		if (index >= 0 && index < s_Data->PointLights.size()) {
			return s_Data->PointLights[index];
		}
		else {
			TE_CORE_WARN("Invalid index for point light retrieval.");
			throw std::out_of_range("Invalid index for point light retrieval.");
		}
	}

	int Renderer3D::GetPointLightCount() {
		return s_Data->PointLights.size();
	}

	void Renderer3D::RemovePointLight(const int& index) {
		if (index >= 0 && index < s_Data->PointLights.size()) {
			s_Data->PointLights.erase(s_Data->PointLights.begin() + index);
		}
		else {
			TE_CORE_WARN("Invalid index for point light removal.");
		}
	}

	void Renderer3D::ClearDirLights() {
		s_Data->DirLights.clear();
	}

	void Renderer3D::ClearSpotLights() {
		s_Data->SpotLights.clear();
	}

	void Renderer3D::ClearPointLights() {
		s_Data->PointLights.clear();
	}

	void Renderer3D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer3D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		s_Data->TextureShader->SetFloat4("u_Color", color);
		s_Data->WhiteTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->TextureShader->SetMat4("u_Transform", transform);

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer3D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture);
	}

	void Renderer3D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		s_Data->TextureShader->SetFloat4("u_Color", glm::vec4(1.0f));
		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->TextureShader->SetMat4("u_Transform", transform);

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer3D::DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color)
	{
		s_Data->TextureShader->SetFloat4("u_Color", color);
		s_Data->WhiteTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), size);
		s_Data->TextureShader->SetMat4("u_Transform", transform);

		s_Data->CubeVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->CubeVertexArray);
	}

	void Renderer3D::DrawLightCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color, const float& shininess)
	{
		s_Data->WhiteTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), size);
		s_Data->TextureLightShader->SetMat4("u_Transform", transform);
		s_Data->TextureLightShader->SetFloat3("u_CameraPosition", s_Data->CameraPosition);
		s_Data->TextureLightShader->SetFloat4("u_Color", color);

		s_Data->TextureLightShader->SetFloat("material.shininess", shininess);

		s_Data->CubeVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->CubeVertexArray);
	}

	void Renderer3D::DrawLightCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color, const float& shininess, const Ref<Texture2D>& texture)
	{
		s_Data->WhiteTexture->Bind(1);
		texture->Bind(0);
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), size);
		s_Data->TextureLightShader->SetMat4("u_Transform", transform);
		s_Data->TextureLightShader->SetFloat3("u_CameraPosition", s_Data->CameraPosition);
		s_Data->TextureLightShader->SetFloat4("u_Color", color);

		s_Data->TextureLightShader->SetFloat("material.shininess", shininess);

		s_Data->CubeVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->CubeVertexArray);
	}

	void Renderer3D::DrawLightCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color, const float& shininess, const Ref<Texture2D>& diffuse, const Ref<Texture2D>& specular)
	{
		diffuse->Bind(0);
		specular->Bind(1);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), size);
		s_Data->TextureLightShader->SetMat4("u_Transform", transform);
		s_Data->TextureLightShader->SetFloat3("u_CameraPosition", s_Data->CameraPosition);
		s_Data->TextureLightShader->SetFloat4("u_Color", color);

		s_Data->TextureLightShader->SetFloat("material.shininess", shininess);

		s_Data->CubeVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->CubeVertexArray);
	}

	void Renderer3D::DrawLightSource(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color)
	{
		s_Data->TextureShader->SetFloat4("u_Color", color);
		s_Data->WhiteTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), size);
		s_Data->TextureShader->SetMat4("u_Transform", transform);

		s_Data->CubeVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->CubeVertexArray);
	}

	void Renderer3D::DrawSkybox(const Ref<TextureCube>& cubemap)
	{
		cubemap->Bind();
		s_Data->SkyboxVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->SkyboxVertexArray);
	}

    void Renderer3D::DrawMesh
    (
        const Mesh& mesh,
        glm::mat4 matrix,
        glm::vec3 translation,
        glm::quat rotation,
        glm::vec3 scale
    )
    {
        mesh.m_VertexArray->Bind();

        // Initialize matrices
        glm::mat4 trans = glm::mat4(1.0f);
        glm::mat4 rot = glm::mat4(1.0f);
        glm::mat4 sca = glm::mat4(1.0f);

        // Transform the matrices to their correct form
        trans = glm::translate(trans, translation);
        rot = glm::mat4_cast(rotation);

        sca = glm::scale(sca, scale);

        s_Data->DefaultShader->SetMat4("u_Model", matrix);
        s_Data->DefaultShader->SetMat4("u_Translation", trans);
        s_Data->DefaultShader->SetMat4("u_Rotation", rot);
        s_Data->DefaultShader->SetMat4("u_Scale", sca);

        s_Data->DefaultShader->SetFloat("material.shininess", 2.0f);

        // Keep track of how many of each type of textures we have
        unsigned int numDiffuse = 0;
        unsigned int numSpecular = 0;

        for (unsigned int i = 0; i < mesh.m_Textures.size(); i++)
        {
            std::string num;
            std::string type = mesh.m_Textures[i]->GetType();
            if (type == "diffuse")
            {
                num = std::to_string(numDiffuse++);
            }
            else if (type == "specular")
            {
                num = std::to_string(numSpecular++);
            }

            s_Data->DefaultShader->SetInt(("material." + type + num).c_str(), i);
            mesh.m_Textures[i]->Bind(i);
        }

        RenderCommand::DrawIndexed(mesh.m_VertexArray);
    }

	void Renderer3D::DrawModel
	(
		const Ref<Model>& model,
		glm::vec3 position,
		glm::quat rotation,
		glm::vec3 scale
	)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), scale) * glm::mat4_cast(rotation);

		for (unsigned int i = 0; i < model->meshes.size(); i++)
		{
			DrawMesh(model->meshes[i], transform * model->matricesMeshes[i]);
		}
	}


}