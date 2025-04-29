#ifndef WORLD_H
#define WORLD_H

#include <math.h>
#include <array>
#include <list>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "camera.h"
#include "hud.h"
#include "framebuffer.h"

#include "texture.h"
#include "texturepack.h"
#include "cubemap.h"

#include "pyramid.h"
#include "quad.h"
#include "box.h"

#include "cylinder.h"
#include "flag.h"
#include "player.h"
#include "teleport.h"
#include "projectile.h"
#include "cloth.h"


class Player;
class Teleport;
//
#define teleportNum 14
#define flagNum 10
#define pyramidNum 10
#define boxNum 10

#define playerNum 1	//pokud je jiné než nula, tak se kreslí hráč
#define rndObjNum 0	//pokud je jiné než nula, tak se kreslí ostatní objekty

class World
{
public:
	World();

	void draw(double dt);

	std::array<Box*,boxNum>arrayBox;
	std::array<Group*,teleportNum>arrayTpBB;
	std::array<Box*,teleportNum*3>arrayTpBox;
	std::array<Pyramid*,pyramidNum>arrayPyramid;
	std::array<Quad*,4>arrayQuads;
	std::array<Teleport*,teleportNum>arrayTpFrames;
	std::array<Quad*,teleportNum>arrayTpPortals;
//std::array<Cylinder*,cylinderNum>arrayCylinder;
	std::array<Flag*,flagNum>arrayFlag;
	Box *lightSource;
	Shader *shading_shader, *sky_shader, *alfa_shader, *instancing_shader, *noise_shader, *crate_shader, *normal_shader, *height_shader, *monochromatic_shader, *snoice_shader, *pbr_shader, *hud_shader;
	Player *player;
	Group *piramids, *boxes, *walls;
	float angle = 0;

	float getSep(OBB *a, OBB *b, glm::vec3 sepAxis);
	glm::vec3 getMTV(const OBB &a, const OBB &b);
	glm::vec3 getMTV(const OBP &a, const OBB &b);
	bool testCollision(glm::vec3 pos_A, glm::vec3 size_A, glm::vec3 pos_B, glm::vec3 size_B);
	CubeMap *sky;
	Texture *texBrick, *texDirt, *texConcrete, *texGrass, *texTest, *texMetal, *texObsidian, *texMoss, *texCrate, *texCrateSpec;
	TexturePack *texpaxCrate, *texpaxMoss, *texFabric, *texDamagedMetal, *texpaxBricks, *texpaxConcrete, *texpaxDirt, *texpaxMetal, *texpaxMudConcrete, *texpaxCobble, *texpaxGravel, *texpaxWetDirt, *texpaxMetalWall, *texpaxPebbles, *texpaxRustyMetal, *texpaxGem, *texpaxObsidian, *texpaxLakeBottom;
	Box * crate, *crate2;
	Quad *quad, *water;
	Quad *grass;
	Camera *camera;
	HUD *hud;
	void move();
	void key(int key, int action);
	void mouseMove(double xpos, double ypos);
	void mouseClick(int button, int action);
	void update();
	void uniformShaders(double dt);
	void drawObj(double dt);
	void drawGround();
	void drawSky();
	void drawHudObj();
	std::list<Projectile*> projectiles;
	int objectArea;
	float world_border;
	Box *skybox;
	Shader *moss_shader,*fragShader, *smoke_shader, *cell_shader;
	float collision(const OBB &obb1, const OBB &obb2);
	float collisionProjection(const OBB &obb1, const OBB &obb2, glm::vec3 projAxis);
	bool collision(const OBP &obp1, const OBB &obb2);
	float collisionProjection(const OBP &obp1, const OBB &obb2, glm::vec3 projAxis);

	Quad * box;
	Cloth *cloth;
	Framebuffer *lakebuffer;
};

#endif // WORLD_H
