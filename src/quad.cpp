#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "quad.h"

Quad::Quad()
{
	num_of_elements = 6;

	GLfloat vertex_array[] =
	{
		// position     color       UV		normála	  tangenta	bitangenta
		// Top face
		-1, -1, 0,	0, 1, 0,	0, 0,		0, 0, 1,    1, 0, 0,    0, 1, 0,
		1, -1, 0,	0, 1, 0,	1, 0,		0, 0, 1,    1, 0, 0,    0, 1, 0,
		1,  1, 0,	0, 1, 0,	1, 1,		0, 0, 1,    1, 0, 0,    0, 1, 0,
		-1,  1, 0,	0, 1, 0,	0, 1,		0, 0, 1,    1, 0, 0,    0, 1, 0,

	};

	vao.vertexBuffer(vertex_array, sizeof(vertex_array), {3, 3, 2, 3, 3, 3});

	GLuint index_array[] =
	{
		0, 1, 2,  0, 2, 3,
	};

	vao.indexBuffer(index_array, sizeof(index_array), GL_TRIANGLES);

}
