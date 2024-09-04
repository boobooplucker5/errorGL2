#include "shader.h"

// theres a GLuint in the class to use the program btw
// theres a GLuint in the class to use the program btw

 void shader::createShader(const char* vertShaderCode, const char* fragShaderCode)
{
	GLuint vertShader;
	GLuint fragShader;

	program = glCreateProgram();

	vertShader = glCreateShader(GL_VERTEX_SHADER);
	fragShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertShader, 1, &vertShaderCode, 0);
	glShaderSource(fragShader, 1, &fragShaderCode, 0);

	glCompileShader(vertShader);
	glCompileShader(fragShader);

	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);

	glLinkProgram(program);

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);
}
