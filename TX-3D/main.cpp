#include "GE.h"

#include "graphics.h"
#include "obj_parser.h"
#include "Uniform.h"
#include "Camera.h"
#include "Mesh.h"
#include "Texture.h"
#include "Oculus.h"
#include "stb_image.h"
#include "Light.h"

#include <stdio.h>
#include <assert.h>
#include <iostream>

#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include "GLFW\glfw3native.h"

namespace graphics {
	// keep track of window size for things like the viewport and the mouse cursor
	int g_gl_width = 640;
	int g_gl_height = 480;
	char* GL_LOG_FILE = "gl.log";
}

void animateTy(std::shared_ptr<Mesh> mesh, double elapsed_seconds, float speed, float &current_speed, float limit);
void animateRy(std::shared_ptr<Mesh> mesh, double elapsed_seconds, float speed);
void animateDiffuseColor(std::shared_ptr<Light> light, double elapsed_seconds, float speed, int &step, float max, float min);

int main() {
	using namespace graphics;

	std::unique_ptr<Oculus> oculus_p;
	bool oculus_mode = false;
	char c;
	std::cout << "enter 'o' to launch oculus mode, enter something else for normal mode" << std::endl;
	std::cin >> c;
	if (c == 'o') {
		oculus_mode = true;
		oculus_p = std::unique_ptr<Oculus>(new Oculus);
	}
	Oculus& oculus = *oculus_p;
	if (oculus_mode && !oculus.isSupported()) {
		oculus_mode = false;
	}

	// instantiate and initialize graphics engine, GL, logging and compiles shaders
	// RAII design
	GE ge;
	Camera& camera = *(ge.getCamera());

	if (oculus_mode) {
		ge.setOculusWindowResolutionPosition(oculus.getResolution().w, oculus.getResolution().h, oculus.getWindowPos().x, oculus.getWindowPos().y);
		GLuint texId = ge.setOculusRenderToTexture(oculus.getTextureSize().w, oculus.getTextureSize().h);
		oculus.renderConfig(texId, glfwGetWin32Window(g_window));
	}


	// SCENE CREATION
	auto crate0 = std::shared_ptr<Mesh>(new Mesh("res/models/crate.obj", std::shared_ptr<Texture>(new Texture("res/textures/white.png", 4, 0))));
	crate0->setModel(translate(scale(crate0->getModelMat(), vec3(0.01, 0.03, 0.01)), vec3(2.5, 0.0, 0.0)));
	ge.addMesh(crate0);
	auto crate1 = std::shared_ptr<Mesh>(new Mesh("res/models/crate.obj", std::shared_ptr<Texture>(new Texture("res/textures/crate1.png", 4, 0))));
	crate1->setModel(translate(scale(crate1->getModelMat(), vec3(0.01, 0.01, 0.01)), vec3(1.25, 0.0, 1.25)));
	ge.addMesh(crate1);
	auto crate2 = std::shared_ptr<Mesh>(new Mesh("res/models/crate.obj", std::shared_ptr<Texture>(new Texture("res/textures/crate2.png", 4, 0))));
	crate2->setModel(translate(scale(crate2->getModelMat(), vec3(0.01, 0.01, 0.01)), vec3(-1.25, 0.0, 1.25)));
	ge.addMesh(crate2);
	auto crate3 = std::shared_ptr<Mesh>(new Mesh("res/models/crate.obj", std::shared_ptr<Texture>(new Texture("res/textures/crate3.png", 4, 0))));
	crate3->setModel(translate(scale(crate3->getModelMat(), vec3(0.01, 0.01, 0.01)), vec3(1.25, 0.0, -1.25)));
	ge.addMesh(crate3);
	auto crate4 = std::shared_ptr<Mesh>(new Mesh("res/models/crate.obj", std::shared_ptr<Texture>(new Texture("res/textures/crate4.png", 4, 0))));
	crate4->setModel(translate(scale(crate4->getModelMat(), vec3(0.01, 0.01, 0.01)), vec3(-1.25, 0.0, -1.25)));
	ge.addMesh(crate4);

	auto barrel1 = std::shared_ptr<Mesh>(new Mesh("res/models/barrel.obj", std::shared_ptr<Texture>(new Texture("res/textures/barrel1.png", 4, 0))));
	barrel1->setModel(translate(scale(barrel1->getModelMat(), vec3(0.01, 0.01, 0.01)), vec3(0.0, 0.0, 3)));
	ge.addMesh(barrel1);
	auto barrel2 = std::shared_ptr<Mesh>(new Mesh("res/models/barrel.obj", std::shared_ptr<Texture>(new Texture("res/textures/barrel2.png", 4, 0))));
	barrel2->setModel(translate(scale(barrel2->getModelMat(), vec3(0.01, 0.01, 0.01)), vec3(0.0, 0.0, -3)));
	ge.addMesh(barrel2);
	auto barrel3 = std::shared_ptr<Mesh>(new Mesh("res/models/barrel.obj", std::shared_ptr<Texture>(new Texture("res/textures/barrel3.png", 4, 0))));
	barrel3->setModel(translate(scale(barrel3->getModelMat(), vec3(0.01, 0.01, 0.01)), vec3(3, 0.0, 1.0)));
	ge.addMesh(barrel3);
	auto barrel4 = std::shared_ptr<Mesh>(new Mesh("res/models/barrel.obj", std::shared_ptr<Texture>(new Texture("res/textures/barrel4.png", 4, 0))));
	barrel4->setModel(translate(scale(barrel4->getModelMat(), vec3(0.01, 0.01, 0.01)), vec3(-3, 0.0, 0.0)));
	ge.addMesh(barrel4);

	auto car1 = std::shared_ptr<Mesh>(new Mesh("res/models/car.obj", std::shared_ptr<Texture>(new Texture("res/textures/car1.jpg", 4, 0))));
	car1->setModel(translate(scale(car1->getModelMat(), vec3(1.0, 1.0, 1.0)), vec3(15, 0.0, 0)));
	ge.addMesh(car1);
	auto car2 = std::shared_ptr<Mesh>(new Mesh("res/models/car.obj", std::shared_ptr<Texture>(new Texture("res/textures/car2.jpg", 4, 0))));
	car2->setModel(translate(scale(car2->getModelMat(), vec3(1.0, 1.0, 1.0)), vec3(15, 0.0, -10)));
	ge.addMesh(car2);
	auto car3 = std::shared_ptr<Mesh>(new Mesh("res/models/car.obj", std::shared_ptr<Texture>(new Texture("res/textures/car3.jpg", 4, 0))));
	car3->setModel(translate(scale(car3->getModelMat(), vec3(1.0, 1.0, 1.0)), vec3(10, 0.0, -5)));
	ge.addMesh(car3);
	auto car4 = std::shared_ptr<Mesh>(new Mesh("res/models/car.obj", std::shared_ptr<Texture>(new Texture("res/textures/car4.jpg", 4, 0))));
	car4->setModel(translate(scale(car4->getModelMat(), vec3(1.0, 1.0, 1.0)), vec3(20, 0.0, -5)));
	ge.addMesh(car4);

	// Simple square
	GLfloat points[] = {
		-0.5f, 0.0f, -0.5f,
		0.5f, 0.0f, 0.5f,
		0.5f, 0.0f, -0.5f,
		0.5f, 0.0f, 0.5f,
		-0.5f, 0.0f, -0.5f,
		- 0.5f, 0.0f, 0.5f
	};
	GLfloat normals[] = {
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};
	GLfloat texcoords[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f
	};
	auto grass = std::shared_ptr<Texture>(new Texture("res/textures/grass.png", 4, 0));

	int nb_square = 28; // should be a multiple of 4
	for (int i = -nb_square / 4; i < nb_square / 4; i++) {
		for (int j = -nb_square / 4; j < nb_square / 4; j++) {
			auto square = std::shared_ptr<Mesh>(new Mesh(points, normals, texcoords, 6, grass));
			square->setModel(translate(scale(square->getModelMat(), vec3(15, 15, 15)), vec3(i * 15.0, 0.0, j * 15.0)));
			ge.addMesh(square);
		}
	}

	auto yellow = std::shared_ptr<Texture>(new Texture("res/textures/yellow.png", 4, 0));
	auto bulb = std::shared_ptr<Mesh>(new Mesh("res/models/sphere.obj", yellow));
	bulb->setModel(translate(scale(bulb->getModelMat(), vec3(0.05, 0.05, 0.05)), vec3(0.0f, 3.0f, 0.0f)));
	ge.addMesh(bulb);

	ge.verify();

	// animation variables
	float speed_1 = 1.0, speed_2 = 2.0, speed_3 = 3.0, speed_4 = 4.0;
	float current_speed_1 = speed_1, current_speed_2 = speed_2, current_speed_3 = speed_3, current_speed_4 = speed_4;
	int colorAnimationStep = 0;

	while (!glfwWindowShouldClose(g_window)) {
		double elapsed_seconds;
		if (oculus_mode)
			elapsed_seconds = oculus.beginFrame();
		else
			elapsed_seconds = ge.elasped_time();

		// update events like input handling 
		glfwPollEvents();
		if (GLFW_PRESS == glfwGetKey(g_window, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(g_window, 1);
		}
		if (GLFW_PRESS == glfwGetKey(g_window, GLFW_KEY_SPACE)) {
			ge.logAll();
		}
		if (glfwGetKey(g_window, GLFW_KEY_Z)) {
			camera.moveForward(elapsed_seconds);
		}
		if (glfwGetKey(g_window, GLFW_KEY_S)) {
			camera.moveBackward(elapsed_seconds);
		}
		if (glfwGetKey(g_window, GLFW_KEY_Q)) {
			camera.moveLeft(elapsed_seconds);
		}
		if (glfwGetKey(g_window, GLFW_KEY_D)) {
			camera.moveRight(elapsed_seconds);
		}
		if (glfwGetKey(g_window, GLFW_KEY_PAGE_UP)) {
			camera.moveTop(elapsed_seconds);
		}
		if (glfwGetKey(g_window, GLFW_KEY_PAGE_DOWN)) {
			camera.moveBottom(elapsed_seconds);
		}
		if (oculus_mode) {
			if (glfwGetKey(g_window, GLFW_KEY_ENTER)) {
				oculus.recenter();
			}
			if (glfwGetKey(g_window, GLFW_KEY_BACKSPACE)) {
				oculus.dismissWarning();
			}
		}
		else {
			if (glfwGetKey(g_window, GLFW_KEY_LEFT)) {
				camera.yawLeft(elapsed_seconds);
			}
			if (glfwGetKey(g_window, GLFW_KEY_RIGHT)) {
				camera.yawRight(elapsed_seconds);
			}
			if (glfwGetKey(g_window, GLFW_KEY_UP)) {
				camera.pitchTop(elapsed_seconds);
			}
			if (glfwGetKey(g_window, GLFW_KEY_DOWN)) {
				camera.pitchBottom(elapsed_seconds);
			}
			if (glfwGetKey(g_window, GLFW_KEY_A)) {
				camera.rollLeft(elapsed_seconds);
			}
			if (glfwGetKey(g_window, GLFW_KEY_E)) {
				camera.rollRight(elapsed_seconds);
			}
		}
		
		// animations
		animateTy(barrel1, elapsed_seconds, speed_1, current_speed_1, 1.0);
		animateTy(barrel2, elapsed_seconds, speed_2, current_speed_2, 1.0);
		animateTy(barrel3, elapsed_seconds, speed_3, current_speed_3, 1.0);
		animateTy(barrel4, elapsed_seconds, speed_4, current_speed_4, 1.0);

		animateRy(crate1, elapsed_seconds, 90);
		animateRy(crate2, elapsed_seconds, -120);
		animateRy(crate3, elapsed_seconds, 150);
		animateRy(crate4, elapsed_seconds, -180);

		animateDiffuseColor(ge.getLight(), elapsed_seconds, 0.5, colorAnimationStep, 0.95, 0.1);


		if (oculus_mode)
		{
			ge.clearOculus();
			// eye 1
			camera.updateOculus(oculus.getOrientation(0), oculus.getPosition(0), oculus.getViewOffset(0), oculus.getProj(0));
			ge.drawOculusFromViewport(oculus.getViewportSize(0).w, oculus.getViewportSize(0).h, oculus.getViewportPos(0).x, oculus.getViewportPos(0).y);
			// eye 2
			camera.updateOculus(oculus.getOrientation(1), oculus.getPosition(1), oculus.getViewOffset(1), oculus.getProj(1));
			ge.drawOculusFromViewport(oculus.getViewportSize(1).w, oculus.getViewportSize(1).h, oculus.getViewportPos(1).x, oculus.getViewportPos(1).y);
			oculus.endFrame();
		}
		else 
		{
			camera.update();
			ge.update_fps_counter();
			ge.draw();
			glfwSwapBuffers(g_window);
		}
	}

	return 0;
}

// current_speed is changed in this function
// speed and limit > 0
void animateTy(std::shared_ptr<Mesh> mesh, double elapsed_seconds, float speed, float &current_speed, float limit)
{
	float s = 0.0;

	if (mesh->getModelMat().m[13] > 2*(limit+1.0)) s = -speed;
	else if (mesh->getModelMat().m[13] < 0.0) s = speed;
	else s = current_speed;
	mesh->setModel(translate(mesh->getModelMat(), vec3(0.0f, s * elapsed_seconds, 0.0f)));
	current_speed = s;
}

void animateRy(std::shared_ptr<Mesh> mesh, double elapsed_seconds, float speed)
{
	mesh->setModel(mat4(mesh->getModelMat()) * rotate_y_deg(identity_mat4(), speed * elapsed_seconds));
}

// step is changed by this function
// initialization with step=0
// 7 >= step >= 1 (7 different steps)
// min and max are extreme color values (between 0.0 and 1.0)
void animateDiffuseColor(std::shared_ptr<Light> light, double elapsed_seconds, float speed, int &step, float max, float min)
{
	float move = speed * elapsed_seconds;
	float tmp;
	vec3 result;

	switch (step)
	{
	case 0: // init : set first value to blue (min,min,max)
		result = vec3(min, min, max);
		step++;
		break;
	case 1: // G -> max  | we arrive at (min,min,max)
		tmp = light->getDiffuseLight().v[1] + move;
		if (tmp > max) {
			result = vec3(min, max, max);
			step++;
		}
		else {
			result = vec3(min, tmp, max);
		}
		break;
	case 2: // B -> min  | we arrive at (min,max,max)
		tmp = light->getDiffuseLight().v[2] - move;
		if (tmp < min) {
			result = vec3(min, max, min);
			step++;
		}
		else {
			result = vec3(min, max, tmp);
		}
		break;
	case 3: // R -> max  | we arrive at (min,max,min)
		tmp = light->getDiffuseLight().v[0] + move;
		if (tmp > max) {
			result = vec3(max, max, min);
			step++;
		}
		else {
			result = vec3(tmp, max, min);
		}
		break;
	case 4: // G -> min  | we arrive at (max,max,min)
		tmp = light->getDiffuseLight().v[1] - move;
		if (tmp < min) {
			result = vec3(max, min, min);
			step++;
		}
		else {
			result = vec3(max, tmp, min);
		}
		break;
	case 5: // B -> max  | we arrive at (max,min,min)
		tmp = light->getDiffuseLight().v[2] + move;
		if (tmp > max) {
			result = vec3(max, min, max);
			step++;
		}
		else {
			result = vec3(max, min, tmp);
		}
		break;
	case 6: // G -> max  | we arrive at (max,min,max)
		tmp = light->getDiffuseLight().v[1] + move;
		if (tmp > max) {
			result = vec3(max, max, max);
			step++;
		}
		else {
			result = vec3(max, tmp, max);
		}
		break;
	case 7: // R & G -> min  | we arrive at (max,max,max)
		tmp = light->getDiffuseLight().v[0] - move;
		if (tmp < min) {
			result = vec3(min, min, max);
			step = 1;
		}
		else {
			result = vec3(tmp, tmp, max);
		}
		break;
	}

	light->setDiffuseLight(result);
}