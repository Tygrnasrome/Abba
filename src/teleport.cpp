#include "teleport.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Teleport::Teleport()
{
	loadFile("data/teleport.obj");
	scale(3,4,3);
}

float Teleport::distance(glm::vec3 p)
{
	//vrátí hodnotu vzdálenosti bodu od teleportu
	float a = sin(yawAng / 180.0 * M_PI);
	float b = -cos(yawAng / 180.0 * M_PI);
	float c = -b * position.z - a * position.x;
	return a * p.x + b * p.z + c;
}

void Teleport::update(Player* p)
{
	glm::vec3 dist = getPosition() - p->getPosition();
	if(sqrt(dist.x * dist.x + dist.z * dist.z) < 2)
	{
		float d1 = distance(prevP);
		float d2 = distance(p->getPosition());
		if(((d1 < 0 && d2 > 0) || (d1 > 0 && d2 < 0)) && permission == true)
		{
			p->setPosition(linked->getPosition());
			return;
		}
		permission = true;
		prevP = p->getPosition();
	}
	else
	{
		permission = false;
	}
}
