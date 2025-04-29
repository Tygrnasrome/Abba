 #include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "player.h"

int a = 1;

Player::Player(glm::vec3 color)
{
	belt = new Box();
	turret = new Box();
	barrel = new Box();

	top = new Group();

	this->color = color;


	float belt_w = 1;
	float belt_h = 0.35;
	float belt_lenght = 0.6;


	float turret_w = 0.4;
	float turret_h = 0.25;
	float turret_shift_lenght = -0.3;
	float turret_lenght = 0.4;


	float barrel_w = 0.08;
	float barrel_h = 0.08;
	float barrel_shift_h = 0.08;
	float barrel_lenght = 0.65;



	belt->scale(belt_w, belt_h, belt_lenght);

	barrel->scale(barrel_w, barrel_h, barrel_lenght);
	turret->scale(turret_w, turret_h, turret_lenght);

	//turret->scale(turret_w, turret_h, turret_lenght);
	turret->place(glm::vec3(0,belt_h+turret_h,turret_shift_lenght));

	//barrel->scale(barrel_w, barrel_h, barrel_lenght);
	barrel->place(glm::vec3(0,belt_h+barrel_shift_h+turret_h,turret_shift_lenght + turret_lenght+ barrel_lenght));

	this->add(belt);
	top->add(turret);
	top->add(barrel);
	this->add(top);

	normal = movingVelocityX = 0.35;
	baseTurnSpeed = turningSpeed = 2.25;
	nitroTurnSpeed = 5;
	movingVelocityY = 1.25;
	tiltVelocity = 0.001;
	nitro = 3.5*normal;

	up = glm::vec3(0,1,0);
	resetKeys();

	inAir = false;

	ay = -0.035;
	beltYaw = 0;
	belt->yaw(beltYaw);
	position.y = position.y-5.5;
	grabCam = false;
	holdingFlag = 0;
}

void Player::mouseMotion(float xpos, float ypos)
{
	if(ypos >= 1930) ypos = 1930;
	if(ypos <= -1205) ypos = -1205;
	pitch = -(ypos - (720.0 / 2))/(1/tiltVelocity);
	yaw((xpos - (1280.0 / 2))/(1/tiltVelocity));
	dx = cos(yaw() * cos(pitch));
	dy = sin(pitch);
	dz = sin(yaw()) * cos(pitch);
	direction = glm::vec3(dx,dy,dz);
	top->yaw(-yaw()/3.14*180+90);
}

void Player::key(int key, int action)
{

	if (key == GLFW_KEY_A && action == GLFW_PRESS){
		keyA = true;

	}
	if (key == GLFW_KEY_A && action == GLFW_RELEASE){
		keyA = false;
	}

	if (key == GLFW_KEY_D && action == GLFW_PRESS){
		keyD = true;
	}

	if (key == GLFW_KEY_D && action == GLFW_RELEASE){
		keyD = false;
	}

	if (key == GLFW_KEY_W && action == GLFW_PRESS){
		keyW = true;
	}

	if (key == GLFW_KEY_W && action == GLFW_RELEASE){
		keyW = false;
	}

	place(glm::vec3(position.x,position.y,position.z));

	if (key == GLFW_KEY_S && action == GLFW_PRESS){
		keyS = true;
	}

	if (key == GLFW_KEY_S && action == GLFW_RELEASE){
		keyS = false;
	}
	if(key == GLFW_KEY_SPACE && action == GLFW_PRESS && inAir == false){
		inAir = true;
		holdingFlag == FLAG_NO_JUMP ? vy = movingVelocityY/4 : holdingFlag == FLAG_SLOW_FALL ? vy = movingVelocityY/2 : vy = movingVelocityY;;

	}
	if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS){
		keyShift = true;
	}

	if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE){
		keyShift = false;
	}

}

void Player::resetKeys()
{
	keyW = false;
	keyA = false;
	keyS = false;
	keyD = false;
	keyShift = false;
}


void Player::update(World *world, Camera* camera)
{
//	printf("\n\nframe %d inair %d", a, inAir);
//	printf("\npred move\nposition x = %f\nposition y = %f\nposition z = %f\n", position.x,position.y,position.z);

	float yBefore = position.y;

	if(!camera->freecam){
		this->move();
	}

	{
//	printf("\npo move\nposition x = %f\nposition y = %f\nposition z = %f\n", position.x,position.y,position.z);

	place(glm::vec3(position.x,position.y,position.z));

	keyShift == true ? movingVelocityX = nitro : movingVelocityX = normal;
	}


	{
	//flags ->
	holdingFlag == FLAG_FAST_ROT ? turningSpeed = nitroTurnSpeed : turningSpeed = baseTurnSpeed;

	if(holdingFlag == FLAG_NITRO)
		movingVelocityX += movingVelocityX;
	if(holdingFlag == FLAG_SLOW)
		movingVelocityX = normal/2;
	}
	// <- flags

	//tests if player landed on box/pyramid
//	printf("pos bud %f size bud %f", getOBB().pos.y, getOBB().size.y);
	for(auto it = world->arrayBox.begin(); it != world->arrayBox.end(); it++)
	{
		if(world->collision((*it)->getOBB(),getOBB())){
//			printf("MTV.y %f pos bud %f size bud %f", world->getMTV((*it)->getOBB(),getOBB()).y, getOBB().pos.y, getOBB().size.y);
			place(position += world->getMTV((*it)->getOBB(),getOBB()));
		}
	}
	for(auto it = world->arrayTpBox.begin(); it != world->arrayTpBox.end(); it++)
	{
		if(world->collision((*it)->getOBB(),getOBB())){
//			printf("MTV.y %f pos bud %f size bud %f", world->getMTV((*it)->getOBB(),getOBB()).y, getOBB().pos.y, getOBB().size.y);
			place(position += world->getMTV((*it)->getOBB(),getOBB()));
		}
	}
	for(auto it = world->arrayPyramid.begin(); it != world->arrayPyramid.end(); it++)
	{
		if(world->collision((*it)->getOBP(),getOBB()))
			place(position += world->getMTV((*it)->getOBP(),getOBB()));
	}

	//test if player landed on the ground
	if(position.y - belt->getSize().y < 0)
	{
		printf("inair -> false\n");
		inAir = false;
		place(glm::vec3(this->position.x,belt->getSize().y,this->position.z));
	}

	if(inAir == true && yBefore == position.y){
		printf("inair -> false2\n");
		inAir = false;
		vy = 0;
	}
	if(inAir == false && yBefore != position.y){
		printf("inair -> true2\n");
		inAir = true;
	}

	if(grabCam == true)
	{
		camera->direction = direction;
		camera->position = position + glm::vec3(0,1.5,0) - direction * 4;
		camera->lookAt(direction);
	}

}

void Player::move(){
	//kolmý vektor

	beltDirection = glm::vec3(cos(-beltYaw * 3.14 / 180.0 /*+ 1.57*/), 0, sin(-beltYaw * 3.14 / 180.0/* + 1.57*/));

	if ((keyA && holdingFlag != FLAG_INVERTED) || (keyD && holdingFlag == FLAG_INVERTED)){
		belt->yaw(beltYaw += turningSpeed);
	}

	if ((keyD && holdingFlag != FLAG_INVERTED) || (keyA && holdingFlag == FLAG_INVERTED)){
		belt->yaw(beltYaw += -turningSpeed);
	}

	if ((keyW && holdingFlag != FLAG_INVERTED) || (keyS && holdingFlag == FLAG_INVERTED)){
		position.x = position.x + beltDirection.x * movingVelocityX;
		position.z = position.z + beltDirection.z * movingVelocityX;
	}

	if ((keyS && holdingFlag != FLAG_INVERTED) || (keyW && holdingFlag == FLAG_INVERTED)){
		position.x = position.x - beltDirection.x * movingVelocityX;
		position.z = position.z - beltDirection.z * movingVelocityX;
	}



	//move player verically
	if(inAir)
	{
		printf("vy %f \n",vy);
		holdingFlag == FLAG_NO_JUMP ? vy += ay*4 : holdingFlag == FLAG_SLOW_FALL ? vy += ay/4 : vy += ay;
		position.y = position.y + vy;

	}
	if(!inAir && position.y - belt->getSize().y > 0){
		position.y = position.y - 0.01;
	}
	place(glm::vec3(this->position.x,this->position.y,this->position.z));
}
/*
glm::mat4 Player::getMatrix()
{
	/* Modelová matice umístí objekt ze svých lokálních souřadnic s nulou uprostřed do světa. */
/*	glm::mat4 model = glm::mat4();
	model = glm::translate(model, position);
	model = glm::scale(model, glm::vec3(Xscale, Yscale, Zscale));
	model = glm::rotate(model, glm::radians(float(90)), glm::vec3(0, 1, 0));
	model = glm::rotate(model, -yaw, glm::vec3(0, 1, 0));

	return model;
}
*/

/*void Player::place(glm::vec3 position)
{
	this->position = position;
	this->place(position);
}*/

//void Player::draw(Shader *shader, Camera *camera)
//{
//	this->draw(shader,camera);
//}

