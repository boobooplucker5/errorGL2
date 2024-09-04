#pragma once
#include <glad/glad.h>
class vao
{
public:
	GLuint buffer;
	void createVao();
	void bindVao();
	void unBindVao();
	void linkVbo(GLuint index);
};

