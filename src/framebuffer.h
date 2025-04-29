#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#include <GL/glew.h>
#include "texture.h"

#include "cubemap.h"
#include <array>

class Framebuffer
{
public:
	Framebuffer(int width, int height,GLenum format = GL_RGB, int colBufNum = 1, GLenum unit = GL_TEXTURE0);
	unsigned int fbo;
	unsigned int rbo;
	std::array<Texture*,2>texture;
	void bind();
	void unbind();
	void clear();
	void attach(CubeMap* cubemap, int side, int mipLevel);
	void resize(unsigned int width, unsigned int height);
	int colBufNum = 1;
};

#endif // FRAMEBUFFER_H
