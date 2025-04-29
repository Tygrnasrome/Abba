#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "utils.h"

#include "shader.h"
#include "box.h"
#include "world.h"
#include "pyramid.h"
#include "player.h"
#include "quad.h"
#include "projectile.h"
#include "framebuffer.h"

using namespace std;
int i = 25;
GLFWwindow * glfw_window = NULL;
bool keep_running = true;

float angle=0;
float angle2=0;
float angle3=0;

Pyramid *Pyramid;
Box *box, *zdroj_svetla;
Shader *shader, *shading_shader, *frame_shader, *blur_shader, *greyscale_shader, *blender_shader, *snoice_shader, *waves_shader, *irradiance_shader;
World *world;
Framebuffer *framebuffer, *pingpongbuffer[2], *refraction, *reflection, *hdr_buffer, *brdf_buffer;
Quad* scene, *water,*underWaterGround;
CubeMap *irradianceMap, *prefilterMap;


float mouseX;
float mouseY;
float mouseXV;
float mouseYV;

int attachment = 2;
bool cursorGrab = true;
int width;
int height;
void glfw_error_callback(int error, const char * description)
{
	cout << "GLFW error " << error << ": " << description << endl;
}

void glfw_key_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
{

	/*	vypnutí programu esc	*/
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		keep_running = false;
	}
	if (key == GLFW_KEY_H && action == GLFW_PRESS)
	{
		attachment++;
		attachment == 3 ? attachment = 0:attachment=attachment;
	}

	world->key(key, action);

}

void glfw_mouse_click_callback(GLFWwindow * window, int button, int action, int mods)
{
	world->mouseClick(button,action);
}

void glfw_mouse_move_callback(GLFWwindow * window, double xpos, double ypos)
{
	mouseX = xpos;
	mouseY = ypos;
	/*	mouse move callback pro aktivní pohled		*/

	world->mouseMove(xpos,ypos);

}

bool createWindow(int width, int height, string title, bool vsync, bool core = true)
{


	if (glfw_window) return false;/* Modelová matice umístí objekt ze svých lokálních souřadnic s nulou uprostřed do světa. */
	if (!glfwInit()) return false;

	glfwSetErrorCallback(glfw_error_callback);

	if(core)
	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	}
	glfwWindowHint(GLFW_SAMPLES,4);
	// create window using GLFW
	if (!(glfw_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL)))
	{
		glfwTerminate();
		return false;
	}

	cout << "GLFW " << glfwGetVersionString() << endl;

	glfwSetKeyCallback(glfw_window, glfw_key_callback);
	glfwSetMouseButtonCallback(glfw_window, glfw_mouse_click_callback);
	glfwSetCursorPosCallback(glfw_window, glfw_mouse_move_callback);

	glfwMakeContextCurrent(glfw_window);

	if (vsync) glfwSwapInterval(1); // <-- VSYNC on

	cout << "OpenGL version: " << glGetString(GL_VERSION) << endl;
	cout << "Renderer: " << glGetString(GL_RENDERER) << endl;
	cout << "Vendor: " << glGetString(GL_VENDOR) << endl;
	cout << "Shading language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

	// load OpenGL functions using GLEW
	if (glewInit() != GLEW_OK)
	{
		glfwDestroyWindow(glfw_window);
		glfw_window = NULL;
		glfwTerminate();
	}
	glfwSetInputMode(glfw_window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);
	cout << "GLEW" << endl;

	return true;
}


void drawScene(double dt)
{
	world->drawSky();
	world->drawObj(dt);
}

void init()
{
	glfwGetWindowSize(glfw_window,&width,&height);

	/* Barva pozadí */
	glClearColor(0.1, 0.1, 0.15, 1);

	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);



	world = new World();


	framebuffer = new Framebuffer(1280,720, GL_RGBA16F, 2, GL_TEXTURE2);
	reflection = new Framebuffer(1280,720, GL_RGBA, 1,GL_TEXTURE4);
	refraction = new Framebuffer(1280,720, GL_RGBA, 1, GL_TEXTURE5);
	pingpongbuffer[0] = new Framebuffer(1280,720, GL_RGBA16F);
	pingpongbuffer[1] = new Framebuffer(1280,720, GL_RGBA16F);

	//necessary
	frame_shader = new Shader("shaders/framebuffer.vert", "shaders/framebuffer.frag");
	blender_shader = new Shader("shaders/framebuffer.vert", "shaders/blender.frag");
	blur_shader = new Shader("shaders/framebuffer.vert", "shaders/blurShader.frag");
	irradiance_shader = new Shader("shaders/sky.vert", "shaders/irradiance.frag");
	//for effect
	greyscale_shader = new Shader("shaders/framebuffer.vert", "shaders/greyscale.frag");

	scene = new Quad();

	snoice_shader = new Shader("shaders/shadingTexture.vert", "shaders/snoice.frag");
	waves_shader = new Shader("shaders/shadingTexture.vert", "shaders/wave.frag");
	water = new Quad();
	water->place(glm::vec3(1,0,1));
	water->scale(world->objectArea,world->objectArea,world->objectArea);
	{
		snoice_shader->use();
		snoice_shader->uniform("viewPos", world->camera->position);

		snoice_shader->uniform("angle", angle);

		snoice_shader->uniform("light.ambient", 0.5,0.5,0.5);
		snoice_shader->uniform("light.diffuse", 0.5,0.5,0.5);
		snoice_shader->uniform("light.specular", 0.5,0.5,0.5);

		snoice_shader->uniform("material.ambient", 0.5,0.5,0.85);
		snoice_shader->uniform("tileNum",glm::vec2(1,1));

		snoice_shader->uniform("material.diffuse", 0.15,0.45,0.85);
		snoice_shader->uniform("material.specular", 0.15,0.45,0.85);
		snoice_shader->uniform("material.shininess", 42);
	}
	underWaterGround = new Quad();
	underWaterGround->place(glm::vec3(1,-1,1));
	water->pitch(-90);
	underWaterGround->scale(world->objectArea,world->objectArea,world->objectArea);

	/*
	 *   Pro kreslení scény připravíme HDR buffer pracující s float hodnotami,
	 *   kam můžeme kreslit bez ztráty informace i svítící střely nebo odlesky
	 *   slunce na kovovém povrchu s vyšším jasem. Až nakonec při kreslení na
	 *   obrazovku provedeme mapování na RGB 0-255 a přesvícené části změníme
	 *   na blur.
	 */
	hdr_buffer = new Framebuffer(1280,720, GL_RGBA16F, 2);

	/* PBR - připravíme view matice pro zachycení projekce na cubemapu okolí. */
	glm::mat4 captureViews[] =
	{
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};

	/*
	 *   PBR - vygenerujeme osvětlovací mapu pro osvětlení difusním IBL. To
	 *   je vlastně textura vzniklá z obrazu oblohy. Když se podíváme určitým
	 *   směrem na cubemapu s oblohou, dostaneme barvu pixelu v daném směru.
	 *   Na libovolný pixel na objektu ale dopadá světlo ne z jednoho pixelu
	 *   oblohy, ale rovnou ze všech po celé polokouli oblohy. Nejvíce z pixelu
	 *   kolmo nad plochou objektu, z pixelů stranou pak méně. V osvětlovací
	 *   mapě si sečteme všechno světlo dopadající na určitý pixel objektu.
	 */
	irradianceMap = new CubeMap(32, GL_TEXTURE10, GL_LINEAR);
	Framebuffer capture2(32, 32, 0, 0, 0);

	irradiance_shader->use();
	irradiance_shader->texture("environmentMap", world->sky);
	irradiance_shader->uniformMatrix("projection", glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f));

	glViewport(0, 0, 32, 32);
	capture2.bind();
	for(unsigned int i = 0; i < 6; i++)
	{
	    irradiance_shader->uniformMatrix("view", captureViews[i]);
	    capture2.attach(irradianceMap, i, 0);
	    capture2.clear();

	    world->skybox->draw(irradiance_shader, NULL);
	}
	capture2.unbind();

//	/*
//	 *   PBR - odraz okolí, který potřebujeme pro spekulární část IBL, vykreslíme
//	 *   do cubemapy a to pro několik různých úrovních drsnosti materiálu (roughness),
//	 *   pro které jsou postupně více a více rozmazané. Tyto úrovně jsou uloženy v mip
//	 *   mapách.
//	 */
//	Framebuffer capture(512, 512);
//	prefilterMap = new CubeMap(512, GL_TEXTURE9);

//	Shader* prefilterShader = new Shader("shaders/prefilter.vert", "shaders/prefilter.frag");
//	prefilterShader->use();
//	prefilterShader->uniformMatrix("projection", glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f));
//	prefilterShader->texture("environmentMap", world->sky);

//	capture.bind();
//	unsigned int maxMipLevels = 5;
//	for(unsigned int mip = 0; mip < maxMipLevels; ++mip)
//	{
//		capture.resize(512 >> mip, 512 >> mip);
//		prefilterShader->uniform("roughness", (float)mip / (maxMipLevels - 1));

//		for(unsigned int i = 0; i < 6; ++i)
//		{
//			prefilterShader->uniformMatrix("view", captureViews[i]);

//			capture.attach(prefilterMap, i, mip);
//			capture.clear();

//			world->skybox->draw(prefilterShader, NULL);
//		}
//	}
//	capture.unbind();

//	/* PBR - vygenerujeme 2D LUT obsahující výsledky BRDF rovnic. */
//	brdf_buffer = new Framebuffer(512, 512, GL_RG16F, 2, GL_TEXTURE8);

//	glViewport(0, 0, 512, 512);
//	brdf_buffer->clear();

//	brdf_shader->use();
//	screen_quad->draw(brdf_shader, world->camera);

//	brdf_buffer->unbind();

	/* Vrátíme normální celoobrazovkový viewport. */
	glViewport(0, 0,1280,720);
}


void displayFrameBuffers()
{
	if (attachment == 0 || attachment == 1)
	{
		frame_shader->use();
		frame_shader->texture("screenTexture", framebuffer->texture[attachment]);
		glDisable(GL_DEPTH_TEST);
		scene->draw(frame_shader);
		glEnable(GL_DEPTH_TEST);
	}
	if (attachment == 2)
	{
		blender_shader->use();
		blender_shader->texture("scene", framebuffer->texture[0]);			//zobrazení scény
		blender_shader->texture("bloomBlur", pingpongbuffer[1]->texture[0]);		//přidáme rozmazané střely/světlé prvky
		blender_shader->uniform("exposure", 1.5);					//kdo ví
		glDisable(GL_DEPTH_TEST);
		scene->draw(blender_shader);
		glEnable(GL_DEPTH_TEST);
	}
}

void render(double dt)
{
	angle += dt * 80 ;
	world->update();
	world->pbr_shader->use();
	world->pbr_shader->texture("irradianceMap", irradianceMap);


	blur_shader->use();

	int amount = 6;
	bool horizontal = true, first_iteration = true;
	for(int i = 0; i < amount; i++)
	{
		pingpongbuffer[horizontal]->bind();
		pingpongbuffer[horizontal]->clear();

		blur_shader->uniformInt("horizontal", horizontal);
		blur_shader->texture("image", first_iteration ? framebuffer->texture[1] : pingpongbuffer[!horizontal]->texture[0]);
		scene->draw(blur_shader);

		horizontal = !horizontal;
		first_iteration = false;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// kresleni jezirek

	glEnable(GL_STENCIL_TEST);
	glStencilMask(0xFF);

	/* Odraz */
	reflection->bind();
	reflection->clear();
	{
		/* Hodnotu přepíšeme vždy bez ohledu na Z buffer. */
		glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);

		/* Nyní nechceme stencil buffer testovat, ale přepsat vždy na 1. */
		glStencilFunc(GL_ALWAYS, 1, 0xFF);

		/* Do color a z-bufferů nyní nekreslíme, jen do stencilu. */
		glColorMask(0x00, 0x00, 0x00, 0x00);
		glDepthMask(GL_FALSE);

		/*
		 *   Nakreslíme tvar jezírek pomocí shaderu kreslícího simplexový
		 *   šum. On prostě kreslí pixely, kde snoise(uv) >= 0.55, ostatní
		 *   discardne, proto nic nenakreslí do stencilu mimo samotných jezírek.
		 */
		snoice_shader->use();
		water->draw(snoice_shader, world->camera);

		/* Opět zapneme kreslení barev a z-bufferu. */
		glDepthMask(GL_TRUE);
		glColorMask(0xFF, 0xFF, 0xFF, 0xFF);

		/* Stencil nyní ochráníme proti zápisu. */
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

		/*
		 *   A zapneme jeho testování, aby posloužil jako šablona.
		 *   Kreslit se bude jen tam, kde je ve stencilu jednička.
		 */
		glStencilFunc(GL_EQUAL, 1, 0xFF);	// kreslit chci, kdyz najdu jednicku

		/* Ozrcadlíme svět podle osy Y a vykreslíme ho. */
		world->camera->view = glm::scale(world->camera->view, glm::vec3(1.0f, -1.0f, 1.0f));
		drawScene(dt);
		world->camera->view = glm::scale(world->camera->view, glm::vec3(1.0f, -1.0f, 1.0f));
	}
	reflection->unbind();
	/* pod hladinou */
	refraction->bind();
	refraction->clear();
	{
		/* Hodnotu přepíšeme vždy bez ohledu na Z buffer. */
		glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);

		/* Nyní nechceme stencil buffer testovat, ale přepsat vždy na 1. */
		glStencilFunc(GL_ALWAYS, 1, 0xFF);

		/* Do color a z-bufferů nyní nekreslíme, jen do stencilu. */
		glColorMask(0x00, 0x00, 0x00, 0x00);
		glDepthMask(GL_FALSE);

		/*
		 *   Nakreslíme tvar jezírek pomocí shaderu kreslícího simplexový
		 *   šum. On prostě kreslí pixely, kde snoise(uv) >= 0.55, ostatní
		 *   discardne, proto nic nenakreslí do stencilu mimo samotných jezírek.
		 */
		snoice_shader->use();
		water->draw(snoice_shader, world->camera);

		/* Opět zapneme kreslení barev a z-bufferu. */
		glDepthMask(GL_TRUE);
		glColorMask(0xFF, 0xFF, 0xFF, 0xFF);

		/* Stencil nyní ochráníme proti zápisu. */
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

		/*
		 *   A zapneme jeho testování, aby posloužil jako šablona.
		 *   Kreslit se bude jen tam, kde je ve stencilu jednička.
		 */
		glStencilFunc(GL_EQUAL, 1, 0xFF);	// kreslit chci, kdyz najdu jednicku

		//kreslení podvodni podlahy
		underWaterGround->pitch(-90);
		world->pbr_shader->use();
		world->pbr_shader->texturePack({"albedoMap","aoMap","normalMap","height_map", "metallicMap","opacityMap", "roughnessMap"}, world->texpaxLakeBottom);
		world->pbr_shader->uniform("uv_repeat",glm::vec2(world->objectArea/2,world->objectArea/2));
		underWaterGround->draw(world->pbr_shader, world->camera);
		world->pbr_shader->uniform("uv_repeat",glm::vec2(1,1));

	}
	refraction->unbind();
	glDisable(GL_STENCIL_TEST);

	framebuffer->bind();
	framebuffer->clear();
	world->draw(dt);
	/* Vodní hladina do scény */
	waves_shader->use();
	waves_shader->uniform("angle", angle / 10);
	waves_shader->uniform("viewPos", world->camera->position);
	waves_shader->texture("reflectionMap", reflection->texture[0]);
	waves_shader->texture("refractionMap", refraction->texture[0]);

	water->draw(waves_shader, world->camera);
	framebuffer->unbind();

	if(world->player->holdingFlag == FLAG_GREYSCALE)
	{
		greyscale_shader->use();
		greyscale_shader->texture("screenTexture", framebuffer->texture[0]);

		glDisable(GL_DEPTH_TEST);
		scene->draw(greyscale_shader);
		glEnable(GL_DEPTH_TEST);
	} else
		if(world->player->holdingFlag == FLAG_BLUR)
		{
			blur_shader->use();
			blur_shader->texture("screenTexture", framebuffer->texture[0]);

			glDisable(GL_DEPTH_TEST);
			scene->draw(blur_shader);
			glEnable(GL_DEPTH_TEST);
		} else
		{
			displayFrameBuffers();
		}
	glUseProgram(0);
	glBindVertexArray(0);
}




int main()
{
	if(!createWindow(1280, 720, "OpenGL", true, true)) return -1;

	init();

	/* Hlavní smyčka */
	double delta = 0;
	double old_time = glfwGetTime();
	double total_time = 0;
	int ticks = 0;
	double max_delta = 0;
	while (keep_running && !glfwWindowShouldClose(glfw_window))
	{
		// process input
		glfwPollEvents();

		// render frame
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		render(delta);
		glfwSwapBuffers(glfw_window);

		// measure how long it took
		double new_time = glfwGetTime();
		delta = new_time - old_time;
		old_time = new_time;

		ticks++;
		total_time += delta;

		if(delta > max_delta)
			max_delta = delta;

		// write out FPS
		if(total_time > 3.0)
		{
			double fps = ticks / total_time;
			cout << "FPS: " << fps << " (" << (total_time / ticks * 1000) << " ms), max " << (max_delta * 1000) << " ms" << endl;

			total_time = 0;
			ticks = 0;
			max_delta = 0;
		}
	}

	glfwDestroyWindow(glfw_window);
	glfw_window = NULL;

	glfwTerminate();

	cout << "Finished." << endl;
	return 0;
}
