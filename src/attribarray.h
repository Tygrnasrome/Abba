#ifndef ATTRIBARRAY_H
#define ATTRIBARRAY_H

#include <vector>
#include <GL/glew.h>

class AttribArray
{
public:
	int drawingMode;
	int num_of_elements;
	GLuint vertex_array_object = 0, element_buffer_object = 0, vertex_buffer_object = 0;
	AttribArray();

	void vertexBuffer(GLfloat* vertex_array, int vertex_arraySize, std::vector<int> attribs);
	void indexBuffer(GLuint* index_array, int index_arraySize, int mode);
	void draw();
	void drawInstanced(int copies);
};

#endif // ATTRIBARRAY_H
