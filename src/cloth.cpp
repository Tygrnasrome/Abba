#include "cloth.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
Cloth::Point::Point(float x, float y, float z)
{
	previous = current = glm::vec3(x,y,z);
	inv_mass = 1;
}
Cloth::Constraint::Constraint(Point *p1, Point *p2)
{
	squared_rest_length = glm::length(p1->current-p2->current)*glm::length(p1->current-p2->current);
	this->p1 = p1;
	this->p2 = p2;
}

void Cloth::Constraint::satisfy()
{
	glm::vec3 delta = p2->current - p1->current;

	double d = glm::dot(delta, delta);
	double diff = (d - squared_rest_length) / ((squared_rest_length + d) * (p1->inv_mass + p2->inv_mass));

	if(p1->inv_mass != 0) p1->current += p1->inv_mass * diff * delta;
	if(p2->inv_mass != 0) p2->current -= p2->inv_mass * diff * delta;
}

Cloth::Cloth(int segments)
{
	maxPoints = segments + 1;
	points.resize(maxPoints);
	for(int i = 0; i < maxPoints; i++){
		points[i].resize(maxPoints);
	}
	generatePoints();
	generateStruct();
}

void Cloth::generatePoints()
{
	double spacing = 2.0 / (maxPoints - 1);
	double y = -1;
	//chceme vyplnit prostor od -1 do 1 a proto y a x násobím lineární fcí spacing
	for(int i = 0; i < maxPoints; i++){

		double x = -1;
		for(int j = 0; j<maxPoints;j++)
		{

			points[i][j] = new Point(x,y,0);
			x+= spacing;
			if(i > 0) constraints.push_back(new Constraint(points[i-1][j],points[i][j]));
			if(j > 0) constraints.push_back(new Constraint(points[i][j-1],points[i][j]));
		}
		y += spacing;
	}
	//upevnění bodů na straně
	points[0][0]->inv_mass = 0;
	points[maxPoints/2 - 1][0]->inv_mass = 0;
	points[maxPoints - 1][0]->inv_mass = 0;
	for(int j = 0; j<maxPoints;j++)
	{
		points[j][0]->inv_mass = 0;
	}

}

void Cloth::generateStruct()
{
	int segments = maxPoints-1;
	GLfloat va[17*maxPoints*maxPoints];
	GLuint ia[segments*segments*6];

	int n = 0, m = 0;
	for(int i = 0; i<maxPoints; i++)
	{
		for(int j = 0; j<maxPoints; j++)
		{
			va[n++] = points[i][j]->current.x;			//pos
			va[n++] = points[i][j]->current.y;			//pos
			va[n++] = points[i][j]->current.z;			//pos

			va[n++] = 0;	va[n++] = 1;	va[n++] = 0;		//color

			va[n++] = (float)j/segments;				//uv
			va[n++] = (float)i/segments;				//uv


			va[n++] = 0;						//normal
			va[n++] = 0;						//normal
			va[n++] = 1;						//normal

			va[n++] = 1;						//tangenta
			va[n++] = 0;						//tangenta
			va[n++] = 0;						//tangenta

			va[n++] = 0;						//bitangenta
			va[n++] = 1;						//bitangenta
			va[n++] = 0;						//bitangenta
		}
	}
	vao.vertexBuffer(va, sizeof(va), {3, 3, 2, 3, 3, 3});

	for (int i = 0; i < segments;i++)
	{
		for (int j = 0; j < segments;j++)
		{
			ia[m++] = i * (maxPoints) + j;
			ia[m++] = i * (maxPoints) + 1 + j;
			ia[m++] = (i  + 1) * (maxPoints) + j;

			ia[m++] = i * (maxPoints) + 1 + j;
			ia[m++] = (i  + 1) * (maxPoints) + 1 + j;
			ia[m++] = (i  + 1) * (maxPoints) + j;

		}
	}
	vao.indexBuffer(ia, sizeof(ia), GL_TRIANGLES);
}

void Cloth::update(float wind)
{
	glm::mat3 r = glm::rotate(glm::mat4(1.0f), wind* (float)M_PI, glm::vec3(0, 1, 0));
	for(int i = 0; i < maxPoints; i++)
		for(int j = 0; j < maxPoints; j++)
		{
			points[i][j]->move(r * glm::vec3(-rnd(1), rnd(0.2) - 0.6, rnd(8) - 4) * 0.0025);

		}

	for(int j = 0; j < 2; j++)
	{
		for(auto i: constraints)
			i->satisfy();
	}
}

void Cloth::Point::move(glm::vec3 force)
{
	if (inv_mass)
	{
		glm::vec3 newPos = (current*1.99)-(previous*0.99) + force;
		this->previous = this->current;
		this->current = newPos;
	}
}

