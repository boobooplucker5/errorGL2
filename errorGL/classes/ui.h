#pragma once
#include "texture.h"
#include <vector>
#include "vec2.h"
#include "vec3.h"
#include "text.h"


class ui
{
public:
	vec2 pos;
	vec2 size;
	text txt;
	vec3 color;
	vec3 txtcolor;
	drawobj drawobject;
	float* vertices;
	unsigned int* indices;
	void create();
	void mousein(float mx, float my);
	void update();
	void updverts();
	
};

