#ifndef SHAPE_H
#define SHAPE_H

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "shader.h"
#include "camera.h"
#include "attribarray.h"

struct OBB
{
	glm::vec3 size;
	glm::vec3 pos;
	glm::vec3 axisX;
	glm::vec3 axisY;
	glm::vec3 axisZ;
};

struct OBP
{
	glm::vec3 size;
	glm::vec3 pos;
	glm::vec3 axisX;
	glm::vec3 axisY;
	glm::vec3 axisZ;
	glm::vec3 axisN;
	glm::vec3 axisW;
	glm::vec3 axisS;
	glm::vec3 axisE;
};

class Shape
{
public:
	Shape *parent;
	Shape();
	virtual void draw(Shader *shader, Camera *camera);
	virtual void draw(Shader *shader);
	virtual void drawInstanced(Shader *shader, Camera *camera, int copies);
	virtual void place(glm::vec3 position);
	virtual void scale(float Xscale, float Yscale, float Zscale);
	virtual void pitch(float angle);
	virtual void yaw(float angle);
	virtual float yaw();
	virtual AttribArray getVao();
	virtual void roll(float angle);
	GLuint vertex_array_object;
	glm::vec3 getSize();
	glm::vec3 getPosition();
	void setPosition(glm::vec3 pos);
	virtual glm::mat4 getMatrix();

	OBB getOBB();
	OBP getOBP();

protected:
	float rnd(float max);
	float y = 0;
	float x = 0;
	float z = 0;
	float w = 1;
	float h = 1;
	float l = 1;

	glm::vec3 position;

	float angle = 0;
	float Xscale = 1;
	float Yscale = 1;
	float Zscale = 1;
	float pitchAng = 0;
	float yawAng = 0;
	float rollAng = 0;
	int num_of_elements = 36;
	AttribArray vao;
};

#endif // SHAPE_H
