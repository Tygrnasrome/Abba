#include "world.h"
//#include "objloader.h"

World::World()
{

	//cloth = new Cloth(20);
	//cloth->place(glm::vec3(2,2,0));
	camera = new Camera();
	hud = new HUD();
	if(teleportNum % 2 == 1)
		printf("liché číslo teleportů");
	objectArea = world_border = 150;
	boxes = new Group;
	piramids = new Group;
	walls = new Group;


	for(int i = 0; i < arrayQuads.size(); i++)
	{
		arrayQuads[i] = new Quad();
		arrayQuads[i]->scale(objectArea,3, 1);
	}
	//wall 0
	arrayQuads[0]->place(glm::vec3(1,arrayQuads[0]->getSize().y,objectArea-1));

	//wall 1
	arrayQuads[1]->yaw(180);
	arrayQuads[1]->place(glm::vec3(1,arrayQuads[1]->getSize().y,-objectArea-1));

	//wall 2
	arrayQuads[2]->yaw(-90);
	arrayQuads[2]->place(glm::vec3(-objectArea+1,arrayQuads[2]->getSize().y,-1));

	//wall 3
	arrayQuads[3]->yaw(90);
	arrayQuads[3]->place(glm::vec3(objectArea+1,arrayQuads[3]->getSize().y,-1));
	float tmpX, tmpZ, tmpYaw, tmpR, tmpB, tmpG;
	for(int i = 0; i < teleportNum; i++)
	{
		tmpX = rand() %objectArea*2-objectArea;
		tmpZ = rand() %objectArea*2-objectArea;

		arrayTpFrames[i]  = new Teleport;
		arrayTpFrames[i]->place(glm::vec3(tmpX,0,tmpZ));

		if(i%2 == 0)
		{
			tmpR = (float)rand()/ (float) RAND_MAX;
			tmpG = (float)rand()/ (float) RAND_MAX;
			tmpB = (float)rand()/ (float) RAND_MAX;
			tmpYaw = rand() % 360;
		}
		arrayTpFrames[i]->yaw(tmpYaw);
		arrayTpFrames[i]->rgb = glm::vec3(tmpR,tmpG,tmpB);
		arrayTpPortals[i]  = new Quad;
		arrayTpPortals[i]->place(glm::vec3(tmpX,4,tmpZ));
		arrayTpPortals[i]->scale(3,4,3);
		arrayTpPortals[i]->yaw(tmpYaw);

		//bounding boxy teleportu
		arrayTpBB[i]  = new Group;
		arrayTpBox[i*3]  = new Box;
		arrayTpBox[i*3 + 1]  = new Box;
		arrayTpBox[i*3 + 2]  = new Box;
		float width = 0.3;
		float height = 4.65;
		arrayTpBox[i*3]->place(glm::vec3(2.7,0,0));
		arrayTpBox[i*3+1]->place(glm::vec3(-2.7,0,0));
		arrayTpBox[i*3+2]->place(glm::vec3(0,height,0));
		arrayTpBox[i*3]->scale(width,height,width);
		arrayTpBox[i*3+1]->scale(width,height,width);
		arrayTpBox[i*3+2]->scale(3,width,width);

		arrayTpBB[i]->add(arrayTpBox[i*3]);
		arrayTpBB[i]->add(arrayTpBox[i*3+1]);
		arrayTpBB[i]->add(arrayTpBox[i*3+2]);

		arrayTpBB[i]->place(glm::vec3(tmpX,3,tmpZ));
		arrayTpBB[i]->yaw(tmpYaw);
	}

	for(int i = 0; i <= teleportNum; i++)
	{
		arrayTpFrames[i]->linked = arrayTpFrames[i+1];
		arrayTpFrames[i+1]->linked = arrayTpFrames[i];
		i++;
	}
	for(int i = 0; i < arrayQuads.size(); i++)
	{
		walls->add(arrayQuads[i]);
	}

	for(int i = 0; i < arrayBox.size(); i++){
		float x = rand() %objectArea*2-objectArea;
		float z = rand() %objectArea*2-objectArea;

		arrayBox[i] = new Box();
		arrayBox[i]->scale(12,8,7);
		arrayBox[i]->place(glm::vec3(x, arrayBox[i]->getSize().y,z));
		boxes->add(arrayBox[i]);
	}

	for(int i = 0; i < arrayPyramid.size(); i++){
		float x = rand() %objectArea*2-objectArea;
		float z = rand() %objectArea*2-objectArea;
		float w = rand() % 1000/400.0+1;

		//arrayPyramid[i] = new Pyramid(x,-6,z, x+1,-7, z+1);
		arrayPyramid[i] = new Pyramid();
		arrayPyramid[i]->scale(10,10,10);
		arrayPyramid[i]->place(glm::vec3(x, arrayPyramid[i]->getSize().y,z));
		piramids->add(arrayPyramid[i]);
	}
	for(int i = 0; i < arrayFlag.size(); i++){
		float x = rand() %objectArea*2-objectArea;
		float z = rand() %objectArea*2-objectArea;
		float w = rand() % 1000/400.0+1;

		arrayFlag[i] = new Flag();
		arrayFlag[i]->place(glm::vec3(x, arrayFlag[i]->getSize().y,z));
	}

	lightSource = new Box();
	lightSource->scale(0.1,0.1,0.1);
	//	shading_shader = new Shader("shaders/shadingTexture.vert", "shaders/shadingTexture.frag");
	sky_shader = new Shader("shaders/sky.vert", "shaders/sky.frag");
	//	alfa_shader = new Shader("shaders/alfaShader.vert", "shaders/alfaShader.frag");
	instancing_shader = new Shader("shaders/instancing.vert", "shaders/alfaShader.frag");
	noise_shader = new Shader("shaders/shadingTexture.vert", "shaders/buzz.frag");
	smoke_shader = new Shader("shaders/shadingTexture.vert", "shaders/smoke.frag");
	//	cell_shader = new Shader("shaders/shadingTexture.vert", "shaders/cell.frag");
	//	moss_shader = new Shader ("shaders/shadingTexture.vert", "shaders/multitexturingShader.frag");
	//	crate_shader = new Shader("shaders/shadingTexture.vert", "shaders/crate.frag");
	//	normal_shader = new Shader("shaders/shadingTexture.vert", "shaders/normalLight.frag");
	//	height_shader = new Shader("shaders/shadingTexture.vert", "shaders/height.frag");
	monochromatic_shader = new Shader("shaders/monochromatic.vert", "shaders/monochromatic.frag");
	hud_shader = new Shader("shaders/monochromatic.vert", "shaders/hud.frag");
	pbr_shader = new Shader("shaders/pbrParallax.vert", "shaders/pbrParallax.frag");


	angle = 0;

	player = new Player(glm::vec3(37/256.0, 150/256.0, 190/256.0));
	player->place(glm::vec3(0,player->belt->getSize().y,1));

	quad = new Quad;
	quad->place(glm::vec3(1,0,1));
	quad->scale(objectArea,objectArea,objectArea);

	sky = new CubeMap;
	texBrick = new Texture("data/Tileable_Bricks_01_Diffuse.png");
	texDirt = new Texture("data/Ground_Dirt_007_SD/Ground_Dirt_007_basecolor.jpg");
	texConcrete = new Texture("data/Concrete_Wall_008_SD/Concrete_Wall_008_basecolor.jpg");
	texTest = new Texture("data/checker-map_tho.png");
	texMetal = new Texture("data/PaintedMetal006_2K-JPG/PaintedMetal006_2K_Color.jpg");
	texGrass = new Texture("data/grass.png", GL_TEXTURE0, GL_CLAMP_TO_EDGE);
	texObsidian = new Texture("data/obsidian.jpg");
	texMoss = new Texture("data/Surface_Imperfection_001_SD/Surface_Imperfection_001_basecolor.jpg", GL_TEXTURE1);
	texpaxMoss = new TexturePack({"data/Tileable_Bricks_01_Diffuse.png","data/Surface_Imperfection_001_SD/Surface_Imperfection_001_basecolor.jpg"});
	texCrate = new Texture("data/container2.png");
	texCrateSpec = new Texture("data/container2_specular.png", GL_TEXTURE1);
	texpaxCrate = new TexturePack({"data/container2.png","data/container2_specular.png"});
	texDamagedMetal = new TexturePack({"data/Metal_Damaged_001_SD/Metal_Damaged_001_basecolor.jpg",
					   "data/Metal_Damaged_001_SD/Metal_Damaged_001_metallic.jpg"});

	//
	//	texpaxBricks = new TexturePack({"data/harshbricks-bl/harshbricks-albedo.png",
	//					"data/harshbricks-bl/harshbricks-ao2.png",
	//					"data/harshbricks-bl/harshbricks-normal.png",
	//					"data/harshbricks-bl/harshbricks-height5-16.png"});
	//

	texpaxBricks = new TexturePack({/*"data/rock-slab-wall1-bl/harshbricks-albedo.png",
													"data/rock-slab-wall1-bl/harshbricks-roughness.png",
													"data/rock-slab-wall1-bl/harshbricks-normal.png"*/


					"data/rock-slab-wall1-bl/rock-slab-wall_albedo.png",
					"data/rock-slab-wall1-bl/rock-slab-wall_roughness.png",
					"data/rock-slab-wall1-bl/rock-slab-wall_normal-ogl.png",
					"data/rock-slab-wall1-bl/rock-slab-wall_height.png"
				       });
	texpaxConcrete = new TexturePack({"data/Concrete_Wall_008_SD/Concrete_Wall_008_basecolor.jpg",
					  "data/Concrete_Wall_008_SD/Concrete_Wall_008_metallic.jpg",
					  "data/Concrete_Wall_008_SD/Concrete_Wall_008_normal.jpg"});
	texpaxDirt = new TexturePack({"data/Ground_Dirt_007_SD/Ground_Dirt_007_basecolor.jpg",
				      "data/Ground_Dirt_007_SD/Ground_Dirt_007_black.jpg",
				      "data/Ground_Dirt_007_SD/Ground_Dirt_007_normal.jpg"});
	texpaxMudConcrete = new TexturePack({"data/ground/Concrete_Muddy_001_BaseColor.jpg",
					     "data/ground/Concrete_Muddy_001_Height.png",
					     "data/ground/Concrete_Muddy_001_Normal.jpg"});
	texpaxMetal = new TexturePack({"data/PaintedMetal006_2K-JPG/PaintedMetal006_2K_Color.jpg",
				       "data/PaintedMetal006_2K-JPG/PaintedMetal006_2K_Metalness.jpg",
				       "data/PaintedMetal006_2K-JPG/PaintedMetal006_2K_NormalGL.jpg"});
	texpaxCobble = new TexturePack({"data/dusty-cobble/dusty-cobble_albedo.png",
					"data/dusty-cobble/dusty-cobble_ao.png",
					"data/dusty-cobble/dusty-cobble_normal-ogl.png",
					"data/dusty-cobble/dusty-cobble_height.png",
					"data/dusty-cobble/dusty-cobble_metallic.png",
					"data/dusty-cobble/dusty-cobble_metallic.png",
					"data/dusty-cobble/dusty-cobble_roughness.png"});
	texpaxGravel = new TexturePack({"data/gravel/Gravel_001_BaseColor.jpg",
					"data/gravel/Gravel_001_Roughness.jpg",
					"data/gravel/Gravel_001_Normal.jpg",
					"data/gravel/Gravel_001_Height.png"});
	texpaxWetDirt = new TexturePack({"data/wet-dirt/Ground_Wet_002_basecolor.jpg",
					 "data/wet-dirt/Ground_Wet_002_ambientOcclusion.jpg",
					 "data/wet-dirt/Ground_Wet_002_normal.jpg",
					 "data/wet-dirt/Ground_Wet_002_height.png",
					 "data/wet-dirt/Ground_Wet_002_opacity.jpg",
					 "data/wet-dirt/Ground_Wet_002_opacity.jpg",
					 "data/wet-dirt/Ground_Wet_002_roughness.jpg"});
	texpaxMetalWall= new TexturePack({"data/scifi-wall/Sci-Fi_Wall_014_basecolor.jpg",
					  "data/scifi-wall/Sci-Fi_Wall_014_ambientOcclusion.jpg",
					  "data/scifi-wall/Sci-Fi_Wall_014_normal.jpg",
					  "data/scifi-wall/Sci-Fi_Wall_014_height.png",
					  "data/scifi-wall/Sci-Fi_Wall_014_metallic.jpg",
					  "data/scifi-wall/Sci-Fi_Wall_014_opacity.jpg",
					  "data/scifi-wall/Sci-Fi_Wall_014_roughness.jpg"});
	texpaxPebbles= new TexturePack({"data/pebbles/Pebbles_028_BaseColor.jpg",
					"data/pebbles/Pebbles_028_AmbientOcclusion.jpg",
					"data/pebbles/Pebbles_028_Normal.jpg",
					"data/pebbles/Pebbles_028_Height.png",
					"data/pebbles/Pebbles_028_Metallic.jpg",
					"data/pebbles/Pebbles_028_Metallic.jpg",
					"data/pebbles/Pebbles_028_Roughness.jpg"});
	texpaxRustyMetal= new TexturePack({"data/rusty-metal/Metal_Rusted_010_basecolor.jpg",
					   "data/rusty-metal/Metal_Rusted_010_ambientOcclusion.jpg",
					   "data/rusty-metal/Metal_Rusted_010_normal.jpg",
					   "data/rusty-metal/Metal_Rusted_010_height.png",
					   "data/rusty-metal/Metal_Rusted_010_metallic.jpg",
					   "data/rusty-metal/Metal_Rusted_010_metallic.jpg",
					   "data/rusty-metal/Metal_Rusted_010_roughness.jpg"});
	texpaxGem= new TexturePack({"data/Crystal_003_SD/Crystal_003_COLOR.jpg",
				    "data/Crystal_003_SD/Crystal_003_OCC.jpg",
				    "data/Crystal_003_SD/Crystal_003_NORM.jpg",
				    "data/Crystal_003_SD/Crystal_003_DISP.png",
				    "data/Crystal_003_SD/Crystal_003_BLACK.jpg",
				    "data/Crystal_003_SD/Crystal_003_BLACK.jpg",
				    "data/Crystal_003_SD/Crystal_003_ROUGH.jpg"});
	texpaxObsidian= new TexturePack({"data/obsidian/Obsidian_001_basecolor.jpg",
					 "data/obsidian/Obsidian_001_ambientOcclusion.jpg",
					 "data/obsidian/Obsidian_001_normal.jpg",
					 "data/obsidian/Obsidian_001_height.png",
					 "data/obsidian/Obsidian_001_metallic.jpg",
					 "data/obsidian/Obsidian_001_metallic.jpg",
					 "data/obsidian/Obsidian_001_roughness.jpg"});
	texFabric = new TexturePack({"data/cloth/Fabric_polyester_003_basecolor.jpg",
				     "data/cloth/Fabric_polyester_003_ambientOcclusion.jpg",
				     "data/cloth/Fabric_polyester_003_normal.jpg",
				     "data/cloth/Fabric_polyester_003_height.png",
				     "data/cloth/Fabric_polyester_003_black.jpg",
				     "data/cloth/Fabric_polyester_003_opacity.jpg",
				     "data/cloth/Fabric_polyester_003_roughness.jpg"});
	texpaxLakeBottom = new TexturePack({	"data/lake-bottom/Ground_Wet_Pebbles_001_basecolor.jpg",
						"data/lake-bottom/Ground_Wet_Pebbles_001_ambientOcclusion.jpg",
						"data/lake-bottom/Ground_Wet_Pebbles_001_normal.jpg",
						"data/lake-bottom/Ground_Wet_Pebbles_001_height.png",
						"data/lake-bottom/Ground_Wet_Pebbles_001_black.jpg",
						"data/lake-bottom/Ground_Wet_Pebbles_001_black.jpg",
						"data/lake-bottom/Ground_Wet_Pebbles_001_roughness.jpg"});



	skybox = new Box();
	grass = new Quad();
	grass->pitch(180);
	grass->place(glm::vec3(0,1,0));
	box = new Quad;
	box->place(glm::vec3(0,0,0));
	crate = new Box();
	crate->scale(2,2,2);
	crate->place(glm::vec3(0,2,0));
	crate2 = new Box();
	crate2->scale(2,2,2);
	crate2->place(glm::vec3(4.1,2,0));
	uniformShaders(0);

}
void World::move()
{
	if(camera->freecam)
		camera->move();
	if(!camera->freecam)
		player->move();
}
void World::key(int key, int action)
{
	if (camera->freecam)
		camera->key(key, action);
	else
		player->key(key, action);

	//flag dispose
	if (key == GLFW_KEY_Q && action == GLFW_PRESS){
		player->holdingFlag = 0;
		for(auto it = arrayFlag.begin(); it != arrayFlag.end(); it++)
		{
			if((*it)->holded == true)
			{
				(*it)->holded = false;

				float x = rand() %objectArea*2-objectArea;
				float z = rand() %objectArea*2-objectArea;


				(*it)->place(glm::vec3(x, (*it)->getSize().y,z));
			}
		}
	}
	/*	aktivace freecam	*/
	if (key == GLFW_KEY_F && action == GLFW_PRESS)
	{
		//resetnutí stavů kláves (antibug mechanika)
		if(camera->freecam)
			camera->resetKeys();
		else
			player->resetKeys();
		//freecam switch
		camera->freecam = !camera->freecam;
		player->grabCam = !camera->freecam;
	}
	if (key == GLFW_KEY_G && action == GLFW_PRESS)
	{
		//		cursorGrab = -cursorGrab;
		//		if(cursorGrab)
		//			glfwSetInputMode(glfw_window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);
		//		else
		//			glfwSetInputMode(glfw_window,GLFW_CURSOR,GLFW_CURSOR_NORMAL);

	}
	if (key == GLFW_KEY_M &&( action == GLFW_PRESS || action == GLFW_RELEASE))
	{
		hud->switchProjections();
	}
}
void World::mouseClick(int button, int action)
{
	/*výstřel*/
	if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && player->holdingFlag != FLAG_NO_SHOOT)
		projectiles.push_back(new Projectile(player->getPosition()+ glm::vec3(player->direction.x,1,player->direction.z), player->direction, player->color,player->yaw()));
}
void World::mouseMove(double xpos, double ypos)
{
	if (camera->freecam)
		camera->mouseMotion(xpos,ypos);
	else
		player->mouseMotion(xpos,ypos);
}
void World::uniformShaders(double dt)
{
	char lightCol[20];
	char lightPos[20];

	pbr_shader->use();
	int i = 0;
	for(auto it = projectiles.begin(); it != projectiles.end(); it++)
	{

		sprintf(lightPos,"lightPositions[%d]",i);
		sprintf(lightCol,"lightColors[%d]",i);
		pbr_shader->uniform(lightPos,(*it)->getPosition());
		pbr_shader->uniform(lightCol,glm::vec3(37, 150, 190));
		if(i > 50)  break;
		i++;
	}
	projectiles.size() > 50 ? pbr_shader->uniformInt("lightCount",50) : pbr_shader->uniformInt("lightCount",projectiles.size());
	pbr_shader->uniform("viewPos", camera->position);

	pbr_shader->uniform("uv_repeat", glm::vec2(1,1));
	pbr_shader->uniform("heightScale", 0.1);

	//	moss_shader->use();
	//	moss_shader->uniform("viewPos", camera->position);

	//	moss_shader->uniform("angle", angle);

	//	moss_shader->uniform("light.ambient", 0.5,0.5,0.5);
	//	moss_shader->uniform("light.diffuse", 0.5,0.5,0.5);
	//	moss_shader->uniform("light.specular", 0.5,0.5,0.5);

	//	moss_shader->uniform("material.ambient", 0.5,0.5,0.85);
	//	moss_shader->uniform("tileNum",glm::vec2(1,1));

	//	moss_shader->uniform("material.diffuse", 0.15,0.45,0.85);
	//	moss_shader->uniform("material.specular", 0.15,0.45,0.85);
	//	moss_shader->uniform("material.shininess", 42);

	//	crate_shader->use();

	//	crate_shader->uniform("viewPos", camera->position);

	//	crate_shader->uniform("angle", angle);

	//	crate_shader->uniform("light.ambient", 0.5,0.5,0.5);
	//	crate_shader->uniform("light.diffuse", 0.5,0.5,0.5);
	//	crate_shader->uniform("light.specular", 0.5,0.5,0.5);
	//	crate_shader->uniform("model",lightSource->getMatrix());
	//	crate_shader->uniform("light.position",lightSource->getPosition());
	//	crate_shader->uniform("material.ambient", 0.5,0.5,0.85);
	//	crate_shader->uniform("tileNum",glm::vec2(1,1));

	//	crate_shader->uniform("material.diffuse", 0.15,0.45,0.85);
	//	crate_shader->uniform("material.specular", 0.15,0.45,0.85);
	//	crate_shader->uniform("material.shininess", 42);

	//	shading_shader->use();
	//	shading_shader->uniform("viewPos", camera->position);

	//	shading_shader->uniform("angle", angle);

	//	shading_shader->uniform("light.ambient", 0.5,0.5,0.5);
	//	shading_shader->uniform("light.diffuse", 0.5,0.5,0.5);
	//	shading_shader->uniform("light.specular", 0.5,0.5,0.5);

	//	shading_shader->uniform("material.ambient", 0.5,0.5,0.85);
	//	shading_shader->uniform("tileNum",glm::vec2(1,1));

	//	shading_shader->uniform("material.diffuse", 0.15,0.45,0.85);
	//	shading_shader->uniform("material.specular", 0.15,0.45,0.85);
	//	shading_shader->uniform("material.shininess", 42);

	//	normal_shader->use();
	//	normal_shader->uniform("viewPos", camera->position);

	//	normal_shader->uniform("angle", angle);

	//	normal_shader->uniform("light.ambient", 0.5,0.5,0.5);
	//	normal_shader->uniform("light.diffuse", 0.5,0.5,0.5);
	//	normal_shader->uniform("light.specular", 1.5,1.5,1.5);

	//	normal_shader->uniform("material.ambient", 0.5,0.5,0.85);
	//	normal_shader->uniform("tileNum",glm::vec2(1,1));

	//	normal_shader->uniform("material.diffuse", 0.15,0.45,0.85);
	//	normal_shader->uniform("material.specular", 0.15,0.45,0.85);
	//	normal_shader->uniform("material.shininess", 42);
	//	normal_shader->uniform("light.position",lightSource->getPosition());

	//	height_shader->use();
	//	height_shader->uniform("viewPos", camera->position);

	//	height_shader->uniform("angle", angle);

	//	height_shader->uniform("light.ambient", 0.5,0.5,0.5);
	//	height_shader->uniform("light.diffuse", 0.5,0.5,0.5);
	//	height_shader->uniform("light.specular", 1.5,1.5,1.5);

	//	height_shader->uniform("material.ambient", 0.5,0.5,0.85);
	//	height_shader->uniform("tileNum",glm::vec2(1,1));

	//	height_shader->uniform("material.diffuse", 0.15,0.45,0.85);
	//	height_shader->uniform("material.specular", 0.15,0.45,0.85);
	//	height_shader->uniform("material.shininess", 42);
	//	height_shader->uniform("light.position",lightSource->getPosition());

	//	shading_shader->uniform("model",lightSource->getMatrix());
	//	shading_shader->uniform("light.position",lightSource->getPosition());

	angle += dt * 80 ;
}
void World::drawObj(double dt)
{
	uniformShaders(dt);
	int r = 5;
	pbr_shader->use();
	pbr_shader->texturePack({"albedoMap","aoMap","normalMap","heightMap", "metallicMap","opacityMap", "roughnessMap"}, texpaxObsidian);
	lightSource->place(glm::vec3(-r*cos(angle/160),2, -r*sin(angle/160)));
	lightSource->draw(pbr_shader, camera);

	//kreslení boxů
	pbr_shader->use();
	pbr_shader->texturePack({"albedoMap","aoMap","normalMap","heightMap", "metallicMap","opacityMap", "roughnessMap"}, texpaxCobble);
	pbr_shader->uniform("uv_repeat", glm::vec2(2,2));
	boxes->draw(pbr_shader, camera);


	//	//kreslení cloth
	//	height_shader->use();
	//	height_shader->texturePack({"texture_map","specular_map","normal_map", "heightMap"}, texpaxBricks);
	//	//cloth->draw(height_shader, camera);

	//kreslení boxu ve středu
	if(rndObjNum != 1)
	{
		//		normal_shader->use();
		//		normal_shader->texturePack({"texture_map","specular_map","normal_map"}, texpaxBricks);
		//		crate->draw(normal_shader, camera);
		//		height_shader->use();
		//		height_shader->texturePack({"texture_map","specular_map","normal_map", "height_map"}, texpaxBricks);
		//		crate2->draw(height_shader, camera);

	}

	//kreslení ohrádky
	pbr_shader->use();
	pbr_shader->texturePack({"albedoMap","aoMap","normalMap","heightMap", "metallicMap","opacityMap", "roughnessMap"}, texpaxMetalWall);
	pbr_shader->uniform("uv_repeat",glm::vec2(objectArea/2,1));
	walls->draw(pbr_shader, camera);
	pbr_shader->uniform("uv_repeat",glm::vec2(1,1));

	//kreslení pyramid
	noise_shader->use();
	//pbr_shader->texturePack({"albedoMap","aoMap","normalMap","heightMap", "metallicMap","opacityMap", "roughnessMap"}, texpaxCobble);
	piramids->draw(noise_shader, camera);

	//kreslení hráče
	if(playerNum != 0)
	{
		pbr_shader->use();
		pbr_shader->texturePack({"albedoMap","aoMap","normalMap","heightMap", "metallicMap","opacityMap", "roughnessMap"}, texpaxRustyMetal);
		player->draw(pbr_shader, camera);

	}

	//kreslení teleportu
	pbr_shader->use();
	pbr_shader->texturePack({"albedoMap","aoMap","normalMap","heightMap", "metallicMap","opacityMap", "roughnessMap"}, texpaxObsidian);
	for(int i = 0; i < teleportNum; i++)
	{
		arrayTpFrames[i]->draw(pbr_shader, camera);
	}

//	//kreslení bounding boxu teleportu
//	pbr_shader->use();
//	pbr_shader->texturePack({"albedoMap","aoMap","normalMap","heightMap", "metallicMap","opacityMap", "roughnessMap"}, texpaxObsidian);
//	for(int i = 0; i < teleportNum * 3; i++)
//	{
//		arrayTpBox[i]->draw(pbr_shader, camera);
//	}
	smoke_shader->use();
	smoke_shader->uniform("angle", angle/8);
	for(int i = 0; i < teleportNum; i++)
	{
		if(i%2 == 0)
			smoke_shader->uniform("rgb", arrayTpFrames[i]->rgb);
		arrayTpPortals[i]->draw(smoke_shader, camera);
	}

	//kresleni střel
	//for projde všechny objekty listu projectiles

	monochromatic_shader->use();
	monochromatic_shader->texture("texture_map",texBrick);

	for(auto it = projectiles.begin(); it != projectiles.end(); it++)
	{
		monochromatic_shader->uniform("color",(*it)->color);
		(*it)->draw(monochromatic_shader, camera);
	}
	//kresleni vlajek
	pbr_shader->use();
	for(int i = 0; i < flagNum; i++)
	{
		pbr_shader->texturePack({"albedoMap","aoMap","normalMap","heightMap", "metallicMap","opacityMap", "roughnessMap"}, texpaxRustyMetal); //metal
		arrayFlag[i]->cylinder->draw(pbr_shader, camera);
		//pbr_shader->uniformInt("opacityThreshold", 1); // there is a need of opacity in cloth, but it should be included in separated opacity texture map
		pbr_shader->texturePack({"albedoMap","aoMap","normalMap","heightMap", "metallicMap","opacityMap", "roughnessMap"}, texFabric); //fabric
		arrayFlag[i]->cloth->draw(pbr_shader, camera);
		//pbr_shader->uniformInt("opacityThreshold", 0);
	}


	//	//kreslení grass
	if(rndObjNum == 0)
	{
		instancing_shader->use();
		instancing_shader->texture("texture_map",texGrass);

		instancing_shader->uniform("worldBorder",world_border*2);
		grass->drawInstanced(instancing_shader, camera, 10000);
	}
}

void World::drawGround()
{
	//kreslení podlahy
	pbr_shader->use();
	pbr_shader->texturePack({"albedoMap","aoMap","normalMap","heightMap", "metallicMap","opacityMap", "roughnessMap"}, texpaxWetDirt);
	quad->pitch(-90);
	pbr_shader->uniform("uv_repeat",glm::vec2(60,60));
	quad->draw(pbr_shader, camera);
}

void World::drawHudObj()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	hud_shader->use();
	hud_shader->uniform("color",glm::vec3(1,1,1));
	//kreslení boxů
	boxes->draw(hud_shader, hud);

	//kreslení cloth
	//cloth->draw(hud_shader, hud);

	//kreslení boxu ve středu
	if(rndObjNum != 1)
	{
		crate->draw(hud_shader, hud);
	}

	//kreslení ohrádky
	walls->draw(hud_shader, hud);

	//kreslení pyramid
	piramids->draw(hud_shader, hud);

	//kreslení hráče
	if(playerNum != 0)
	{
		hud_shader->uniform("color",player->color);
		player->scale(2,2,2);
		player->draw(hud_shader, hud);
		player->scale(1,1,1);
		hud_shader->uniform("color",glm::vec3(1,1,1));
	}

	//kreslení teleportu
	for(int i = 0; i < teleportNum; i++)
	{
		hud_shader->uniform("color",arrayTpFrames[i]->rgb);
		arrayTpFrames[i]->draw(hud_shader, hud);
	}

	for(int i = 0; i < teleportNum; i++)
	{
		arrayTpPortals[i]->draw(hud_shader, hud);
	}
hud_shader->uniform("color",glm::vec3(1,1,1));
	//kresleni střel
	//for projde všechny objekty listu projectiles
	for(auto it = projectiles.begin(); it != projectiles.end(); it++)
	{
		monochromatic_shader->use();
		monochromatic_shader->uniform("color",(*it)->color);
		(*it)->draw(monochromatic_shader, hud);
	}
	hud_shader->use();
	hud_shader->uniform("color",glm::vec3(1,1,1));
	//kresleni vlajek
	for(int i = 0; i < flagNum; i++)
	{
		arrayFlag[i]->cylinder->draw(hud_shader, hud);
		//arrayFlag[i]->cloth->draw(hud_shader, hud);
	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
void World::draw(double dt)
{
	drawSky();
	drawObj(dt);
	drawHudObj();
	drawGround();
}
void World::drawSky()
{
	//kreslení cubemap
	skybox->yaw(0);

	glm::mat4 camera_view_bak = camera->view;
	camera->view = glm::mat4(glm::mat3(camera->view));

	glDepthMask(GL_FALSE);
	sky_shader->use();

	sky->bind();
	skybox->draw(sky_shader, camera);

	glDepthMask(GL_TRUE);
	camera->view = camera_view_bak;
}

void World::update()
{
	if(camera->freecam){
		camera->move();
	}
	player->update(this,camera);
	//test kolize vlajek s hráčem a posun držené vlajky
	for(auto it = arrayFlag.begin(); it != arrayFlag.end(); it++)
	{
		if(collision((*it)->getOBB(),player->getOBB()) && player->holdingFlag == 0)
		{
			(*it)->holded = true;
			player->holdingFlag = (*it)->type;
		}
		//posunování držené vlajky za hráčem
		if((*it)->holded == true)
			(*it)->place(player->getPosition());
	}
	for(int i = 0 ; i<teleportNum ; i++)
	{
		arrayTpFrames[i]->update(player);
	}

	//for projde všechny objekty listu projectiles
	for(auto it = projectiles.begin(); it != projectiles.end(); it++)
	{
		//update pozice projektilů
		(*it)->update();

		//pokud alespoň jedna z hodnot souřadnic projektilu přesáhne world_border tak se zničí
		//nefunguje
		if(abs((*it)->position.x) > world_border || abs((*it)->position.y) > world_border || abs((*it)->position.z) > world_border)
		{
			//odstranění z listu projectiles
			delete *it;
			it = projectiles.erase(it);
			it--;
		}
	}

	/*kolize krychlí (domů) se střelama*/

	for(int i = 0; i < arrayBox.size(); i++)
	{
		for(auto it = projectiles.begin(); it != projectiles.end(); it++){


			if(collision((*it)->getOBB(), arrayBox[i]->getOBB()))
			{
				/*střela vybuchne a pak se vymaže*/


				//vybuchnutí zatím nic nedělá
				//(*it)->explode();

				delete *it;
				it = projectiles.erase(it);
				it--;
			}
		}
	}
	//generování nových bodů vlajky
	//čím větší je winddirrange, tak se bude směr větru méně měnit, max 1(mění se o 360 stupňů), min 0(nemění se)
	float windDirRange = 0.2;
	for(int i = 0; i < arrayFlag.size(); i++)
	{
		arrayFlag[i]->cloth->update(sin(angle/360) < windDirRange ? sin(angle/360) > -windDirRange ? sin(angle/360):-windDirRange : windDirRange);
		arrayFlag[i]->cloth->generateStruct();
	}
	hud->update(player->direction,player->getPosition());
}


glm::vec3 World::getMTV(const OBB &a, const OBB &b)
{
	// MTV = minimum translation vector
	glm::vec3 dist = b.pos - a.pos;

	float d, minD = 3.40282e+038;
	glm::vec3 axis;

	if((d = -collisionProjection(a,b, a.axisX)) > -.00001 && d < minD)
	{
		minD = d;
		axis = glm::dot(dist,a.axisX) < 0 ? -a.axisX : a.axisX;
	}
	if((d = -collisionProjection(a,b, a.axisY)) > -.00001 && d < minD)
	{
		minD = d;
		axis = glm::dot(dist,a.axisY) < 0 ? -a.axisY : a.axisY;
	}
	if((d = -collisionProjection(a,b, a.axisZ)) > -.00001 && d < minD)
	{
		minD = d;
		axis = glm::dot(dist,a.axisZ) < 0 ? -a.axisZ : a.axisZ;
	}
	//rohy
	if((d = -collisionProjection(a,b, b.axisZ)) > -.00001 && d < minD)
	{
		minD = d;
		axis = glm::dot(dist,b.axisZ) < 0 ? -b.axisZ : b.axisZ;
	}
	return axis * minD;
}

glm::vec3 World::getMTV(const OBP &a, const OBB &b)
{
	// MTV = minimum translation vector

	float d, minD = 3.40282e+038;
	glm::vec3 axis;

	if((d = -collisionProjection(a,b, a.axisN)) > .00001 && d < minD)
	{
		minD = d;
		axis = a.axisZ;
	}
	if((d = -collisionProjection(a,b, a.axisW)) > .00001 && d < minD)
	{
		minD = d;
		axis = -a.axisX;
	}
	if((d = -collisionProjection(a,b, a.axisS)) > .00001 && d < minD)
	{
		minD = d;
		axis = -a.axisZ;
	}
	if((d = -collisionProjection(a,b, a.axisE)) > .00001 && d < minD)
	{
		minD = d;
		axis = a.axisX;
	}
	return axis * minD;
}

float World::collisionProjection(const OBB &obb1, const OBB &obb2, glm::vec3 projAxis){
	float distance = fabs(glm::dot(obb1.pos - obb2.pos, projAxis));
	float radSum =
			fabs(glm::dot(obb1.size.x * obb1.axisX, projAxis)) +
			fabs(glm::dot(obb1.size.y * obb1.axisY, projAxis)) +
			fabs(glm::dot(obb1.size.z * obb1.axisZ, projAxis)) +
			fabs(glm::dot(obb2.size.x * obb2.axisX, projAxis)) +
			fabs(glm::dot(obb2.size.y * obb2.axisY, projAxis)) +
			fabs(glm::dot(obb2.size.z * obb2.axisZ, projAxis));

	return distance - radSum;
}

float World::collisionProjection(const OBP &obp1, const OBB &obb2, glm::vec3 projAxis){
	float distance = fabs(glm::dot(obp1.pos - obb2.pos, projAxis));
	float radSum;
	float x = glm::dot(obp1.size.x * obp1.axisX, projAxis);
	float y = glm::dot(obp1.size.y * obp1.axisY, projAxis);
	float z = glm::dot(obp1.size.z * obp1.axisZ, projAxis);

	if(glm::dot(projAxis, obb2.pos - obp1.pos) < 0)
	{
		radSum = fabs(	fmin(y,
				     fmin(-y + x + z,
					  fmin(-y - x + z,
					       fmin(-y + x - z,
						    -y - x - z)))));
	} else
	{
		radSum = fabs(	fmax(y,
				     fmax(-y + x + z,
					  fmax(-y - x + z,
					       fmax(-y + x - z,
						    -y - x - z)))));
	}
	radSum +=	fabs(glm::dot(obb2.size.x * obb2.axisX, projAxis)) +
			fabs(glm::dot(obb2.size.y * obb2.axisY, projAxis)) +
			fabs(glm::dot(obb2.size.z * obb2.axisZ, projAxis));

	return distance - radSum;
}

float World::collision(const OBB &obb1, const OBB &obb2){
	return !(
				collisionProjection(obb1, obb2, obb1.axisX) > 0 ||
				collisionProjection(obb1, obb2, obb1.axisY) > 0 ||
				collisionProjection(obb1, obb2, obb1.axisZ) > 0 ||
				collisionProjection(obb1, obb2, obb2.axisX) > 0 ||
				collisionProjection(obb1, obb2, obb2.axisY) > 0 ||
				collisionProjection(obb1, obb2, obb2.axisZ) > 0 ||
				collisionProjection(obb1, obb2, glm::cross(obb1.axisX,obb2.axisX)) > 0 ||
				collisionProjection(obb1, obb2, glm::cross(obb1.axisX,obb2.axisY)) > 0 ||
				collisionProjection(obb1, obb2, glm::cross(obb1.axisX,obb2.axisZ)) > 0 ||
				collisionProjection(obb1, obb2, glm::cross(obb1.axisY,obb2.axisX)) > 0 ||
				collisionProjection(obb1, obb2, glm::cross(obb1.axisY,obb2.axisY)) > 0 ||
				collisionProjection(obb1, obb2, glm::cross(obb1.axisY,obb2.axisZ)) > 0 ||
				collisionProjection(obb1, obb2, glm::cross(obb1.axisZ,obb2.axisX)) > 0 ||
				collisionProjection(obb1, obb2, glm::cross(obb1.axisZ,obb2.axisY)) > 0 ||
				collisionProjection(obb1, obb2, glm::cross(obb1.axisZ,obb2.axisZ)) > 0
				);
}

bool World::collision(const OBP &obp1, const OBB &obb2){
	return !(
				collisionProjection(obp1, obb2, obp1.axisN) > 0 ||
				collisionProjection(obp1, obb2, obp1.axisE) > 0 ||
				collisionProjection(obp1, obb2, obp1.axisS) > 0 ||
				collisionProjection(obp1, obb2, obp1.axisW) > 0 ||
				collisionProjection(obp1, obb2, obb2.axisX) > 0 ||
				collisionProjection(obp1, obb2, obb2.axisY) > 0 ||
				collisionProjection(obp1, obb2, obb2.axisZ) > 0 ||
				collisionProjection(obp1, obb2, glm::cross(obp1.axisN,obb2.axisX)) > 0 ||
				collisionProjection(obp1, obb2, glm::cross(obp1.axisN,obb2.axisY)) > 0 ||
				collisionProjection(obp1, obb2, glm::cross(obp1.axisN,obb2.axisZ)) > 0 ||
				collisionProjection(obp1, obb2, glm::cross(obp1.axisE,obb2.axisX)) > 0 ||
				collisionProjection(obp1, obb2, glm::cross(obp1.axisE,obb2.axisY)) > 0 ||
				collisionProjection(obp1, obb2, glm::cross(obp1.axisE,obb2.axisZ)) > 0 ||
				collisionProjection(obp1, obb2, glm::cross(obp1.axisS,obb2.axisX)) > 0 ||
				collisionProjection(obp1, obb2, glm::cross(obp1.axisS,obb2.axisY)) > 0 ||
				collisionProjection(obp1, obb2, glm::cross(obp1.axisS,obb2.axisZ)) > 0 ||
				collisionProjection(obp1, obb2, glm::cross(obp1.axisW,obb2.axisX)) > 0 ||
				collisionProjection(obp1, obb2, glm::cross(obp1.axisW,obb2.axisY)) > 0 ||
				collisionProjection(obp1, obb2, glm::cross(obp1.axisW,obb2.axisZ)) > 0
				);
}
