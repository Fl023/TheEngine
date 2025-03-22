#include "TheEngine.h"


class ExampleLayer : public TheEngine::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f)
	{
		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};
		std::shared_ptr<TheEngine::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(TheEngine::VertexBuffer::Create(vertices, sizeof(vertices)));
		vertexBuffer->SetLayout({
			{ TheEngine::ShaderDataType::Float3, "aPos" },
			{ TheEngine::ShaderDataType::Float4, "aCol" }
			});
		m_VertexArray.reset(TheEngine::VertexArray::Create());
		m_VertexArray->AddVertexBuffer(vertexBuffer);
		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<TheEngine::IndexBuffer> indexBuffer;
		indexBuffer.reset(TheEngine::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);
		std::string vertexSrc = R"(
			#version 330 core
			layout(location = 0) in vec3 aPos;
			layout(location = 1) in vec4 aCol;
			out vec4 vertexColor;
			uniform mat4 u_ViewProjection;
			void main()
			{
				vertexColor = aCol;
				gl_Position = u_ViewProjection * vec4(aPos, 1.0);
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
		m_Shader.reset(new TheEngine::Shader(vertexSrc, fragmentSrc));
		float squareVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			 -0.75f,  0.75f, 0.0f
		};
		std::shared_ptr<TheEngine::VertexBuffer> squareVB;
		squareVB.reset(TheEngine::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ TheEngine::ShaderDataType::Float3, "aPos" }
			});
		m_SquareVA.reset(TheEngine::VertexArray::Create());
		m_SquareVA->AddVertexBuffer(squareVB);
		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<TheEngine::IndexBuffer> squareIB;
		squareIB.reset(TheEngine::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);
		std::string flatColorShaderVertexSrc = R"(
			#version 330 core
			layout(location = 0) in vec3 aPos;
			uniform mat4 u_ViewProjection;
			void main()
			{
				gl_Position = u_ViewProjection * vec4(aPos, 1.0);
			}
		)";
		std::string flatColorShaderFragmentSrc = R"(
			#version 330 core
			out vec4 FragColor;
			//uniform vec3 u_Color;
			void main()
			{
				FragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);
			}
		)";
		m_ShaderSquare.reset(new TheEngine::Shader(flatColorShaderVertexSrc, flatColorShaderFragmentSrc));

	}

	void OnUpdate() override
	{
		if (TheEngine::Input::IsKeyPressed(TE_KEY_LEFT))
		{
			m_CameraPosition.x -= m_CameraTranslationSpeed;
		}
		if (TheEngine::Input::IsKeyPressed(TE_KEY_RIGHT))
		{
			m_CameraPosition.x += m_CameraTranslationSpeed;
		}
		if (TheEngine::Input::IsKeyPressed(TE_KEY_UP))
		{
			m_CameraPosition.y += m_CameraTranslationSpeed;
		}
		if (TheEngine::Input::IsKeyPressed(TE_KEY_DOWN))
		{
			m_CameraPosition.y -= m_CameraTranslationSpeed;
		}
		if (TheEngine::Input::IsKeyPressed(TE_KEY_A))
		{
			m_CameraRotation += m_CameraRotationSpeed;
		}
		if (TheEngine::Input::IsKeyPressed(TE_KEY_D))
		{
			m_CameraRotation -= m_CameraRotationSpeed;
		}


		TheEngine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		TheEngine::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		TheEngine::Renderer::BeginScene(m_Camera);

		TheEngine::Renderer::Submit(m_ShaderSquare, m_SquareVA);
		TheEngine::Renderer::Submit(m_Shader, m_VertexArray);

		TheEngine::Renderer::EndScene();
	}

	void OnImGuiRender() override
	{

	}

	void OnEvent(TheEngine::Event& event) override
	{
	}

private:
	//TheEngine::ShaderLibrary m_ShaderLibrary;
	std::shared_ptr<TheEngine::Shader> m_Shader;
	std::shared_ptr<TheEngine::VertexArray> m_VertexArray;

	std::shared_ptr<TheEngine::Shader> m_ShaderSquare;
	std::shared_ptr<TheEngine::VertexArray> m_SquareVA;

	TheEngine::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraRotation = 0.0f;
	float m_CameraTranslationSpeed = 0.1f, m_CameraRotationSpeed = 1.80f;

};

class Sandbox : public TheEngine::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}
	~Sandbox()
	{
	}
};

TheEngine::Application* TheEngine::CreateApplication()
{
	return new Sandbox();
}
