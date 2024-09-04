#include "mesh.h"
#include "vec3.h"
#include "vec2.h"
#include "glad/glad.h"
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <cstdlib>
#include <array>
#include <map>
#include <unordered_map>
using namespace std;

int* mesh::search(string line, int start, char seperator)
{
	static int seperatorIndexes[3] = {};
	int size = 0;
	for (int i = start; i < line.length(); i++)
	{
		if (line[i] == seperator)
		{


			seperatorIndexes[size] = i;
			size++;
		}
	}


	return seperatorIndexes;
}
void mesh::slash(string line, int& s, int& s2)
{

	for (int i = 0; i < line.length(); i++)
	{
		if (line[i] == '/')
		{
			if (s == -1)
			{
				s = i;
			}
			else
			{

				s2 = i;
			}

		}
	}

}

void mesh::objload(const char* directory)
{

	fstream file;
	file.open(directory);

	if (file.fail())
	{
		cout << "bad directory, mb gang\n";
		return;
	}
	string line;
	unsigned int trisize = 0;
	unsigned int vertexsize = 0;
	unsigned int uvcordsize = 0;
	unsigned int normalsize = 0;

	unsigned int vertexinc = 0;
	unsigned int normalinc = 0;
	unsigned int uvcordinc = 0;

	unsigned int trinc = 0;
	unsigned int filesetinc = 1;

	vec2* uvcord;
	vec3* vertex;
	vec3* normal;

	while (getline(file, line))
	{
		switch (line[0]) {


		case 'v':
			switch (line[1])
			{

			case 't':
				uvcordsize++;
				break;
			case 'n':
				normalsize++;
				break;
			case ' ':
				vertexsize++;
				break;
			}
			break;
		case 'f':
			trisize++;

			break;

		default:
			break;
		};



	}
	trisize *= 3;

	vertex = new vec3[vertexsize];
	index = new unsigned int[trisize];

	normal = new vec3[normalsize];

	uvcord = new vec2[uvcordsize];




	map<array<unsigned int, 3>, unsigned int> fileset;

	file.close();
	file.open(directory);

	while (getline(file, line))
	{
		int* spaces = search(line, 0, ' ');
		switch (line[0])
		{
		case 'v':
		{
			string snum = line.substr(spaces[0] + 1, spaces[1] - spaces[0] - 1);
			string snum1 = line.substr(spaces[1] + 1, spaces[2] - spaces[1] - 1);
			string snum2 = line.substr(spaces[2] + 1, line.size() - spaces[2] - 1);
			float num = stof(snum);
			float num1 = stof(snum1);
			float num2 = stof(snum2);
			switch (line[1])
			{
			case 'n':
				normal[normalinc] = { num, num1, num2 };
				normalinc++;                                             
				break;
			case 't':
			{
				string snum3 = line.substr(spaces[1] + 1, line.size() - spaces[1] - 1);
				float num3 = stof(snum3);
				uvcord[uvcordinc] = { num, num3 };
				uvcordinc++;
				break;
			}
			case ' ':
				vertex[vertexinc] = { num, num1, num2 };
				vertexinc++;
				break;
			}

			break;
		}

		case 'f':

			string spair = line.substr(spaces[0] + 1, spaces[1] - spaces[0] - 1);
			string spair1 = line.substr(spaces[1] + 1, spaces[2] - spaces[1] - 1);
			string spair2 = line.substr(spaces[2] + 1, line.size() - spaces[2] - 1);

			int slashes = -1;
			int slashes0;
			int slashes1 = -1;
			int slashes01;
			int slashes2 = -1;
			int slashes02;

			slash(spair, slashes, slashes0);
			slash(spair1, slashes1, slashes01);
			slash(spair2, slashes2, slashes02);



			string snum00 = spair.substr(0, slashes);
			string snum01 = spair.substr(slashes + 1, slashes0 - slashes - 1);
			string snum02 = spair.substr(slashes0 + 1, spair.length() - slashes0);

			string snum10 = spair1.substr(0, slashes1);
			string snum11 = spair1.substr(slashes1 + 1, slashes01 - slashes1 - 1);
			string snum12 = spair1.substr(slashes01 + 1, spair1.length() - slashes01);

			string snum20 = spair2.substr(0, slashes2);
			string snum21 = spair2.substr(slashes2 + 1, slashes02 - slashes2 - 1);
			string snum22 = spair2.substr(slashes02 + 1, spair2.length() - slashes02);


			unsigned int num00 = stoi(snum00);
			unsigned int num01 = stoi(snum01);
			unsigned int num02 = stoi(snum02);
			unsigned int num10 = stoi(snum10);
			unsigned int num11 = stoi(snum11);
			unsigned int num12 = stoi(snum12);
			unsigned int num20 = stoi(snum20);
			unsigned int num21 = stoi(snum21);
			unsigned int num22 = stoi(snum22);

			array<unsigned int, 3> nums0 = { num00, num01, num02 };
			array<unsigned int, 3> nums1 = { num10, num11, num12 };
			array<unsigned int, 3> nums2 = { num20, num21, num22 };
			if (fileset[nums0] == 0)
			{
				fileset[nums0] = filesetinc;
				filesetinc++;
			}
			if (fileset[nums1] == 0)
			{
				fileset[nums1] = filesetinc;
				filesetinc++;
			}
			if (fileset[nums2] == 0)
			{
				fileset[nums2] = filesetinc;
				filesetinc++;
			}
			index[trinc] = fileset[nums0] - 1;
			index[trinc + 1] = fileset[nums1] - 1;
			index[trinc + 2] = fileset[nums2] - 1;

			trinc += 3;


			break;

		}


	}
	verts = new float[filesetinc * 8-8];
	tverts = new float[filesetinc * 8 - 8];
	//cout << trinc/3 << "\n";
	for (auto& pair : fileset)
	{


		int sp = pair.second * 8 - 8;
		vec3 pos = vertex[pair.first[0] - 1];
		vec2 uv = uvcord[pair.first[1] - 1];
		vec3 norm = normal[pair.first[2] - 1];
		verts[sp] = pos.x;
		verts[sp + 1] = pos.y;
		verts[sp + 2] = pos.z;
		verts[sp + 3] = uv.x;
		verts[sp + 4] = uv.y;
		verts[sp + 5] = 1;
		verts[sp + 6] = 1;
		verts[sp + 7] = 1;
		verts[sp + 5] = abs(norm.x);
		verts[sp + 6] = abs(norm.y);
		verts[sp + 7] = abs(norm.z);


	}

	for (int i = 0; i < filesetinc*8-8; i++)
	{
		tverts[i] = verts[i];
	}

	tisize = trisize * 4;
	tvsize = filesetinc * 32-32;
	
}
void mesh::convert(const char* directory)
{
	ofstream binfile(directory,ios::binary);
	
		
		binfile.write(reinterpret_cast<char*>(&tvsize), sizeof(int));
		binfile.write(reinterpret_cast<char*>(&tisize), sizeof(int));
		binfile.write(reinterpret_cast<char*>(verts), tvsize);
		binfile.write(reinterpret_cast<char*>(index), int(tisize));
		binfile.close();

}
void mesh::load(const char* directory)
{

	ifstream binfile(directory, ios::binary);

	binfile.read(reinterpret_cast<char*>(&tvsize), sizeof(int));
	binfile.read(reinterpret_cast<char*>(&tisize), sizeof(int));
	verts = new float[tvsize / 4];
	tverts = new float[tvsize / 4];
	index = new unsigned int[tisize / 4];
	binfile.read(reinterpret_cast<char*>(verts), tvsize);
	binfile.read(reinterpret_cast<char*>(index), tisize);
	
	for (int i = 0; i < tvsize / 4; i++)
	{
		tverts[i] = verts[i];
	
	}
	

	//cout << tisize << "\n";
	binfile.close();
}


