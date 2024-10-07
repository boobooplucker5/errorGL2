#include "matt3.h"
void matt3::empty()
{
	m[0] = 0; m[1] = 0; m[2] = 0;
	m[3] = 0; m[4] = 0; m[5] = 0; 
	m[6] = 0; m[7] = 0; m[8] = 0;
}
void matt3::translate()
{ 
	m[0] = 1; m[1] = 0; m[2] = 0;
	m[3] = 0; m[4] = 1; m[5] = 0;
	m[6] = 0; m[7] = 0; m[8] = 1;
}
void matt3::fill()
{
	m[0] = 1 ; m[1] = 1; m[2] = 1;
	m[3] = 1; m[4] = 1; m[5] = 1;
	m[6] = 1; m[7] = 1; m[8] = 1;
}

void matt3::vertmult(float* verts, float* tverts, unsigned int vsize)
{

	for (int i = 0; i < vsize; i+=8)
	{


		tverts[i] = verts[i] * m[0] + verts[i + 1] * m[1] + verts[i + 2] * m[2];
		tverts[i+1] = verts[i] * m[3] + verts[i + 1] * m[4] + verts[i + 2] * m[5];
		tverts[i+2] = verts[i] * m[6] + verts[i + 1] * m[7] + verts[i + 2] * m[8];

		tverts[i+5] = verts[i+5] * m[0] + verts[i+6] * m[1] + verts[i+7] * m[2];
		tverts[i+6] = verts[i+5] * m[3] + verts[i+6] * m[4] + verts[i+7] * m[5];
		tverts[i+7] = verts[i+5] * m[6] + verts[i+6] * m[7] + verts[i+7] * m[8];

	}
}