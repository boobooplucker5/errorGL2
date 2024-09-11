#pragma once
class drawobj
{
public:
	unsigned int vbo;
	unsigned int vao;
	unsigned int ebo;
	drawobj(unsigned int b, unsigned int a, unsigned int e)
	{
		vbo = b;
		vao = a;
		ebo = e;
	}
};

