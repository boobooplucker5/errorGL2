#pragma once
#include "mesh.h"


class object
{
public:
	mesh objectmesh;
	drawobj drawobject;
	vec3 position;
	vec3 size;
	vec3 orientation;
	
	void create(const char* meshdirectory);

};

