#ifndef SHADER_H
#define SHADER_H
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>
#include "texture.h"
#include "texturepack.h"
#include "cubemap.h"

class Shader
{
public:
	Shader(const char *vert, const char *frag);
	void uniformInt (const char* name, int x);
	void uniform (const char* name, float x);
	void uniform (const char* name, float r,float g,float b);
	void uniform (const char* name,glm::vec3 values);
	void uniform (const char* name,glm::vec2 values);
	void uniform (const char* name, glm::mat4 x);
	void uniformMatrix(const char* name, const glm::mat4 &matrix);

	void use();
	void texture(const char* name, Texture* texName);
	void texture(const char* name, CubeMap* texture);
	void texturePack(std::vector<const char*> name, TexturePack *texpaxName);
	GLuint shader_program;

};

#endif // SHADER_H
