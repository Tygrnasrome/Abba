#include "texturepack.h"

TexturePack::TexturePack(std::vector<const char *> textures)
{
	int unit = 0;
	for(auto it = textures.begin(); it != textures.end(); it++)
	{
		Texture *tmp = new Texture(*it, GL_TEXTURE0 + unit);
		this->textures.push_back(tmp);
		unit++;
	}
	num = unit;

}
void TexturePack::bind(int num)
{
	int i = 0;
	for(auto it = textures.begin(); it != textures.end(); it++)
	{
		if(i==num)
			(*it)->bind();
		i++;
	}
}
