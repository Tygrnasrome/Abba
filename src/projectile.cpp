#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "projectile.h"

Projectile::Projectile(glm::vec3 position, glm::vec3 direction, glm::vec3 color, float playerYaw)
{
	/* Vytvoříme VAO objekt sdružující buffery a konfiguraci atributů. */
	this->position = position;
	this->direction = direction;
	this->color = color;
	GLfloat vertex_array[] =
	{
		// position     color       UV			normála
		// Top face - Green
		-1, 1,  1,      0, 1, 0,    1, 0,	0,1,0,
		-1, 1, -1,      0, 1, 0,    1, 1,	0,1,0,
		1, 1, -1,       0, 1, 0,    0, 1,	0,1,0,
		1, 1,  1,       0, 1, 0,    0, 0,	0,1,0,

		// ...
		//bottom face	color       UV
		-1,-1,  1,	0, 1, 0,	0, 0,	0,-1,0,
		-1,-1, -1,	0, 1, 0,	0, 1,	0,-1,0,
		1, -1, -1,	0, 1, 0,	1, 1,	0,-1,0,
		1, -1,  1,	0, 1, 0,	1, 0,	0,-1,0,
		// ...glBindVertexArray(0);
		//left face	color       UV
		-1, -1, 1,	0, 0, 0,	1, 1,	-1,0,0,
		-1,-1,-1,	0, 0, 0,	0, 1,	-1,0,0,
		-1, 1, -1,	0, 0, 0,	0, 0,	-1,0,0,
		-1, 1, 1,	0, 0, 0,	1, 0,	-1,0,0,
		//right face	color       UV
		1, -1, 1,	0, 1, 1,	0, 1,	1,0,0,
		1,-1,-1,	0, 1, 1,	1, 1,	1,0,0,
		1, 1, -1,	0, 1, 1,	1, 0,	1,0,0,
		1, 1, 1,	0, 1, 1,	0, 0,	1,0,0,
		// ...
		//front face	color       UV
		-1,1,-1,	1, 1, 0,	1, 0,	0,0,-1,
		-1,-1,-1,	1, 1, 0,	1, 1,	0,0,-1,
		1,-1,-1,	1, 1, 0,	0, 1,	0,0,-1,
		1,1,-1,		1, 1, 0,	0, 0,	0,0,-1,
		// ...
		//back face	color  left     UV
		-1,1,1,		1, 1, 0,	0, 0,	0,0,1,
		-1,-1,1,	1, 1, 0,	0, 1,	0,0,1,
		1,-1,1,		1, 1, 0,	1, 1,	0,0,1,
		1,1,1,		1, 1, 0,	1, 0,	0,0,1,
	};

	vao.vertexBuffer(vertex_array, sizeof(vertex_array), {3, 3, 2, 3});
	GLuint index_array[] =
	{
		0, 1, 2,  0, 2, 3,
		4, 5, 6,  4, 6, 7,
		8, 9, 10, 8, 10,11,
		12,13,14, 12,14,15,
		16,17,18, 16,18,19,
		20,21,22, 20,22,23,
	};

	vao.indexBuffer(index_array, sizeof(index_array), GL_TRIANGLES);
	size = 0.8;
	movingVelocity = 0.45;
	scale(size,size,size);

	roll(45);
	pitch(45);
	yawAngle = rnd(360);
	yaw(yawAngle);
	rotationSpeed = 3;
	place(position);
}

void Projectile::update()
{
	//posun ve směru pohybu projektilu
	position.x = position.x + direction.x * movingVelocity;
	position.z = position.z + direction.z * movingVelocity;
	place(position);
	yaw(yawAngle+=rotationSpeed);
}

void Projectile::explode()
{
	//ještě jsem neudělal
}
