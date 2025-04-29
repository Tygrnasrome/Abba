#ifndef GROUP_H
#define GROUP_H

#include <list>

#include "shape.h"

class Group: public Shape
{
public:
	Group();
	void add(Shape *object);
	std::list<Shape*> objects;
	void draw(Shader *shader, Camera *camera);
};

#endif // GROUP_H
