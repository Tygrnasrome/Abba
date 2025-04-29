#ifndef CUBEMAP_H
#define CUBEMAP_H

class CubeMap
{
public:
	unsigned int texture;
	CubeMap(GLenum unit = GL_TEXTURE0);
	CubeMap(unsigned int size, GLenum unit = GL_TEXTURE0, GLenum min_filter = GL_LINEAR_MIPMAP_LINEAR);
	void bind();
	GLenum prefered_unit;
};

#endif // CUBEMAP_H
