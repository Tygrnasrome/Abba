#include "shader.h"
#include <stdio.h>
#include <iostream>
using namespace std;

Shader::Shader(const char* vert, const char* frag){
	FILE * vertF;
	FILE * fragF;

	vertF = fopen (vert,"rb");
	fragF = fopen (frag,"rb");
	if(vertF == NULL || fragF == NULL){

		if(fragF == NULL){
			printf("soubor nebyl nalezen %s\n", frag);
		}
		if(vertF == NULL){
			printf("soubor nebyl nalezen %s\n", vert);
		}
		exit(1);
	}

	//přesunutí na konec souboru
	fseek(vertF,0,SEEK_END);
	fseek(fragF,0,SEEK_END);
	//určení velikosti souboru
	int sizeVert = ftell(vertF);
	int sizeFrag = ftell(fragF);
	//vrácení se na začátek
	rewind(vertF);
	rewind(fragF);

	char* vertex_shader_source = new char[sizeVert+1];
	char* fragment_shader_source = new char[sizeFrag+1];

	fread(vertex_shader_source,1,sizeVert,vertF);
	fread(fragment_shader_source,1,sizeFrag,fragF);
	vertex_shader_source[sizeVert] = 0;
	fragment_shader_source[sizeFrag] = 0;

	fclose (vertF);
	fclose (fragF);


	int res;
	printf("compiling %s\n",vert);

	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
	glCompileShader(vertex_shader);

	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &res);
	if(res == 0)
	{
		int len;
		char msg[1024];
		glGetShaderInfoLog(vertex_shader, 1024, &len, msg);

		cout << "Vertex shader compilation error:" << endl;
		printf("%s",vertex_shader_source);
		cout << msg << endl;
		exit(0);
	}


	printf("compiling %s\n",frag);

	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
	glCompileShader(fragment_shader);

	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &res);
	if(res == 0)
	{
		int len;
		char msg[1024];
		glGetShaderInfoLog(fragment_shader, 1024, &len, msg);

		cout << "Shader compilation error:" << endl;
		printf("%s",fragment_shader_source);
		cout << msg << endl;
		exit(0);
	}




	shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	glGetProgramiv(shader_program, GL_LINK_STATUS, &res);
	if(res == 0)
	{
		int len;
		char msg[1024];
		glGetProgramInfoLog(shader_program, 1024, &len, msg);

		cout << "Shader linking error:" << endl;
		cout << msg << endl;
		exit(0);
	}


	glValidateProgram(shader_program);
	glGetProgramiv(shader_program, GL_LINK_STATUS, &res);

	if(res == 0)
	{
		int len;
		char msg[1024];
		glGetProgramInfoLog(shader_program, 1024, &len, msg);

		cout << "Shader validation error:" << endl;
		cout << msg << endl;
		exit(0);
	}

}

void Shader::uniform (const char* name, float x)
{
	unsigned int location;
	location = glGetUniformLocation(shader_program, name);

	glUniform1f(location, x);
}

void Shader::uniformInt (const char* name, int x)
{
	unsigned int location;
	location = glGetUniformLocation(shader_program, name);

	glUniform1i(location, x);
}

void Shader::uniform (const char* name, float r,float g,float b)
{
	unsigned int uniformLocation;
	uniformLocation = glGetUniformLocation(shader_program, name);

	glUniform3f(uniformLocation, r, g, b);

}

void Shader::uniform (const char* name, glm::vec3 values)
{
	unsigned int vector;
	vector = glGetUniformLocation(shader_program, name);

	glUniform3f(vector, values.x, values.y, values.z);

}

void Shader::uniform (const char* name, glm::vec2 values)
{
	unsigned int vector;
	vector = glGetUniformLocation(shader_program, name);

	glUniform2f(vector, values.x, values.y);

}

void Shader::uniform (const char* name, glm::mat4 x)
{
	GLint loc = glGetUniformLocation(shader_program, name);
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(x));
}

void Shader::uniformMatrix(const char* name, const glm::mat4 &matrix)
{
	glUniformMatrix4fv(glGetUniformLocation(shader_program, name), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::use()
{
	glUseProgram(shader_program);
}

void Shader::texture(const char* name, Texture* texName)
{
	texName->bind();
	glUniform1i(glGetUniformLocation(shader_program, name), texName->preffered_unit - GL_TEXTURE0);
}

void Shader::texture(const char* name, CubeMap* texture)
{
	texture->bind();
	glUniform1i(glGetUniformLocation(shader_program, name), texture->prefered_unit - GL_TEXTURE0);
}

void Shader::texturePack(std::vector<const char *> name, TexturePack* texpaxName)
{
	for (int i = 0; i < texpaxName->num; ++i)
	{
		texpaxName->bind(i);
		int tmp = 0;
		for(auto it = name.begin(); it != name.end(); it++)
		{
			if(i==tmp)
				glUniform1i(glGetUniformLocation(shader_program, *it), i);
			tmp++;
		}
	}

}













