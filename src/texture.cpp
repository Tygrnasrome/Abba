#include "texture.h"


Texture::Texture(const char* fileName, GLenum unit, GLint repeatMode)

{
	/*
	 *   Pro naši texturu potřebujeme vygenerovat IDčko, kterým se na ni budeme odvolávat. Protože si říkáme jen
	 *   o jednu, nemusíme předat pole.
	 */

	preffered_unit = unit;

	glActiveTexture(unit);
	glGenTextures(1, &texture);


	/* Abychom mohli s texturou pracovat v dalších voláních, označíme ji jako aktivní. */
	glBindTexture(GL_TEXTURE_2D, texture);

	/*
	 *   UV souřadnice mají základní rozsah od 0 do 1. Ale pokud použijeme větší hodnoty, textura se začne
	 *   opakovat. Zde můžeme určit režim, možnosti: GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER.
	 *   S a T odpovídá souřadnicím x a y, R by šlo do hloubky. Poslední z možností způsobí, že se textura
	 *   opakovat nebude, potom bychom měli ještě nastavit GL_TEXTURE_BORDER_COLOR na čtyřfloatové pole s barvou.
	 */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeatMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeatMode);

	/* Nastavíme filtr, který vyhledává pixely v textuře. Můžeme ho nastavit zvlášť při pohledu z dálky a z blízka. */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	/* Nahrajeme obrázek z disku do paměti. */
	int width, height, nrChannels;
	unsigned char *data = stbi_load(fileName, &width, &height, &nrChannels, 0);
	if(!data)
	{
		fprintf(stderr, "Cannot load file image %s\nSTB Reason: %s\n", fileName, stbi_failure_reason());
		exit(0);
	}

	/* Nahrajeme obrázek do textury v grafické kartě a vygenerujeme mipmapy. */


	glTexImage2D(GL_TEXTURE_2D, 0, nrChannels == 3 ? GL_RGB : nrChannels == 4 ? GL_RGBA : GL_RED, width, height, 0,
				       nrChannels == 3 ? GL_RGB : nrChannels == 4 ? GL_RGBA : GL_RED, GL_UNSIGNED_BYTE, data);


	//glGenerateMipmap(GL_TEXTURE_2D);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D,GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT,8.0f);
	stbi_image_free(data);
}

Texture::Texture(int width, int height, GLenum unit, GLenum format)
{
	preffered_unit = unit;
	glActiveTexture(unit);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0,
				       GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture::bind()
{
	glActiveTexture(preffered_unit);
	glBindTexture(GL_TEXTURE_2D, texture);
}
void Texture::resize(int width, int height)
{
	bind();
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
}
