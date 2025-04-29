#ifndef FLAG_H
#define FLAG_H

#include "cylinder.h"
#include "cloth.h"
#include "group.h"

//typy flag
//flag_var je počet typů flag
#define flag_var 9
#define FLAG_NITRO 1
#define FLAG_NO_JUMP 2
#define FLAG_NO_SHOOT 3
#define FLAG_SLOW 4
#define FLAG_INVERTED 5
#define FLAG_FAST_ROT 6
#define FLAG_SLOW_FALL 7
#define FLAG_BLUR 8
#define FLAG_GREYSCALE 9


class Flag : public Group
{
public:
	Flag();
	Cylinder* cylinder;
	Cloth* cloth;
	bool holded;
	int type;
};

#endif // FLAG_H
