#pragma once

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>

namespace TheEngine {

    // Base Camera class
    class Camera
    {
    public:
        virtual ~Camera() = default;

        const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
        const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
        const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
        const glm::vec3& GetPosition() const { return m_Position; }

    protected:
        glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
        glm::mat4 m_ProjectionMatrix;
        glm::mat4 m_ViewMatrix;
        glm::mat4 m_ViewProjectionMatrix;
    };


    class Camera2D : public Camera
    {
    public:
        Camera2D(float left, float right, float bottom, float top);

        void SetProjection(float left, float right, float bottom, float top);
        void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewProjectionMatrix(); }
        void SetRotation(float rotation) { m_Rotation = rotation; RecalculateViewProjectionMatrix(); }

        float GetRotation() const { return m_Rotation; }

    private:
        void RecalculateViewProjectionMatrix();

    private:
        float m_Rotation = 0.0f;
    };

    class Camera3D : public Camera
    {
    public:
        Camera3D(const glm::vec3& position, float fov, float aspectRatio, float nearClip, float farClip);

        void SetProjection(float fov, float aspectRatio, float nearClip, float farClip);
        void SetPosition(const glm::vec3& position);
        void SetRotation(float yaw, float pitch, float roll = 0.0f);

    private:
        void RecalculateViewProjectionMatrix();

    private:
        float m_Yaw = 0.0f;
        float m_Pitch = 0.0f;
        float m_Roll = 0.0f;
    };

}