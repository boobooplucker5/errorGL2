#include "ebo.h"
void ebo::createEbo()
{
	glGenBuffers(1, &buffer);
}
void ebo::bindEbo()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
}
void ebo::unBindEbo()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void ebo::sendData(GLsizeiptr size, GLuint* inds)
{
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size , inds, GL_STATIC_DRAW);

}