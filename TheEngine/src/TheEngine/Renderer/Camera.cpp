#include "tepch.h"
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace TheEngine {


    Camera2D::Camera2D(float left, float right, float bottom, float top)
    {
        m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
        m_ViewMatrix = glm::mat4(1.0f);
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

    void Camera2D::SetProjection(float left, float right, float bottom, float top)
    {
        m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

    void Camera2D::RecalculateViewProjectionMatrix()
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
            glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

        m_ViewMatrix = glm::inverse(transform);
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }




    Camera3D::Camera3D(const glm::vec3& position, float fov, float aspectRatio, float nearClip, float farClip)
    {
        m_Position = position;
        m_ProjectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);
        RecalculateViewProjectionMatrix();
    }

    void Camera3D::SetProjection(float fov, float aspectRatio, float nearClip, float farClip)
    {
        m_ProjectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

    void Camera3D::SetPosition(const glm::vec3& position)
    {
        m_Position = position;
        RecalculateViewProjectionMatrix();
    }

    void Camera3D::SetRotation(float yaw, float pitch, float roll)
    {
        m_Yaw = yaw;
        m_Pitch = pitch;
        m_Roll = roll;
        RecalculateViewProjectionMatrix();
    }

    void Camera3D::RecalculateViewProjectionMatrix()
    {
        glm::mat4 rotation = glm::yawPitchRoll(glm::radians(m_Yaw), glm::radians(m_Pitch), glm::radians(m_Roll));

        glm::vec3 forward = glm::normalize(glm::vec3(rotation * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)));
        glm::vec3 up = glm::normalize(glm::vec3(rotation * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f)));

        m_ViewMatrix = glm::lookAt(m_Position, m_Position + forward, up);
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

}