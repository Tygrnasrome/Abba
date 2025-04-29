#include "cylinder.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Cylinder::Cylinder(int sides)
{
	GLfloat va[(sides +1) *2* 17];
	GLuint ia[sides * 6];

	int n = 0;
	int m = 0;
	for(int i = 0; i <= sides ; i++)
	{
		va[n++] = cos(2 * M_PI/sides * i);			//pos
		va[n++] = 1;						//pos
		va[n++] = sin(2 * M_PI/sides * i);			//pos

		va[n++] = 0;	va[n++] = 1;	va[n++] = 0;		//color

		va[n++] = (float)i/sides;				//uv
		va[n++] = 1;						//uv


		va[n++] = cos(2 * M_PI/sides * i);			//normal
		va[n++] = 1;						//normal
		va[n++] = sin(2 * M_PI/sides * i);			//normal

		va[n++] = cos(2 * M_PI/sides * i + M_PI / 2);		//tangenta
		va[n++] = 1;						//tangenta
		va[n++] = sin(2 * M_PI/sides * i + M_PI / 2);		//tangenta

		va[n++] = 0;						//bitangenta
		va[n++] = 1;						//bitangenta
		va[n++] = 0;						//bitangenta


		va[n++] = cos(2 * M_PI/sides * i);			//pos
		va[n++] = -1;						//pos
		va[n++] = sin(2 * M_PI/sides * i);			//pos

		va[n++] = 0;	va[n++] = 1;	va[n++] = 0;		//color

		va[n++] = (float)i/sides;				//uv
		va[n++] = 0;						//uv

		va[n++] = cos(2 * M_PI/sides * i);			//normal
		va[n++] = -1;						//normal
		va[n++] = sin(2 * M_PI/sides * i);			//normal

		va[n++] = cos(2 * M_PI/sides * i + M_PI / 2);		//tangenta
		va[n++] = -1;						//tangenta
		va[n++] = sin(2 * M_PI/sides * i + M_PI / 2);		//tangenta

		va[n++] = 0;						//bitangenta
		va[n++] = -1;						//bitangenta
		va[n++] = 0;						//bitangenta
	}

	vao.vertexBuffer(va, sizeof(va), {3, 3, 2, 3, 3, 3});

	for(int i = 0; i < sides; i++)
	{
		ia[m++] = i*2;
		ia[m++] = i*2+1;
		ia[m++] = i*2+2;
		ia[m++] = i*2+2;
		ia[m++] = i*2+1;
		ia[m++] = i*2+3;
	}
	vao.indexBuffer(ia, sizeof(ia), GL_TRIANGLES);
}
