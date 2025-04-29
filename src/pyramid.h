#ifndef PIRAMID_H
#define PIRAMID_H

#include "shape.h"

class Pyramid: public Shape
{
public:
	Pyramid();
	void draw(Shader *shader, Camera *camera);
	glm::vec2 seed;
};

#endif // PIRAMID_H
