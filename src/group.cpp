#include "group.h"

Group::Group()
{

}

void Group::add(Shape *object)
{
	object->parent = this;
	objects.push_back(object);
}

void Group::draw(Shader *shader, Camera *camera)
{
	for(auto &it: objects) it->draw(shader, camera);
}
