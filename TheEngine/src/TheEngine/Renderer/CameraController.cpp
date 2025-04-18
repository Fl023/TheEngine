#include "tepch.h"
#include "CameraController.h"

#include "TheEngine/Core/Input.h"
#include "TheEngine/Core/KeyCodes.h"

namespace TheEngine {

    Camera2DController::Camera2DController(float aspectRatio, bool rotation)
        : m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_Rotation(rotation)
    {
    }

    void Camera2DController::OnUpdate(Timestep ts)
    {
        if (Input::IsKeyPressed(TE_KEY_A))
            m_CameraPosition.x -= m_CameraTranslationSpeed * ts;
        else if (Input::IsKeyPressed(TE_KEY_D))
            m_CameraPosition.x += m_CameraTranslationSpeed * ts;

        if (Input::IsKeyPressed(TE_KEY_W))
            m_CameraPosition.y += m_CameraTranslationSpeed * ts;
        else if (Input::IsKeyPressed(TE_KEY_S))
            m_CameraPosition.y -= m_CameraTranslationSpeed * ts;

        if (m_Rotation)
        {
            if (Input::IsKeyPressed(TE_KEY_Q))
                m_CameraRotation += m_CameraRotationSpeed * ts;
            if (Input::IsKeyPressed(TE_KEY_E))
                m_CameraRotation -= m_CameraRotationSpeed * ts;

            m_Camera.SetRotation(m_CameraRotation);
        }

        m_Camera.SetPosition(m_CameraPosition);

        m_CameraTranslationSpeed = m_ZoomLevel;
    }

    void Camera2DController::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseScrolledEvent>(TE_BIND_EVENT_FN(Camera2DController::OnMouseScrolled));
        dispatcher.Dispatch<WindowResizeEvent>(TE_BIND_EVENT_FN(Camera2DController::OnWindowResized));
    }

    bool Camera2DController::OnMouseScrolled(MouseScrolledEvent& e)
    {
        m_ZoomLevel -= e.GetYOffset() * 0.25f;
        m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
        m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
        return false;
    }

    bool Camera2DController::OnWindowResized(WindowResizeEvent& e)
    {
        m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
        m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
        return false;
    }




    // Camera 3D

    Camera3DController::Camera3DController(const glm::vec3& position, float fov, float aspectRatio, float nearClip, float farClip)
		: m_Fov(fov), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip), m_Position(position),
          m_Camera(position, fov, aspectRatio, nearClip, farClip)
    {
    }

    void Camera3DController::OnUpdate(Timestep ts)
    {

        glm::mat4 rotation = glm::yawPitchRoll(glm::radians(m_Yaw), glm::radians(m_Pitch), glm::radians(0.0f));

        glm::vec3 front = glm::normalize(glm::vec3(rotation * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)));
        glm::vec3 up = glm::normalize(glm::vec3(rotation * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f)));
        glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));

        if (Input::IsKeyPressed(TE_KEY_W))
            m_Position += front * m_TranslationSpeed * (float)ts;
        if (Input::IsKeyPressed(TE_KEY_S))
            m_Position -= front * m_TranslationSpeed * (float)ts;
        if (Input::IsKeyPressed(TE_KEY_A))
            m_Position -= right * m_TranslationSpeed * (float)ts;
        if (Input::IsKeyPressed(TE_KEY_D))
            m_Position += right * m_TranslationSpeed * (float)ts;
        if (Input::IsKeyPressed(TE_KEY_Q))
            m_Position += up * m_TranslationSpeed * (float)ts;
        if (Input::IsKeyPressed(TE_KEY_E))
            m_Position -= up * m_TranslationSpeed * (float)ts;

        if (Input::IsKeyPressed(TE_KEY_LEFT))
            m_Yaw -= m_RotationSpeed * ts;
        if (Input::IsKeyPressed(TE_KEY_RIGHT))
            m_Yaw += m_RotationSpeed * ts;
        if (Input::IsKeyPressed(TE_KEY_UP))
            m_Pitch += m_RotationSpeed * ts;
        if (Input::IsKeyPressed(TE_KEY_DOWN))
            m_Pitch -= m_RotationSpeed * ts;
        if (Input::IsKeyPressed(TE_KEY_P))
            m_Roll += m_RotationSpeed * ts;
        if (Input::IsKeyPressed(TE_KEY_O))
            m_Roll -= m_RotationSpeed * ts;

        if (m_Pitch > 89.0f)
            m_Pitch = 89.0f;
        if (m_Pitch < -89.0f)
            m_Pitch = -89.0f;

        m_Camera.SetPosition(m_Position);
        m_Camera.SetRotation(m_Yaw, m_Pitch, m_Roll);
    }

    void Camera3DController::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseMovedEvent>(TE_BIND_EVENT_FN(Camera3DController::OnMouseMoved));
        dispatcher.Dispatch<MouseScrolledEvent>(TE_BIND_EVENT_FN(Camera3DController::OnMouseScrolled));
    }

    bool Camera3DController::OnMouseMoved(MouseMovedEvent& e)
    {
        float xoffset = m_LastMouseX - e.GetX();
        float yoffset = m_LastMouseY - e.GetY();
        m_LastMouseX = e.GetX();
        m_LastMouseY = e.GetY();

        float sensitivity = 0.1f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        m_Yaw += xoffset;
        m_Pitch += yoffset;

        if (m_Pitch > 89.0f)
            m_Pitch = 89.0f;
        if (m_Pitch < -89.0f)
            m_Pitch = -89.0f;

        m_Camera.SetRotation(m_Yaw, m_Pitch);
        return false;
    }

    bool Camera3DController::OnMouseScrolled(MouseScrolledEvent& e)
    {
        m_Fov -= e.GetYOffset() * 2.0f * m_Fov / 45.0f;
        if (m_Fov < 1.0f)
            m_Fov = 1.0f;
        if (m_Fov > 135.0f)
            m_Fov = 135.0f;

        m_Camera.SetProjection(m_Fov, m_AspectRatio, m_NearClip, m_FarClip);
        return false;
    }
}