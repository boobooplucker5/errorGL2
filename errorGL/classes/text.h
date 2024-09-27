#pragma once
#include <string>
#include <map>
#include "character.h"
#include "vec2.h"
class text
{
public:
	vec2 position;
	vec2 bearing;
	unsigned int advance;
	unsigned int size;
	float* vertices;
	unsigned int* indices;
};

