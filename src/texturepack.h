#ifndef TEXTUREPACK_H
#define TEXTUREPACK_H

#include "texture.h"
#include <vector>
class TexturePack
{
public:
	TexturePack(std::vector<const char *> textures);
	std::vector<Texture*> textures;
	void bind(int num);
	int num;

};

#endif // TEXTUREPACK_H
