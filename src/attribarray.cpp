#include "attribarray.h"

AttribArray::AttribArray()
{
}

void AttribArray::vertexBuffer(GLfloat *vertex_array, int vertex_arraySize, std::vector <int> attribs){

	/* Vytvoříme VAO objekt sdružující buffery a konfiguraci atributů. */
	       if(vertex_buffer_object)
		       glDeleteBuffers(1, &vertex_buffer_object);
	       if(element_buffer_object)
		       glDeleteBuffers(1, &element_buffer_object);
	       if(vertex_array_object)
		       glDeleteVertexArrays(1, &vertex_array_object);
	glGenVertexArrays(1, &vertex_array_object);

	/* Následující příkazy budou pracovat s tímto objektem. */
	glBindVertexArray(vertex_array_object);

	int dislocation = 0;
	int strida = 0;

	//this->vertexArray = ElementsNumber;
	for(auto it: attribs) strida += it;

	/* Vytvoříme vertex buffer pro data vrcholů. */

	glGenBuffers(1, &vertex_buffer_object);

	/* Následující příkazy budou pracovat s tímto bufferem. */
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);

	/* Nahrajeme data do paměti grafické karty, v programu pak mohou zaniknout. */
	glBufferData(GL_ARRAY_BUFFER, vertex_arraySize, vertex_array, GL_STATIC_DRAW);


	for (int i = 0; i < attribs.size(); ++i) {
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(
			i,                                  // Číslo atributu dle shaderu
			attribs[i],                     // Počet hodnot
			GL_FLOAT,                           // Typ proměnné
			GL_FALSE,                           // Normalizováno?
			strida * sizeof(GLfloat),           // Střída
			(void*)dislocation                  // Posun od začátku bufferu
		);
		dislocation += attribs[i] * sizeof(GLfloat);
	}
}

void AttribArray::indexBuffer(GLuint* index_array, int index_arraySize, int mode){
	drawingMode = mode;
	num_of_elements = index_arraySize/4;


	/* Vytvoříme element buffer pro pole indexů. */

	glGenBuffers(1, &element_buffer_object);

	/* Následující příkazy budou pracovat s tímto bufferem. */
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object);

	/* Nahrajeme data do paměti grafické karty, v programu pak mohou zaniknout. */
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_arraySize, index_array, GL_STATIC_DRAW);
}

void AttribArray::draw(){
	glBindVertexArray(vertex_array_object);
	/*	nabindování vertex_array_object (VAO)
	 *	pro následující příkaz				*/

	glDrawElements(drawingMode, num_of_elements, GL_UNSIGNED_INT, 0);
	/*	GL_TRIANGLES - protože ve vertex_array_object jsou stěny ve formátu trojúhelníků
	 *	36 - toto je počet trojúhelníků tj. sizeof(index_array)
	 *	0 - protože používáme nabindovaný VAO a tímpádem nevyužíváme pointer */

}

void AttribArray::drawInstanced(int copies){
	glBindVertexArray(vertex_array_object);
	/*	nabindování vertex_array_object (VAO)
	 *	pro následující příkaz				*/

	glDrawElementsInstanced(drawingMode, num_of_elements, GL_UNSIGNED_INT, 0, copies);
	/*	GL_TRIANGLES - protože ve vertex_array_object jsou stěny ve formátu trojúhelníků
	 *	36 - toto je počet trojúhelníků tj. sizeof(index_array)
	 *	0 - protože používáme nabindovaný VAO a tímpádem nevyužíváme pointer */

}
