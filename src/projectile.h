#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "shape.h"

class Projectile : public Shape
{
public:
	Projectile(glm::vec3 position, glm::vec3 direction, glm::vec3 color, float playerYaw);
	void update();
	void explode();
	glm::vec3 direction;
	glm::vec3 position;
	glm::vec3 color;
	float movingVelocity = 0.15;
	float size;
	float yawAngle;
	float rotationSpeed;
};

#endif // PROJECTILE_H
