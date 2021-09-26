#include "shader.h"

glg::Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
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
	checkCompileErrors(vertexShader, CompileErrorType::Shader);

	// Compiling Fragment Shader
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
	glCompileShader(fragmentShader);
	checkCompileErrors(fragmentShader, CompileErrorType::Shader);

	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);
	checkCompileErrors(ID, CompileErrorType::Program);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glUseProgram(ID);
}

glg::Shader::Shader(const char* computePath)
{
	std::string computeCode;
	std::ifstream cShaderFile;
	cShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		cShaderFile.open(computePath);
		std::stringstream cShaderStream;
		cShaderStream << cShaderFile.rdbuf();
		computeCode = cShaderStream.str();
	}
	catch (std::ifstream::failure ex) {
		std::cout << "Could not read shader file" << std::endl;
	}

	const char* cShaderCode = computeCode.c_str();
	unsigned int computeShader;
	// Compiling Vertex Shader
	computeShader = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(computeShader, 1, &cShaderCode, NULL);
	glCompileShader(computeShader);
	checkCompileErrors(computeShader, CompileErrorType::Shader);

	// Compiling Fragment Shader


	ID = glCreateProgram();
	glAttachShader(ID, computeShader);
	glLinkProgram(ID);
	checkCompileErrors(ID, CompileErrorType::Program);

	glDeleteShader(computeShader);
	glUseProgram(ID);
}
