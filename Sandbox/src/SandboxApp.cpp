#include <TheEngine.h>
#include <TheEngine/Core/EntryPoint.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Sandbox2D.h"


class ExampleLayer : public TheEngine::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_CameraController(1280.0f / 720.0f, true)
	{
		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};
		TheEngine::Ref<TheEngine::VertexBuffer> vertexBuffer = TheEngine::VertexBuffer::Create(vertices, sizeof(vertices));
		vertexBuffer->SetLayout({
			{ TheEngine::ShaderDataType::Float3, "aPos" },
			{ TheEngine::ShaderDataType::Float4, "aCol" }
			});
		m_VertexArray = TheEngine::VertexArray::Create();
		m_VertexArray->AddVertexBuffer(vertexBuffer);
		uint32_t indices[3] = { 0, 1, 2 };
		TheEngine::Ref<TheEngine::IndexBuffer> indexBuffer = TheEngine::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		m_VertexArray->SetIndexBuffer(indexBuffer);
		std::string vertexSrc = R"(
			#version 330 core
			layout(location = 0) in vec3 aPos;
			layout(location = 1) in vec4 aCol;
			out vec4 vertexColor;
			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;
			void main()
			{
				vertexColor = aCol;
				gl_Position = u_ViewProjection * u_Transform * vec4(aPos, 1.0);
			}
		)";
		std::string fragmentSrc = R"(
			#version 330 core
			out vec4 FragColor;
			in vec4 vertexColor;
			void main()
			{
				FragColor = vertexColor;
			}
		)";

		m_Shader = TheEngine::Shader::Create("VertexPosColor", vertexSrc, fragmentSrc);

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			 -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};
		TheEngine::Ref<TheEngine::VertexBuffer> squareVB = TheEngine::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		squareVB->SetLayout({
			{ TheEngine::ShaderDataType::Float3, "a_Position" },
			{ TheEngine::ShaderDataType::Float2, "a_TexCoord" }
			});


		m_SquareVA = TheEngine::VertexArray::Create();
		m_SquareVA->AddVertexBuffer(squareVB);
		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		TheEngine::Ref<TheEngine::IndexBuffer> squareIB = TheEngine::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		m_SquareVA->SetIndexBuffer(squareIB);
		std::string flatColorShaderVertexSrc = R"(
			#version 330 core
			layout(location = 0) in vec3 a_Position;
			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;
			void main()
			{
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";
		std::string flatColorShaderFragmentSrc = R"(
			#version 330 core
			out vec4 FragColor;
			uniform vec3 u_Color;
			void main()
			{
				FragColor = vec4(u_Color, 1.0f);
			}
		)";
		m_ShaderSquare = TheEngine::Shader::Create("flatColor", flatColorShaderVertexSrc, flatColorShaderFragmentSrc);

		auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

		m_Texture = TheEngine::Texture2D::Create("assets/textures/Checkerboard.png");
		m_TransparentTexture = TheEngine::Texture2D::Create("assets/textures/PNG_transparency_demonstration_1.png");

		std::dynamic_pointer_cast<TheEngine::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<TheEngine::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(TheEngine::Timestep ts) override
	{
		TE_TRACE("fps: {0} ({1}ms)", 1 / ts.GetSeconds(), ts.GetMilliseconds());

		m_CameraController.OnUpdate(ts);

		TheEngine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		TheEngine::RenderCommand::Clear();

		TheEngine::Renderer::BeginScene(m_CameraController.GetCamera());
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<TheEngine::OpenGLShader>(m_ShaderSquare)->Bind();
		std::dynamic_pointer_cast<TheEngine::OpenGLShader>(m_ShaderSquare)->UploadUniformFloat3("u_Color", m_SquareColor);

		for (int y = 0; y < 20; y++) {
			for (int x = 0; x < 20; x++) {
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				TheEngine::Renderer::Submit(m_ShaderSquare, m_SquareVA, transform);
			}
		}

		auto textureShader = m_ShaderLibrary.Get("Texture");

		m_Texture->Bind();
		TheEngine::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		m_TransparentTexture->Bind();
		TheEngine::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		// Triangle
		// TheEngine::Renderer::Submit(m_Shader, m_VertexArray);

		TheEngine::Renderer::EndScene();
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(TheEngine::Event& e) override
	{
		m_CameraController.OnEvent(e);
	}

private:
	TheEngine::ShaderLibrary m_ShaderLibrary;
	TheEngine::Ref<TheEngine::Shader> m_Shader;
	TheEngine::Ref<TheEngine::VertexArray> m_VertexArray;

	TheEngine::Ref<TheEngine::Shader> m_ShaderSquare;
	TheEngine::Ref<TheEngine::VertexArray> m_SquareVA;

	TheEngine::Ref<TheEngine::Texture2D> m_Texture, m_TransparentTexture;

	TheEngine::Camera2DController m_CameraController;

	glm::vec3 m_SquareColor = {0.2f, 0.3f, 0.8f};
};

class Sandbox : public TheEngine::Application
{
public:
	Sandbox()
	{
		// PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
	}
	~Sandbox()
	{
	}
};

TheEngine::Application* TheEngine::CreateApplication()
{
	return new Sandbox();
}
