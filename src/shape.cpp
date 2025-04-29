#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "shape.h"

Shape::Shape()
{
	parent = nullptr;
}
float Shape::rnd(float max)
{
	return rand()/(float)RAND_MAX * max;
}
void Shape::place(glm::vec3 position)
{
	this->position = position;
}

/*	tato funkce vytvoří a vrátí modelovou matici
 *	bude v ní obsaženo umístění, škálování a rotaci		*/
glm::mat4 Shape::getMatrix()
{
	/* Modelová matice umístí objekt ze svých lokálních souřadnic s nulou uprostřed do světa. */
	glm::mat4 model = glm::mat4();
	model = glm::translate(model, position);
	model = glm::rotate(model, glm::radians(float(yawAng)), glm::vec3(0,1,0));
	model = glm::rotate(model, glm::radians(float(pitchAng)), glm::vec3(1,0,0));

	model = glm::rotate(model, glm::radians(float(rollAng)), glm::vec3(0,0,1));
	model = glm::scale(model, glm::vec3(Xscale, Yscale, Zscale));

	return parent ? parent->getMatrix() * model : model;
}
glm::vec3 Shape::getSize()
{
	glm::vec3 size = glm::vec3(Xscale, Yscale, Zscale);
	return size;
}
void Shape::setPosition(glm::vec3 pos)
{
	this->position = pos;
}
/*	škálování (použije se až v modelové matici)		*/
void Shape::scale(float Xscale, float Yscale, float Zscale)
{
	this->Xscale = Xscale;
	this->Zscale = Zscale;
	this->Yscale = Yscale;
}
/*	rotace (použije se až v modelové matici)
 *	úhel o který se kvádr/krychle otočí okolo osy, která
 *	se určí pitch (vektor osy x)
*/
void Shape::pitch(float angle)
{
	this->pitchAng = angle;
}
void Shape::yaw(float angle)
{
	this->yawAng = angle;
}
float Shape::yaw()
{
	return this->yawAng;
}
void Shape::roll(float angle)
{
	this->rollAng = angle;
}
/*	vykreslí kvádr/krychli					*/
void Shape::draw(Shader *shader, Camera *camera)
{
	shader->use();
	if(camera)
		shader->uniform("view",camera->view);

	//specifikace materialu
	shader->uniform("model", getMatrix());
	if(camera)
		shader->uniform("projection",camera->projection);

	vao.draw();
}
void Shape::draw(Shader *shader)
{
	shader->use();
	vao.draw();
}

void Shape::drawInstanced(Shader *shader, Camera *camera, int copies)
{
	shader->use();

	shader->uniform("view",camera->view);


	//specifikace materialu
	shader->uniform("model", getMatrix());

	shader->uniform("projection",camera->projection);

	vao.drawInstanced(copies);
}

AttribArray Shape::getVao()
{
	return vao;
}
glm::vec3 Shape::getPosition(){
	return position;
}


OBB Shape::getOBB()
{
	glm::mat3 m;
	OBB obb;
	m = getMatrix();
	obb.size = glm::vec3(glm::length(m[0]), glm::length(m[1]), glm::length(m[2]));
	obb.pos = getMatrix() * glm::vec4(0,0,0, 1);
	obb.axisX = glm::normalize(m * glm::vec3(1,0,0));
	obb.axisY = glm::normalize(m * glm::vec3(0,1,0));
	obb.axisZ = glm::normalize(m * glm::vec3(0,0,1));
	return obb;
}
OBP Shape::getOBP()
{
	glm::mat3 m;
	OBP obp;
	m = getMatrix();
	obp.size = glm::vec3(glm::length(m[0]), glm::length(m[1]), glm::length(m[2]));
	obp.pos = getMatrix() * glm::vec4(0,0,0, 1);
	obp.axisX = glm::normalize(m * glm::vec3(1,0,0));
	obp.axisY = glm::normalize(m * glm::vec3(0,1,0));
	obp.axisZ = glm::normalize(m * glm::vec3(0,0,1));
	obp.axisN = glm::normalize(m * glm::vec3(0,1,2));
	obp.axisW = glm::normalize(m * glm::vec3(-2,1,0));
	obp.axisS = glm::normalize(m * glm::vec3(0,1,-2));
	obp.axisE = glm::normalize(m * glm::vec3(2,1,0));
	return obp;

}
