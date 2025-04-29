#ifndef BOX_H
#define BOX_H

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <GL/glew.h>

#include "shape.h"

class Box: public Shape
{
public:
	Box();
	Box(float x1, float y1, float z1, float x2, float y2, float z2);

};

#endif // BOX_H
