#include "ui.h"
void ui::create()
{
	vertices = new float[20];
	indices = new unsigned int[6];
	vertices[0] = (pos.x - size.x / 2);
	vertices[1] = (pos.y - size.y / 2);

	vertices[2] = color.x;
	vertices[3] = color.y;
	vertices[4] = color.z;

	vertices[5] = (pos.x + size.x / 2);
	vertices[6] = (pos.y - size.y / 2);

	vertices[7] = color.x;
	vertices[8] = color.y;
	vertices[9] = color.z;

	vertices[10] = (pos.x + size.x / 2);
	vertices[11] = (pos.y + size.y / 2);

	vertices[12] = color.x;
	vertices[13] = color.y;
	vertices[14] = color.z;

	vertices[15] = (pos.x - size.x / 2);
	vertices[16] = (pos.y + size.y / 2);

	vertices[17] = color.x;
	vertices[18] = color.y;
	vertices[19] = color.z;

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 0;
	indices[4] = 2;
	indices[5] = 3 ;

}
void ui::mousein(float mx, float my)
{
	if (mx > pos.x - size.x / 2 && mx < pos.x + size.x / 2)
	{
		if (my > pos.y - size.y / 2 && my < pos.y + size.y / 2)
		{
			cout << "yay\n";
		}
	}
}
void ui::update()
{
	glBindVertexArray(drawobject.vao);
	glBindBuffer(GL_ARRAY_BUFFER, drawobject.vbo);
	glBufferData(GL_ARRAY_BUFFER,80,vertices,GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
void ui::updverts()
{

	vertices[0] = (pos.x - size.x / 2);
	vertices[1] = (pos.y - size.y / 2);

	vertices[2] = color.x;
	vertices[3] = color.y;
	vertices[4] = color.z;

	vertices[5] = (pos.x + size.x / 2);
	vertices[6] = (pos.y - size.y / 2);

	vertices[7] = color.x;
	vertices[8] = color.y;
	vertices[9] = color.z;

	vertices[10] = (pos.x + size.x / 2);
	vertices[11] = (pos.y + size.y / 2);

	vertices[12] = color.x;
	vertices[13] = color.y;
	vertices[14] = color.z;

	vertices[15] = (pos.x - size.x / 2);
	vertices[16] = (pos.y + size.y / 2);

	vertices[17] = color.x;
	vertices[18] = color.y;
	vertices[19] = color.z;
}