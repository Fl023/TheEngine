#pragma once

#include "TheEngine/Renderer/Camera.h"
#include "TheEngine/Core/Timestep.h"

#include "TheEngine/Events/ApplicationEvent.h"
#include "TheEngine/Events/MouseEvent.h"

namespace TheEngine {

    class CameraController
    {
    public:
        virtual ~CameraController() = default;

        virtual void OnUpdate(Timestep ts) = 0;
        virtual void OnEvent(Event& e) = 0;

        virtual Camera& GetCamera() = 0;
        virtual const Camera& GetCamera() const = 0;
    };


    class Camera2DController : public CameraController
    {
    public:
        Camera2DController(float aspectRatio, bool rotation = false);

        virtual void OnUpdate(Timestep ts) override;
        virtual void OnEvent(Event& e) override;

        virtual Camera& GetCamera() override { return m_Camera; }
        virtual const Camera& GetCamera() const override { return m_Camera; }

        float GetZoomLevel() const { return m_ZoomLevel; }
        void SetZoomLevel(float level) { m_ZoomLevel = level; }
    private:
        bool OnMouseScrolled(MouseScrolledEvent& e);
        bool OnWindowResized(WindowResizeEvent& e);
    private:
        float m_AspectRatio;
        float m_ZoomLevel = 1.0f;
        Camera2D m_Camera; // Camera2D must be derived from Camera

        bool m_Rotation;

        glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
        float m_CameraRotation = 0.0f;
        float m_CameraTranslationSpeed = 5.0f, m_CameraRotationSpeed = 180.0f;
    };

    class Camera3DController : public CameraController
    {
    public:
        // Constructor accepts the initial camera parameters.
        Camera3DController(const glm::vec3& position, float fov, float aspectRatio, float nearClip, float farClip);

        virtual void OnUpdate(Timestep ts) override;
        virtual void OnEvent(Event& e) override;

        virtual Camera& GetCamera() override { return m_Camera; }
        virtual const Camera& GetCamera() const override { return m_Camera; }

    private:
        bool OnMouseMoved(MouseMovedEvent& e);
        bool OnMouseScrolled(MouseScrolledEvent& e);
    private:
        Camera3D m_Camera; // Camera3D must be derived from Camera
        float m_Fov, m_AspectRatio, m_NearClip, m_FarClip;

        glm::vec3 m_Position = { 0.0f, 0.0f, 3.0f };
        float m_Yaw = 0.0f;   // Typically initialized to point along -Z
        float m_Pitch = 0.0f;
        float m_Roll = 0.0f;
        float m_Zoom = 45.0f;

        float m_TranslationSpeed = 5.0f;
        float m_RotationSpeed = 180.f;

        // For mouse input.
        float m_LastMouseX = 0.0f, m_LastMouseY = 0.0f;
    };

}