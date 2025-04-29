#include "framebuffer.h"


Framebuffer::Framebuffer(int width, int height, GLenum format, int colBufNum, GLenum unit)
{

	this->colBufNum = colBufNum;
	//
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	for(int i = 0; i < colBufNum ; i++)
	{
		texture[i] = new Texture(width, height, unit + i, format);
		texture[i]->bind();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, texture[i]->texture, 0);
	}

	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	/* Necheme-li do něj hned kreslit. */
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/* Při renderování do mipmap se musí měnit velikost bufferu */
void Framebuffer::resize(unsigned int width, unsigned int height)
{
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

	for(unsigned i = 0; i < colBufNum; i++)
		texture[i]->resize(width, height);

	glViewport(0, 0, width, height);
}

/*
 *   Funkce attach se používá v případě kreslení do cubemapy, kdy je potřeba
 *   postupně přepínat, do které ze šesti textur a také do které úrovně mip
 *   mapy budeme nyní kreslit.
 */
void Framebuffer::attach(CubeMap *cubemap, int side, int mipLevel)
{
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + side, cubemap->texture, mipLevel);
}

void Framebuffer::bind(){
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	if(colBufNum > 1)
	{
		unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
		glDrawBuffers(colBufNum, attachments);
	}
}

void Framebuffer::unbind(){
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::clear()
{
	bind();
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}
