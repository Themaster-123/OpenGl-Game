#pragma once

#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <sstream>

namespace OGG {
	class Shader
	{
	public:
		unsigned int ID;

		Shader(const char* vertexPath, const char* fragmentPath) {
			std::string vertexCode;
			std::string fragmentCode;
			std::ifstream vShaderFile;
			std::ifstream fShaderFile;
			vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
			fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

			try {
				vShaderFile.open(vertexPath);
				fShaderFile.open(fragmentPath);
				std::stringstream vShaderStream, fShaderStream;
				vShaderStream << vShaderFile.rdbuf();
				fShaderStream << fShaderFile.rdbuf();
				vertexCode = vShaderStream.str();
				fragmentCode = fShaderStream.str();
			}
			catch (std::ifstream::failure ex) {
				std::cout << "Could not read shader file" << std::endl;
			}

			const char* vShaderCode = vertexCode.c_str();
			const char* fShaderCode = fragmentCode.c_str();
			unsigned int vertexShader, fragmentShader;
			// Compiling Vertex Shader
			vertexShader = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertexShader, 1, &vShaderCode, NULL);
			glCompileShader(vertexShader);
			checkCompileErrors(vertexShader, CompileErrorType::Vertex);

			// Compiling Fragment Shader
			fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
			glCompileShader(fragmentShader);
			checkCompileErrors(fragmentShader, CompileErrorType::Fragment);

			ID = glCreateProgram();
			glAttachShader(ID, vertexShader);
			glAttachShader(ID, fragmentShader);
			glLinkProgram(ID);
			checkCompileErrors(ID, CompileErrorType::Program);

			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);
		}
		void use() {
			glUseProgram(ID);
		}
		void setBool(const std::string& name, bool value) const {
			glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
		}
		void setInt(const std::string& name, int value) const {
			glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
		}
		void setFloat(const std::string& name, float value) const {
			glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
		}

		enum class CompileErrorType
		{
			Vertex,
			Fragment,
			Program
		};

	private:
		void checkCompileErrors(unsigned int shader, CompileErrorType type) {
			int success;
			char infoLog[512];
			switch (type)
			{
			case CompileErrorType::Program:
				glGetProgramiv(shader, GL_COMPILE_STATUS, &success);
				if (!success) {
					glGetProgramInfoLog(ID, 512, NULL, infoLog);
					std::cout << "Shader Program Failed: " << infoLog << std::endl;
				}
				break;
			default:
				glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
				if (!success) {
					glGetShaderInfoLog(shader, 512, NULL, infoLog);
					std::cout << "Shader Compilation Faild: " << infoLog << std::endl;
				}
				break;
			}
		}
	};
}

