#ifndef OBJLOADER_H
#define OBJLOADER_H

#include "shape.h"

class ObjLoader: public Shape
{
public:
	ObjLoader();
	void loadFile(const char *filename);
};

#endif // OBJLOADER_H
