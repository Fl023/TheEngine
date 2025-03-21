#pragma once

#include <string>
#include <glm/glm.hpp>



namespace TheEngine {

    std::string get_file_contents(const char* filename);

    class Shader
    {
    public:
        // Constructor that build the Shader Program from 2 different shaders
        Shader(const char* vertexFile, const char* fragmentFile);
        Shader(std::string& vertexFile, std::string& fragmentFile);

        // Activates the Shader Program
        void Bind();
        // Deactivates the Shader Program
        void Unbind();
        // Deletes the Shader Program
        void Delete();

        void setBool(const std::string& name, bool value) const;
        void setInt(const std::string& name, int value) const;
        void setFloat(const std::string& name, float value) const;
        void setVec2(const std::string& name, const glm::vec2& value) const;
        void setVec2(const std::string& name, float x, float y) const;
        void setVec3(const std::string& name, const glm::vec3& value) const;
        void setVec3(const std::string& name, float x, float y, float z) const;
        void setVec4(const std::string& name, const glm::vec4& value) const;
        void setVec4(const std::string& name, float x, float y, float z, float w) const;
        void setMat2(const std::string& name, const glm::mat2& mat) const;
        void setMat3(const std::string& name, const glm::mat3& mat) const;
        void setMat4(const std::string& name, const glm::mat4& mat) const;
    private:
        // Checks if the different Shaders have compiled properly
        void compileErrors(unsigned int shader, const char* type);
        // Reference ID of the Shader Program
        uint32_t m_RendererID;
    };


}