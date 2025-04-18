#include "Sandbox2D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_Camera2DController(1280.0f / 720.0f), m_Camera3DController({ 0.0f,0.0f,6.0f }, 45, 1280.0f / 720.0f, 0.1f, 100.0f)
{
	m_ActiveCameraControllerPointer = &m_Camera3DController;
}

void Sandbox2D::OnAttach()
{
	m_CheckerboardTexture = TheEngine::Texture2D::Create("assets/textures/Checkerboard.png");
	m_BoxDiffuse = TheEngine::Texture2D::Create("assets/textures/BoxDiffuse.png");
	m_BoxSpecular = TheEngine::Texture2D::Create("assets/textures/BoxSpecular.png");
	m_Skybox = TheEngine::TextureCube::Create({
		"assets/textures/skybox/right.jpg",
		"assets/textures/skybox/left.jpg",
		"assets/textures/skybox/top.jpg",
		"assets/textures/skybox/bottom.jpg",
		"assets/textures/skybox/front.jpg",
		"assets/textures/skybox/back.jpg"
		});

	m_Models.push_back(TheEngine::Model::Create("assets/models/map/scene.gltf"));
    m_Models.push_back(TheEngine::Model::Create("assets/models/bunny/scene.gltf"));
    m_Models.push_back(TheEngine::Model::Create("assets/models/grindstone/scene.gltf"));
	m_Models.push_back(TheEngine::Model::Create("assets/models/scroll/scene.gltf"));
    m_Models.push_back(TheEngine::Model::Create("assets/models/sword/scene.gltf"));

	// Set directional light
	TheEngine::Renderer3D::AddDirLight({
		glm::vec3(-0.2f, -1.0f, -0.3f), // Direction
        glm::vec3(0.05f),				// Ambient
		glm::vec3(0.4f),                // Diffuse
		glm::vec3(0.5f)                 // Specular
		});

	// Set spotlight
	TheEngine::Renderer3D::AddSpotLight({
        glm::vec3(0.0f, 0.0f, 3.0f),    // Position
        glm::vec3(0.0f, 0.0f, -1.0f),   // Direction
		glm::cos(glm::radians(12.5f)),  // CutOff
		glm::cos(glm::radians(15.0f)),  // OuterCutOff
		1.0f, 0.09f, 0.032f,            // Attenuation (constant, linear, quadratic)
		glm::vec3(0.0f),                // Ambient
		glm::vec3(1.0f),                // Diffuse
		glm::vec3(1.0f)                 // Specular
		});

	// Add point lights
	TheEngine::Renderer3D::AddPointLight({
		glm::vec3(1.0f, 0.5f, 2.0f),    // Position
		glm::vec3(0.05f),               // Ambient
		glm::vec3(0.8f),                // Diffuse
		glm::vec3(1.0f),                // Specular
		1.0f, 0.09f, 0.032f             // Attenuation (constant, linear, quadratic)
		});
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(TheEngine::Timestep ts)
{
	// Update
	m_ActiveCameraControllerPointer->OnUpdate(ts);

	// Render
	TheEngine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	TheEngine::RenderCommand::Clear();

    TheEngine::Renderer3D::BeginScene(m_ActiveCameraControllerPointer->GetCamera());

    // Update and render directional lights
    for (int i = 0; i < TheEngine::Renderer3D::GetDirLightCount(); i++)
    {
        auto& light = TheEngine::Renderer3D::GetDirLight(i);
        glm::vec3 lightColor = light.diffuse; // Example light color
        TheEngine::Renderer3D::DrawLightSource(-10.0f * light.direction, { 0.5f, 0.5f, 0.5f }, glm::vec4(lightColor, 1.0f));
    }

    // Update and render point lights
    for (int i = 0; i < TheEngine::Renderer3D::GetPointLightCount(); i++)
    {
        auto& light = TheEngine::Renderer3D::GetPointLight(i);
        glm::vec3 lightColor = light.diffuse; // Example light color
        TheEngine::Renderer3D::DrawLightSource(light.position, { 0.5f, 0.5f, 0.5f }, glm::vec4(lightColor, 1.0f));
    }

    // Update and render spotlights
    for (int i = 0; i < TheEngine::Renderer3D::GetSpotLightCount(); i++)
    {
        auto& light = TheEngine::Renderer3D::GetSpotLight(i);
        glm::vec3 lightColor = light.diffuse; // Example light color
        TheEngine::Renderer3D::DrawLightSource(light.position, { 0.5f, 0.5f, 0.5f }, glm::vec4(lightColor, 1.0f));
    }

    TheEngine::Renderer3D::EndScene();


	//TheEngine::Renderer3D::BeginScene(m_ActiveCameraControllerPointer->GetCamera());
	//m_Time += ts;
	//glm::vec3 cubePos = { 1.0f * glm::sin(m_Time), 1.0f * glm::cos(m_Time), 1.0f };
	//TheEngine::Renderer3D::GetPointLight(0).position = cubePos;
	//TheEngine::Renderer3D::GetPointLight(0).ambient = 0.05f * glm::vec3(m_LightColor);
	//TheEngine::Renderer3D::GetPointLight(0).diffuse = 0.8f * glm::vec3(m_LightColor);
	//TheEngine::Renderer3D::GetPointLight(0).specular = 1.0f * glm::vec3(m_LightColor);
	//TheEngine::Renderer3D::EndScene();

	//TheEngine::Renderer3D::BeginSceneLight(m_ActiveCameraControllerPointer->GetCamera());
	////TheEngine::Renderer3D::DrawLightCube({ 0.0f, 0.0f, 0.0f }, { 4.5f , 4.5f, 0.3f }, m_SquareColor, m_shininess, m_BoxDiffuse, m_BoxSpecular);
 //   TheEngine::Renderer3D::DrawLightCube({ 0.0f, 0.0f, 0.0f }, { 4.5f , 4.5f, 0.3f }, m_SquareColor, m_shininess, m_BoxDiffuse, m_BoxSpecular);
	//TheEngine::Renderer3D::EndScene();

	TheEngine::Renderer3D::BeginSceneDefault(m_ActiveCameraControllerPointer->GetCamera());
	//for (int i = 0; i<10; i++)
	//{
 //       for (int j = 0; j < 10; j++)
 //       TheEngine::Renderer3D::DrawModel(m_Models[1], glm::vec3(i*1.0f, 0.0f, j*1.0f), glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3(4.0f));
	//}
    TheEngine::Renderer3D::DrawModel(m_Models[0], glm::vec3(0.0f, -5.0f, 0.0f), glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3(4.0f));
    TheEngine::Renderer3D::EndScene();


	TheEngine::Renderer3D::BeginSceneSkybox(m_ActiveCameraControllerPointer->GetCamera());
	TheEngine::Renderer3D::DrawSkybox(m_Skybox);
	TheEngine::Renderer3D::EndSceneSkybox();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");

	// Toggle to choose between 3D and 2D camera controllers.
	static bool use3DCamera = true; 
	if (ImGui::Checkbox("Use 3D Camera", &use3DCamera))
	{
		if (use3DCamera)
			m_ActiveCameraControllerPointer = &m_Camera3DController;
		else
			m_ActiveCameraControllerPointer = &m_Camera2DController;
	}

	if (use3DCamera)
		ImGui::Text("3D Camera Active");
	else
		ImGui::Text("2D Camera Active");


	ImGui::ColorEdit4("Cube Color", glm::value_ptr(m_SquareColor));
	ImGui::ColorEdit4("Light Color", glm::value_ptr(m_LightColor));
	ImGui::DragInt("Shininess", &m_shininess, 1.0f, 2, 256);


    // Directional Lights
    if (ImGui::CollapsingHeader("Directional Lights"))
    {
        static glm::vec3 dirDirection = glm::vec3(-0.2f, -1.0f, -0.3f);
        static glm::vec3 dirAmbient = glm::vec3(0.05f);
        static glm::vec3 dirDiffuse = glm::vec3(0.4f);
        static glm::vec3 dirSpecular = glm::vec3(0.5f);

        if (ImGui::Button("Add Directional Light"))
        {
            TheEngine::Renderer3D::AddDirLight({ dirDirection, dirAmbient, dirDiffuse, dirSpecular });
        }

        for (int i = 0; i < TheEngine::Renderer3D::GetDirLightCount(); i++)
        {
            ImGui::PushID(i);
            ImGui::Text("Directional Light %d", i);
            auto& light = TheEngine::Renderer3D::GetDirLight(i);
            ImGui::DragFloat3("Direction", glm::value_ptr(light.direction), 0.1f);
            ImGui::ColorEdit3("Ambient", glm::value_ptr(light.ambient));
            ImGui::ColorEdit3("Diffuse", glm::value_ptr(light.diffuse));
            ImGui::ColorEdit3("Specular", glm::value_ptr(light.specular));
            if (ImGui::Button("Remove"))
            {
                TheEngine::Renderer3D::RemoveDirLight(i);
            }
            ImGui::PopID();
        }
    }

    // Spotlights
    if (ImGui::CollapsingHeader("Spotlights"))
    {
        static glm::vec3 spotPosition = glm::vec3(0.0f, 0.0f, 3.0f);
        static glm::vec3 spotDirection = glm::vec3(0.0f, 0.0f, -1.0f);
        static float spotCutOff = glm::cos(glm::radians(12.5f));
        static float spotOuterCutOff = glm::cos(glm::radians(15.0f));
        static glm::vec3 spotAmbient = glm::vec3(0.0f);
        static glm::vec3 spotDiffuse = glm::vec3(1.0f);
        static glm::vec3 spotSpecular = glm::vec3(1.0f);
        static float spotConstant = 1.0f;
        static float spotLinear = 0.09f;
        static float spotQuadratic = 0.032f;

        if (ImGui::Button("Add Spotlight"))
        {
            TheEngine::Renderer3D::AddSpotLight({ spotPosition, spotDirection, spotCutOff, spotOuterCutOff, spotConstant, spotLinear, spotQuadratic, spotAmbient, spotDiffuse, spotSpecular });
        }

        for (int i = 0; i < TheEngine::Renderer3D::GetSpotLightCount(); i++)
        {
            ImGui::PushID(i);
            ImGui::Text("Spotlight %d", i);
            auto& light = TheEngine::Renderer3D::GetSpotLight(i);
            ImGui::DragFloat3("Position", glm::value_ptr(light.position), 0.1f);
            ImGui::DragFloat3("Direction", glm::value_ptr(light.direction), 0.1f);
            ImGui::DragFloat("CutOff", &light.cutOff, 0.01f, 0.0f, 1.0f);
            ImGui::DragFloat("OuterCutOff", &light.outerCutOff, 0.01f, 0.0f, 1.0f);
            ImGui::ColorEdit3("Ambient", glm::value_ptr(light.ambient));
            ImGui::ColorEdit3("Diffuse", glm::value_ptr(light.diffuse));
            ImGui::ColorEdit3("Specular", glm::value_ptr(light.specular));
            ImGui::DragFloat("Constant", &light.constant, 0.01f);
            ImGui::DragFloat("Linear", &light.linear, 0.01f);
            ImGui::DragFloat("Quadratic", &light.quadratic, 0.01f);
            if (ImGui::Button("Remove"))
            {
                TheEngine::Renderer3D::RemoveSpotLight(i);
            }
            ImGui::PopID();
        }
    }

    // Point Lights
    if (ImGui::CollapsingHeader("Point Lights"))
    {
        static glm::vec3 pointPosition = glm::vec3(1.0f, 0.5f, 2.0f);
        static glm::vec3 pointAmbient = glm::vec3(0.05f);
        static glm::vec3 pointDiffuse = glm::vec3(0.8f);
        static glm::vec3 pointSpecular = glm::vec3(1.0f);
        static float pointConstant = 1.0f;
        static float pointLinear = 0.09f;
        static float pointQuadratic = 0.032f;

        if (ImGui::Button("Add Point Light"))
        {
            TheEngine::Renderer3D::AddPointLight({ pointPosition, pointAmbient, pointDiffuse, pointSpecular, pointConstant, pointLinear, pointQuadratic });
        }

        for (int i = 0; i < TheEngine::Renderer3D::GetPointLightCount(); i++)
        {
            ImGui::PushID(i);
            ImGui::Text("Point Light %d", i);
            auto& light = TheEngine::Renderer3D::GetPointLight(i);
            ImGui::DragFloat3("Position", glm::value_ptr(light.position), 0.1f);
            ImGui::ColorEdit3("Ambient", glm::value_ptr(light.ambient));
            ImGui::ColorEdit3("Diffuse", glm::value_ptr(light.diffuse));
            ImGui::ColorEdit3("Specular", glm::value_ptr(light.specular));
            ImGui::DragFloat("Constant", &light.constant, 0.01f);
            ImGui::DragFloat("Linear", &light.linear, 0.01f);
            ImGui::DragFloat("Quadratic", &light.quadratic, 0.01f);
            if (ImGui::Button("Remove"))
            {
                TheEngine::Renderer3D::RemovePointLight(i);
            }
            ImGui::PopID();
        }
    }


	ImGui::End();
}

void Sandbox2D::OnEvent(TheEngine::Event& e)
{
	m_ActiveCameraControllerPointer->OnEvent(e);
}