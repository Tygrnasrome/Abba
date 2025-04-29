#ifndef TEXT_H
#define TEXT_H
#include "shape.h"

class Text : public Shape
{
public:
	Text(char*text);
	void letter(char letter);
};

#endif // TEXT_H
