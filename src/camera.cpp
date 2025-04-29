#include "camera.h"
#include "shader.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

extern Shader *shader;

bool keyA;
Camera::Camera(){
	projection = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 1000.0f);
	position = glm::vec3(-10,1,0);
	up = glm::vec3(0,1,0);

	pitch = 0;
	yaw = 0;

	dx = cos(pitch) * cos(yaw);
	dy = sin(yaw);
	dz = sin(pitch) * cos(yaw);

	direction = glm::vec3(dx,dy,dz);
	view = glm::lookAt(position,position + direction,this->up);
	movingVelocity = normal = 0.25;
	nitro = 1;
	tiltVelocity = 0.001;


	keyW = false;
	keyA = false;
	keyS = false;
	keyD = false;
	keySpace = false;
	keyC = false;
}

//natočení kamery po pohybu myši
void Camera::mouseMotion(float xpos, float ypos){
	if(ypos >= 1930) ypos = 1930;
	if(ypos <= -1205) ypos = -1205;
	pitch = -(ypos - (720.0 / 2))/(1/tiltVelocity);
	yaw = (xpos - (1280.0 / 2))/(1/tiltVelocity);
	dx = cos(yaw) * cos(pitch);
	dy = sin(pitch);
	dz = sin(yaw) * cos(pitch);
	direction = glm::vec3(dx,dy,dz);
	this->lookAt(direction);
}

void Camera::key(int key, int action){

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

	if (key == GLFW_KEY_S && action == GLFW_PRESS){
		keyS = true;
	}

	if (key == GLFW_KEY_S && action == GLFW_RELEASE){
		keyS = false;
	}
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS){
		keySpace = true;
	}

	if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE){
		keySpace = false;
	}
	if (key == GLFW_KEY_C && action == GLFW_PRESS){
		keyC = true;
	}

	if (key == GLFW_KEY_C && action == GLFW_RELEASE){
		keyC = false;
	}
	if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS){
		keyShift = true;
	}

	if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE){
		keyShift = false;
	}

}

void Camera::resetKeys()
{
	keyW = false;
	keyA = false;
	keyS = false;
	keyD = false;
	keySpace = false;
	keyC = false;
	keyShift = false;
}

void Camera::move(){
	//kolmý vektor
	glm::vec3 c = glm::normalize(glm::cross(direction, up));

	if (keyShift)
		movingVelocity = nitro;
		else
		movingVelocity = normal;
	if (keyA){
		position = glm::vec3(position.x, position.y, position.z)- c * movingVelocity;
		this->lookAt(direction);
	}

	if (keyD){
		position = glm::vec3(position.x, position.y, position.z) + c * movingVelocity;
		this->lookAt(direction);
	}

	if (keyW){
		position.x = position.x + direction.x * movingVelocity;
		position.z = position.z + direction.z * movingVelocity;
		this->lookAt(direction);
	}

	if (keyS){
		position.x = position.x - direction.x * movingVelocity;
		position.z = position.z - direction.z * movingVelocity;
		this->lookAt(direction);
	}

	if (keySpace)
	{
		position.y = position.y + movingVelocity;
		this->lookAt(direction);
	}

	if (keyC)
	{
		position.y = position.y - movingVelocity;
		this->lookAt(direction);
	}

}

//natočení kamery na daný vektor
glm::mat4 Camera::lookAt(glm::vec3 direction){
	view = glm::lookAt(position,position + direction,this->up);
	return view;
}
