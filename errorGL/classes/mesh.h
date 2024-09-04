#include "glad/glad.h"
#include "matt3.h"
#include <vector>
#include "vec3.h"
#include "vec2.h"
#include <fstream>
#include <iostream>

using namespace std;
 
class mesh
{

public:


	unsigned int* index;
	float* verts;
	float* tverts;

	unsigned int tisize;
	unsigned int tvsize;
	matt3* transmat;


	int* search(string line, int start, char seperator);
	void slash(string line, int &s, int &s2);
	void load(const char* directory);
	void convert(const char* directory);
	void objload(const char* directory);
};

