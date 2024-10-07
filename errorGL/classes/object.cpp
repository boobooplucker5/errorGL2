#include "object.h"

void object::create(const char* meshdirectory)
{

	objectmesh.load(meshdirectory);
	for (int i = 0; i < objectmesh.tvsize / 4; i+=8)
	{

		objectmesh.verts[i] *= size.x;
		objectmesh.verts[i+1] *= size.y;
		objectmesh.verts[i+2] *= size.z;
		objectmesh.verts[i] += position.x;
		objectmesh.verts[i + 1] += position.y;
		objectmesh.verts[i + 2] += position.z;
	
	}

	
}
