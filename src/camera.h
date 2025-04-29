#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/ext.hpp>

class Camera{
public:
	Camera();
	void mouseMotion(float xpos, float ypos);
	void key(int key, int action);
	void move();
	void resetKeys();

	glm::mat4 projection;
	glm::mat4 view;
	glm::mat4 lookAt(glm::vec3 direction);
	glm::vec3 position;
	glm::vec3 up;
	glm::vec3 direction;

	float pitch;
	float yaw;
	float dx;
	float dy;
	float dz;

	float movingVelocity;
	float tiltVelocity;
	float nitro;
	float normal;

	bool keyW;
	bool keyA;
	bool keyS;
	bool keyD;
	bool keySpace;
	bool keyC;
	bool keyShift;
	bool freecam = true;

};

#endif // CAMERA_H
