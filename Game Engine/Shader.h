#pragma once
#include <string>
class Shader
{
public:
	Shader();
	~Shader();
	int initialize(std::string, std::string);
private:
	const char * vertexShaderSource;
	const char * fragmentShaderSource;
	int vertexShader;
	int fragmentShader;
	int shaderProgram;
};

