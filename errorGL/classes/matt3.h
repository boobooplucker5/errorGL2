#pragma once
class matt3
{
public:
	float m[9];

	float* operator*(float a[3])
	{
		//a[0] = a[0] * m[0] + a[0] * m[1] + a[0] * m[2];
		//a[1] = a[1] * m[3] + a[1] * m[4] + a[1] * m[5];
		//a[2] = a[2] * m[6] + a[2] * m[7] + a[2] * m[8];
		static float b[3] = { a[0] * m[0] + a[0] * m[1] + a[0] * m[2],    a[1] * m[3] + a[1] * m[4] + a[1] * m[5],    a[2] * m[6] + a[2] * m[7] + a[2] * m[8] };
		return b;
	}
	matt3 operator*(matt3 a)
	{
		matt3 b;
		b.m[0] = m[0] * a.m[0];
		b.m[1] = m[1] * a.m[1];
		b.m[2] = m[2] * a.m[2];

		b.m[3] = m[3] * a.m[3];
		b.m[4] = m[4] * a.m[4];
		b.m[5] = m[5] * a.m[5];

		b.m[6] = m[6] * a.m[6];
		b.m[7] = m[7] * a.m[7];
		b.m[8] = m[8] * a.m[8];
		return b;
	}
	matt3 operator+(matt3 a)
	{
		matt3 b;
		b.m[0] = m[0] * a.m[0];
		b.m[1] = m[1] * a.m[1];
		b.m[2] = m[2] * a.m[2];

		b.m[3] = m[3] * a.m[3];
		b.m[4] = m[4] * a.m[4];
		b.m[5] = m[5] * a.m[5];

		b.m[6] = m[6] * a.m[6];
		b.m[7] = m[7] * a.m[7];
		b.m[8] = m[8] * a.m[8];
		return b;
	}
	matt3 operator[](float a[9])
	{
		m[0] = a[0];
		m[1] =  a[1];
		m[2] = a[2];
		m[3] = a[3];
		m[4] = a[4];
		m[5] = a[5];
		m[6] = a[6];
		m[7] = a[7];
		m[8] = a[8];
	}
	void empty();
	void translate();
	void vertmult(float* verts, float* tverts, unsigned int vsize);
	void fill();

};


