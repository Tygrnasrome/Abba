#ifndef TELEPORT_H
#define TELEPORT_H
class Player;
#include "player.h"
#include "objloader.h"



class Teleport : public ObjLoader
{
public:
	Teleport();
	Teleport* linked;
	glm::vec3 rgb;
	float distance(glm::vec3 p);
	void update(Player *p);
	glm::vec3 prevP;
	bool permission = false;
};

#endif // TELEPORT_H
