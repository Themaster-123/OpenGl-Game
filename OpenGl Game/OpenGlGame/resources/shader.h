#pragma once

#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"
#include <vector>
#include "material.h"

namespace glg {


	class Shader
	{
	public:
		unsigned int ID;

		Shader() {
		}

		Shader(const char* vertexPath, const char* fragmentPath);

		Shader(const char* computePath);

		void use() {
			glUseProgram(ID);
		}

		void compute(glm::ivec3 numGroups) {
			use();
			glDispatchCompute(numGroups.x, numGroups.y, numGroups.z);
		}

		void compute(uint32_t x, uint32_t y, uint32_t z) {
			use();
			glDispatchCompute(x, y, z);
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
		void setVec2(const std::string& name, const glm::vec2& value) const
		{
			glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
		}
		void setVec2(const std::string& name, float x, float y) const
		{
			glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
		}
		void setVec3(const std::string& name, const glm::vec3& value) const
		{
			glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
		}
		void setVec3(const std::string& name, float x, float y, float z) const
		{
			glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
		}
		void setVec4(const std::string& name, const glm::vec4& value) const
		{
			glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
		}
		void setVec4(const std::string& name, float x, float y, float z, float w)
		{
			glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
		}
		void setIVec2(const std::string& name, const glm::ivec2& value) const
		{
			glUniform2iv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
		}
		void setIVec2(const std::string& name, float x, float y) const
		{
			glUniform2i(glGetUniformLocation(ID, name.c_str()), x, y);
		}
		void setIVec3(const std::string& name, const glm::ivec3& value) const
		{
			glUniform3iv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
		}
		void setIVec3(const std::string& name, float x, float y, float z) const
		{
			glUniform3i(glGetUniformLocation(ID, name.c_str()), x, y, z);
		}
		void setIVec4(const std::string& name, const glm::ivec4& value) const
		{
			glUniform4iv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
		}
		void setIVec4(const std::string& name, float x, float y, float z, float w)
		{
			glUniform4i(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
		}
		void setMat2(const std::string& name, const glm::mat2& mat) const
		{
			glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
		}
		void setMat3(const std::string& name, const glm::mat3& mat) const
		{
			glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
		}
		void setMat4(const std::string& name, const glm::mat4& mat) const
		{
			glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
		}
		void setMaterial(const std::string& name, const Material& material) const {
			this->setVec3(name + ".ambient", material.ambient);
			this->setVec3(name + ".diffuse", material.diffuse);
			this->setVec3(name + ".specular", material.specular);
			this->setFloat(name + ".shininess", material.shininess);
		}

		enum class CompileErrorType
		{
			Shader,
			Program
		};

	private:

		void checkCompileErrors(unsigned int shader, CompileErrorType type) {
			int success;
			char infoLog[512];
			switch (type)
			{
			case CompileErrorType::Program:
				glGetProgramiv(shader, GL_LINK_STATUS, &success);
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

