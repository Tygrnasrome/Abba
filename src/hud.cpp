#include "hud.h"

HUD::HUD()
{
	//ortogonální (pravoúhlá) projekce
	projection =  baseProjection = glm::ortho(-100.f * 1280.0f / 720.0f, 600.f * 1280.0f / 720.0f, -200.f, 500.f, 0.1f, 200.0f);
	maxProjection = glm::ortho(-200.f * 1280.0f / 720.0f, 200.f * 1280.0f / 720.0f, -200.f, 200.f, 0.1f, 200.0f); // ještě nefunguje
	//asi zbytečné dva řádky
	shiftY = glm::vec3(0,20,0);
	up = glm::vec3(1,0,0);
}

void HUD::update(glm::vec3 up, glm::vec3 playerPos)
{
	this->up = up;
	position = glm::vec3(0,0,0) + shiftY;
	view = glm::lookAt(position, glm::vec3(0,0,0), glm::vec3(0,0,1));
}
void HUD::switchProjections()
{
	if (projection == baseProjection) projection = maxProjection;
	else projection = baseProjection;
}
