#include "tepch.h"
#include"Shader.h"

#include <glad/glad.h>


namespace TheEngine {
	// Reads a text file and outputs a string with everything in the text file
	std::string get_file_contents(const char* filename)
	{
		std::ifstream in(filename, std::ios::binary);
		if (in)
		{
			std::string contents;
			in.seekg(0, std::ios::end);
			contents.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&contents[0], contents.size());
			in.close();
			return(contents);
		}
		throw(errno);
	}

	// Constructor that build the Shader Program from 2 different shaders
	Shader::Shader(const char* vertexFile, const char* fragmentFile)
	{
		// Read vertexFile and fragmentFile and store the strings
		/*std::string vertexCode = get_file_contents(vertexFile);
		std::string fragmentCode = get_file_contents(fragmentFile);*/

		std::string vertexCode = R"(
			#version 330 core
			layout(location = 0) in vec3 aPos;
			layout(location = 1) in vec4 aCol;

			out vec4 vertexColor;
			void main()
			{
				vertexColor = aCol;
				gl_Position = vec4(aPos, 1.0);
			}
		)";
		std::string fragmentCode = R"(
			#version 330 core
			out vec4 FragColor;

			in vec4 vertexColor;
			void main()
			{
				FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
				FragColor = vertexColor;
			}
		)";

		// Convert the shader source strings into character arrays
		const char* vertexSource = vertexCode.c_str();
		const char* fragmentSource = fragmentCode.c_str();

		// Create Vertex Shader Object and get its reference
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		// Attach Vertex Shader source to the Vertex Shader Object
		glShaderSource(vertexShader, 1, &vertexSource, NULL);
		// Compile the Vertex Shader into machine code
		glCompileShader(vertexShader);
		// Checks if Shader compiled succesfully
		compileErrors(vertexShader, "VERTEX");

		// Create Fragment Shader Object and get its reference
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		// Attach Fragment Shader source to the Fragment Shader Object
		glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
		// Compile the Vertex Shader into machine code
		glCompileShader(fragmentShader);
		// Checks if Shader compiled succesfully
		compileErrors(fragmentShader, "FRAGMENT");

		// Create Shader Program Object and get its reference
		m_RendererID = glCreateProgram();
		// Attach the Vertex and Fragment Shaders to the Shader Program
		glAttachShader(m_RendererID, vertexShader);
		glAttachShader(m_RendererID, fragmentShader);
		// Wrap-up/Link all the shaders together into the Shader Program
		glLinkProgram(m_RendererID);
		// Checks if Shaders linked succesfully
		compileErrors(m_RendererID, "PROGRAM");

		// Delete the now useless Vertex and Fragment Shader objects
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

	}

	Shader::Shader(std::string& vertexFile, std::string& fragmentFile)
	{
		// Read vertexFile and fragmentFile and store the strings
		/*std::string vertexCode = get_file_contents(vertexFile);
		std::string fragmentCode = get_file_contents(fragmentFile);*/

		std::string vertexCode = vertexFile;
		std::string fragmentCode = fragmentFile;

		// Convert the shader source strings into character arrays
		const char* vertexSource = vertexCode.c_str();
		const char* fragmentSource = fragmentCode.c_str();

		// Create Vertex Shader Object and get its reference
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		// Attach Vertex Shader source to the Vertex Shader Object
		glShaderSource(vertexShader, 1, &vertexSource, NULL);
		// Compile the Vertex Shader into machine code
		glCompileShader(vertexShader);
		// Checks if Shader compiled succesfully
		compileErrors(vertexShader, "VERTEX");

		// Create Fragment Shader Object and get its reference
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		// Attach Fragment Shader source to the Fragment Shader Object
		glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
		// Compile the Vertex Shader into machine code
		glCompileShader(fragmentShader);
		// Checks if Shader compiled succesfully
		compileErrors(fragmentShader, "FRAGMENT");

		// Create Shader Program Object and get its reference
		m_RendererID = glCreateProgram();
		// Attach the Vertex and Fragment Shaders to the Shader Program
		glAttachShader(m_RendererID, vertexShader);
		glAttachShader(m_RendererID, fragmentShader);
		// Wrap-up/Link all the shaders together into the Shader Program
		glLinkProgram(m_RendererID);
		// Checks if Shaders linked succesfully
		compileErrors(m_RendererID, "PROGRAM");

		// Delete the now useless Vertex and Fragment Shader objects
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

	}

	// Activates the Shader Program
	void Shader::Bind()
	{
		glUseProgram(m_RendererID);
	}

	// Deactivates the Shader Program
	void Shader::Unbind()
	{
		glUseProgram(0);
	}

	// Deletes the Shader Program
	void Shader::Delete()
	{
		glDeleteProgram(m_RendererID);
	}

	// Checks if the different Shaders have compiled properly
	void Shader::compileErrors(unsigned int shader, const char* type)
	{
		// Stores status of compilation
		GLint hasCompiled;
		// Character array to store error message in
		char infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
			if (hasCompiled == GL_FALSE)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				TE_CORE_ERROR("SHADER_COMPILATION_ERROR for: {0}", type);
				TE_CORE_ERROR("{0}", infoLog);
				TE_CORE_ASSERT(false, "Shader compilation failure!");
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
			if (hasCompiled == GL_FALSE)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				TE_CORE_ERROR("SHADER_LINKING_ERROR for: {0}", type);
				TE_CORE_ERROR("{0}", infoLog);
				TE_CORE_ASSERT(false, "Shader linking failure!");
			}
		}
	}

	void Shader::setBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(m_RendererID, name.c_str()), (int)value);
	}
	// ------------------------------------------------------------------------
	void Shader::setInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(m_RendererID, name.c_str()), value);
	}
	// ------------------------------------------------------------------------
	void Shader::setFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(m_RendererID, name.c_str()), value);
	}
	// ------------------------------------------------------------------------
	void Shader::setVec2(const std::string& name, const glm::vec2& value) const
	{
		glUniform2fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, &value[0]);
	}
	void Shader::setVec2(const std::string& name, float x, float y) const
	{
		glUniform2f(glGetUniformLocation(m_RendererID, name.c_str()), x, y);
	}
	// ------------------------------------------------------------------------
	void Shader::setVec3(const std::string& name, const glm::vec3& value) const
	{
		glUniform3fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, &value[0]);
	}
	void Shader::setVec3(const std::string& name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(m_RendererID, name.c_str()), x, y, z);
	}
	// ------------------------------------------------------------------------
	void Shader::setVec4(const std::string& name, const glm::vec4& value) const
	{
		glUniform4fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, &value[0]);
	}
	void Shader::setVec4(const std::string& name, float x, float y, float z, float w) const
	{
		glUniform4f(glGetUniformLocation(m_RendererID, name.c_str()), x, y, z, w);
	}
	// ------------------------------------------------------------------------
	void Shader::setMat2(const std::string& name, const glm::mat2& mat) const
	{
		glUniformMatrix2fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	void Shader::setMat3(const std::string& name, const glm::mat3& mat) const
	{
		glUniformMatrix3fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

}