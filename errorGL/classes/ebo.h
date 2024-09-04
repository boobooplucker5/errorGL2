#pragma once
#include <glad/glad.h>
class ebo
{
public:
	GLuint buffer;
	void createEbo();
	void bindEbo();
	void unBindEbo();
	void sendData(GLsizeiptr size, GLuint* inds);
};

