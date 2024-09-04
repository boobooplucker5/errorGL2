#include "vbo.h"
void  vbo::createVbo()
{
	glGenBuffers(1,&buffer);
};

void vbo::bindVbo()
{
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
};
void vbo::unBindVbo()
{
	glBindBuffer(GL_ARRAY_BUFFER,0);
};
void vbo::sendData(GLsizeiptr size,GLfloat* verts)
{
	glBufferData(GL_ARRAY_BUFFER, size, verts, GL_STATIC_DRAW);
};