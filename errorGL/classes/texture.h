#pragma once
#include <glad/glad.h>
#include "stb_image.h"
#include <string>
#include <iostream>
using namespace std;
class texture
{
	public:
	unsigned int id;
	GLuint uniform;
	texture(const char* directory);

};

