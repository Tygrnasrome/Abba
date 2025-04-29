#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "cubemap.h"
#include "stb_image.h"

CubeMap::CubeMap(GLenum unit)
{

	/*
     *   Pro naši texturu potřebujeme vygenerovat IDčko, kterým se na ni budeme odvolávat. Protože si říkáme jen
     *   o jednu, nemusíme předat pole.
     */
	glGenTextures(1, &texture);
	prefered_unit = unit;
	/* Abychom mohli s texturou pracovat v dalších voláních, označíme ji jako aktivní. */
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

	/*
     *   UV souřadnice mají základní rozsah od 0 do 1. Ale pokud použijeme větší hodnoty, textura se začne
     *   opakovat. Zde můžeme určit režim, možnosti: GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER.
     *   S a T odpovídá souřadnicím x a y, R by šlo do hloubky. Poslední z možností způsobí, že se textura
     *   opakovat nebude, potom bychom měli ještě nastavit GL_TEXTURE_BORDER_COLOR na čtyřfloatové pole s barvou.
     */
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	/* Nastavíme filtr, který vyhledává pixely v textuře. Můžeme ho nastavit zvlášť při pohledu z dálky a z blízka. */
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	/* Nahrajeme obrázek z disku do paměti. */
	int width, height, nrChannels;
	char path[50];
	for (int i = 0; i < 6; ++i) {
		sprintf(path, "data/skybox/%d.jpg",i);
		unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
		if(!data)
		{
			fprintf(stderr, "Cannot load file image %s\nSTB Reason: %s\n", "data/skybox/%d.jpg", stbi_failure_reason());
			exit(0);
		}

		/* Nahrajeme obrázek do textury v grafické kartě a vygenerujeme mipmapy. */
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		//glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
	}

}

CubeMap::CubeMap(unsigned int size, GLenum unit, GLenum min_filter)
{
	glActiveTexture(unit);
	prefered_unit = unit;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	for(unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, size, size, 0, GL_RGB, GL_FLOAT, nullptr);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	/* Min filter je parametrizovaný, protože pro irradiance mapu musí být GL_LINEAR. */
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, min_filter);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	/* Generování mipmap je trik, který přiměje OpenGL alokavat paměti, i když jsme nepředali obrázek. */
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
}

void CubeMap::bind()
{
	glActiveTexture(prefered_unit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
}
