#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "pyramid.h"
#include "attribarray.h"

Pyramid::Pyramid()
{
	num_of_elements = 18;

	GLfloat vertex_array[] =
	{
		// position     color       UV		normála (normálu opravdu ještě nemám, čísla ignorujte)

		//bottom face 1/2
		-1,-1,  1,	0, 1, 0,	0, 1,		0,-1,0,
		-1,-1, -1,	0, 1, 0,	0, 0,		0,-1,0,
		1, -1, -1,	0, 1, 0,	1, 0,		0,-1,0,


		//bottom face 2/2
		-1,-1,  1,	0, 1, 0,	0, 1,		0,-1,0,
		1, -1, -1,	0, 1, 0,	1, 0,		0,-1,0,
		1, -1,  1,	0, 1, 0,	1, 1,		0,-1,0,

		//left face
		-1, -1, 1,	0, 1, 0,	0, 1,		-1,1,0,
		-1,-1,-1,	0, 1, 0,	0, 0,		-1,1,0,
		0, 1, 0,	0, 1, 0,	0.5, 0.5,	-1,1,0,
		//right face
		1, -1, 1,	0, 1, 0,	1, 1,		1,1,0,
		1,-1,-1,	0, 1, 0,	1, 0,		1,1,0,
		0, 1, 0,	0, 1, 0,	0.5, 0.5,	1,1,0,


		//front face
		-1,-1,-1,	0, 1, 0,	0, 0,		0,1,-1,
		1,-1,-1,	0, 1, 0,	1, 0,		0,1,-1,
		0,1,0,		0, 1, 0,	0.5, 0.5,	0,1,-1,

		//back face
		-1,-1,1,	0, 1, 0,	0, 1,		0,1,1,
		1,-1,1,		0, 1, 0,	1, 1,		0,1,1,
		0,1,0,		0, 1, 0,	0.5, 0.5,	0,1,1,
	};

	vao.vertexBuffer(vertex_array, sizeof(vertex_array), {3, 3, 2, 3});

	GLuint index_array[] =
	{
		0, 1, 2,
		3,4,5,
		6,7,8,
		9,10,11,
		12,13,14,
		15,16,17,
	};

	vao.indexBuffer(index_array, sizeof(index_array), GL_TRIANGLES);
	seed = glm::vec2((float) rand()/RAND_MAX*100);

}


void Pyramid::draw(Shader *shader, Camera *camera)
{
	shader->use();
	shader->uniform("view",camera->view);

	//specifikace materialu
	shader->uniform("model", getMatrix());
	shader->uniform("seed",seed);
	shader->uniform("projection",camera->projection);

	vao.draw();
}

