#include "text.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include <string>
using namespace std;

void text::settext()
{
	float xpos = position.x / 800;
	float ypos = position.y / 800;
	vertices = new float* [textstring.length()];
	drawobjects = new drawobj[textstring.length()];

	for (int i = 0; i < textstring.length(); i++)
	{


		vertices[i] = new float[16];

	}
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	indices[3] = 1;
	indices[4] = 3;
	indices[5] = 2;

	for (int i = 0; i < textstring.length(); i++)
	{


		character chr = characters[textstring.at(i)];

		float xposition = xpos + float(chr.bearing.x) / 800;
		float yposition = ypos - float(chr.size.y - chr.bearing.y) / 800;

		vertices[i][0] = xposition;
		vertices[i][1] = yposition;
		vertices[i][2] = 0;
		vertices[i][3] = 0;

		vertices[i][4] = xposition + float(chr.size.x) / 800;
		vertices[i][5] = yposition;
		vertices[i][6] = 1;
		vertices[i][7] = 0;

		vertices[i][8] = xposition;
		vertices[i][9] = yposition + float(chr.size.y) / 800;
		vertices[i][10] = 0;
		vertices[i][11] = 1;

		vertices[i][12] = xposition + float(chr.size.x) / 800;
		vertices[i][13] = yposition + float(chr.size.y) / 800;
		vertices[i][14] = 1;
		vertices[i][15] = 1;




		xpos += float(chr.advance) / 64 / 800;


	}
	if (centered == true)
	{
		float tingye = (position.x - vertices[textstring.length() - 1][12]) / 2;

		for (int i = 0; i < textstring.length(); i++)
		{
			for (int x = 0; x < 16; x += 4)
			{
				vertices[i][x] += tingye;
			}

		}
		//xpos -= tingye;

	}
}
