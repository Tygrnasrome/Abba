#include "objloader.h"

ObjLoader::ObjLoader()
{

}
void ObjLoader::loadFile(const char *filename)
{
	float x,y,z;
	int a,b,c, n;
	a = b = c = n = 0;
	char d;
	std::vector <float> vertexes;
	std::vector <float> normals;
	std::vector <float> uv;

	std::vector <GLfloat> va;
	std::vector <GLuint> ia;


	std::string starting_chars;
	FILE* f;
	if ((f=fopen(filename, "r"))== NULL)
	{
		printf("soubor %s se nepodařilo otevřít", filename);
		return;
	}
	while(!feof(f))
	{
		while (d != 'v' && d != 'f' && !feof(f)) {
			d = fgetc(f);
		}
		starting_chars = "";
		starting_chars += d;
		d = fgetc(f);
		starting_chars += d;

		if(starting_chars == "v ")
		{
			//vertex
			fscanf(f, "%f %f %f", &x, &y, &z);
			vertexes.push_back(x);
			vertexes.push_back(y);
			vertexes.push_back(z);
		}
		if(starting_chars == "vt")
		{
			//uv
			fscanf(f, " %f %f", &x, &y);
			uv.push_back(x);
			uv.push_back(y);
		}
		if(starting_chars == "vn")
		{
			//normal
			fscanf(f, " %f %f %f", &x, &y, &z);
			normals.push_back(x);
			normals.push_back(y);
			normals.push_back(z);
		}

		if(starting_chars == "f ")
		{
			for(int i = 0;i<3;i++)
			{
				fscanf(f, "%d/%d/%d", &a, &b, &c);
				ia.push_back(n++);
				va.push_back(vertexes[(a - 1)*3]);
				va.push_back(vertexes[(a - 1)*3+1]);
				va.push_back(vertexes[(a - 1)*3+2]);
				va.push_back(0);
				va.push_back(1);
				va.push_back(0);
				va.push_back(uv[(b - 1)*2]);
				va.push_back(uv[(b - 1)*2+1]);
				va.push_back(normals[(c - 1)*3]);
				va.push_back(normals[(c - 1)*3+1]);
				va.push_back(normals[(c - 1)*3+2]);

			}
		}
		if(starting_chars != "v " && starting_chars != "vn" && starting_chars != "vt" && starting_chars != "f ")
			printf("sth went wrong in objload, starting char: %c %s\n", d, starting_chars.c_str());

	}
	if (fclose(f) == EOF)
	{
		printf("soubor %s se nepodařilo uzavřít", filename);
		return;
	}
	vao.vertexBuffer(va.data(), va.size() * 4, {3, 3, 2, 3});
	vao.indexBuffer(ia.data(), ia.size() * 4, GL_TRIANGLES);
}
