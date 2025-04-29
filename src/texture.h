#ifndef TEXTURE_H
#define TEXTURE_H
#include "stb_image.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Texture
{
public:
	GLenum internalFormat;

	Texture(const char *fileName,GLenum unit = GL_TEXTURE0, GLint repeatMode = GL_REPEAT);
	Texture(int width, int height, GLenum unit, GLenum format = GL_RGB);

	void bind();
	void resize(int width, int height);
	unsigned int texture;
	GLenum preffered_unit;
};

#endif // TEXTURE_H
