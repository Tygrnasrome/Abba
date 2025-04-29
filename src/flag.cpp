#include "flag.h"

Flag::Flag()
{
	cylinder = new Cylinder(8);
	cloth = new Cloth(20);

	float h = 2;
	float r = 0.1;
	float flagL = 0.4;
	float flagH = 0.3;

	cylinder->scale(r,h,r);
	cloth->scale(flagL,flagH,flagL);

	cylinder->place(glm::vec3(0,0,0));
	cloth->place(glm::vec3(flagL -cylinder->getSize().z,h-flagH,0));

	add(cylinder);
	add(cloth);
	holded = false;
	type = rand() % flag_var + 1;
}
