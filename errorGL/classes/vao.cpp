#include "vao.h"

void vao::createVao()
{
	glGenVertexArrays(1, &buffer);
}
void vao::bindVao()
{
	glBindVertexArray(buffer);
}
void vao::unBindVao()
{
	glBindVertexArray(0);
}

void vao::linkVbo(GLuint index)
{
	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


	
}