#ifndef HUD_H
#define HUD_H

#include "camera.h"

class HUD : public Camera
{
public:
	HUD();
	void update(glm::vec3 up, glm::vec3 playerPos);
	void switchProjections();
	glm::vec3 shiftY;
	glm::mat4 baseProjection;
	glm::mat4 maxProjection;

};

#endif // HUD_H
