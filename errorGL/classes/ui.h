#pragma once
#include "texture.h"
#include <vector>
#include "vec3.h"
class ui
{
public:
	ui(float xp, float yp, float xs, float ys, vector<texture> txt, string txtstr, vec3 clr, vec3 txtcolor)
	{
		xpos = xp; ypos = yp;
		xsize = xs; ysize = ys;
		text = txt; textstring = txtstr;
		color = clr; textcolor = txtcolor;

	}
	int xpos;
	int ypos;
	int xsize;
	int ysize;
	string textstring;
	vector<texture> text;
	vec3 color;
	vec3 textcolor;
};

