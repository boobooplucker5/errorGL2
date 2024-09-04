#pragma once
#include "glad/glad.h"
class shader
{
public:
	GLuint program;

	void createShader(const char* vertShaderCode, const char* fragShaderCode);
	

};

