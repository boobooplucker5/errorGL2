#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <string>
#include <map>
#include "character.h"
#include "vec2.h"
#include "vec3.h"
#include "drawobj.h"
#include <string>
using namespace std;
class text
{

public:

	drawobj* drawobjects;
	bool centered;
	bool typing;
	map<char, character> characters;
	string textstring;
	vec2 position;
	vec3 textcolor;
	float** vertices;
	unsigned int indices[6];
	void settext();
	
};