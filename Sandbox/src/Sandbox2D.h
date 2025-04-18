#pragma once

#include "TheEngine.h"

class Sandbox2D : public TheEngine::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(TheEngine::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(TheEngine::Event& e) override;
private:
	TheEngine::Camera2DController m_Camera2DController;
	TheEngine::Camera3DController m_Camera3DController;
	TheEngine::CameraController* m_ActiveCameraControllerPointer;
	TheEngine::Camera m_ActiveCamera;

	// Temp
	TheEngine::Ref<TheEngine::VertexArray> m_SquareVA;
	TheEngine::Ref<TheEngine::Shader> m_FlatColorShader;


	TheEngine::Ref<TheEngine::Texture2D> m_CheckerboardTexture;
	TheEngine::Ref<TheEngine::Texture2D> m_BoxDiffuse;
	TheEngine::Ref<TheEngine::Texture2D> m_BoxSpecular;
	TheEngine::Ref<TheEngine::TextureCube> m_Skybox;

	std::vector<TheEngine::Ref<TheEngine::Model>> m_Models;

	glm::vec4 m_SquareColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	glm::vec4 m_LightColor = { 1.0f, 1.0f, 1.0f, 1.0f };

	float m_Time = 0.0f;
	int m_shininess = 32;
};