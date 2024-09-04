#pragma once
#include "glad/glad.h"
class vbo
{
public:
	GLuint buffer;
	void createVbo();
	void bindVbo();
	void unBindVbo();
	void sendData(GLsizeiptr size, GLfloat* verts);
	
};

