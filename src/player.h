#ifndef PLAYER_H
#define PLAYER_H

#include "shape.h"
#include "group.h"
#include "world.h"

class World;


class Player: public Group
{
public:
	Player(glm::vec3 color);
	void mouseMotion(float xpos, float ypos);
	void key(int key, int action);
	void move();
	void update(World *world,Camera *camera);
	//void place(glm::vec3);
	//void draw(Shader *shader, Camera *camera);
	void resetKeys();

	//glm::mat4 getMatrix() { return tank->getMatrix(); }
	glm::mat4 view;
	glm::vec3 direction, beltDirection;
	glm::vec3 up;

	Group *top;
	glm::vec3 color;
	Box *barrel, *belt, *turret;

	typedef enum
	{
		STANDING,
		JUMPING,
		FALLING,
		ON_BOX
	} State;
	State state;

	bool grabCam;

	float movingVelocityX;
	float movingVelocityY;
	float tiltVelocity;
	float nitro;
	float normal;
	float turningSpeed, baseTurnSpeed, nitroTurnSpeed;

	bool keyW;
	bool keyA;
	bool keyS;
	bool keyD;
	bool keyShift;

	float pitch;
	float beltYaw;
	float dx;
	float dy;
	float dz;

	float vy;
	float ay;

	bool inAir;

	int holdingFlag;

};

#endif // PLAYER_H
