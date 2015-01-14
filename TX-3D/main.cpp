#include "GE.h"

#include "graphics.h"
#include "obj_parser.h"
#include "Uniform.h"
#include "Camera.h"
#include "Mesh.h"
#include "Texture.h"
#include "Oculus.h"
#include "stb_image.h"

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

void animateY(std::shared_ptr<Mesh> mesh, double elapsed_seconds, float speed, float &current_speed, float limit);

int main() {
	using namespace graphics;

	bool oculus_mode = true;/*false;
	char c;
	std::cout << "enter 'o' to launch oculus mode, enter something else for normal mode" << std::endl;
	std::cin >> c;
	if (c == 'o') {
		oculus_mode = true;
	}*/
	Oculus oculus;
	if (!oculus.isSupported()) {
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


	auto t1 = std::shared_ptr<Texture>(new Texture("res/textures/skulluvmap.png", 4, 0));
	auto t2 = std::shared_ptr<Texture>(new Texture("res/textures/arthur_texture.png", 4, 0));

	auto sphere1 = std::shared_ptr<Mesh>(new Mesh("res/models/sphere.obj", t2));
	auto sphere2 = std::shared_ptr<Mesh>(new Mesh("res/models/sphere.obj", t1));
	sphere2->setModel(translate(identity_mat4(), vec3(3.5f, 0.0f, 0.0f)));
	auto sphere3 = std::shared_ptr<Mesh>(new Mesh("res/models/sphere.obj", t1));
	sphere3->setModel(translate(identity_mat4(), vec3(-3.5f, 0.0f, 0.0f)));
	
	ge.addMesh(sphere1);
	ge.addMesh(sphere2);
	ge.addMesh(sphere3);

	auto t3 = std::shared_ptr<Texture>(new Texture("res/textures/fskin.jpg", 4, 0));
	auto obj = std::shared_ptr<Mesh>(new Mesh("res/models/f360.obj", t3));
	obj->setModel(translate(scale(obj->getModelMat(), vec3(0.7, 0.7, 0.7)), vec3(1.5, 0.0, -5.0)));
	ge.addMesh(obj);

	/*auto t3 = std::shared_ptr<Texture>(new Texture("res/textures/metal.png", 4, 0));
	auto obj = std::shared_ptr<Mesh>(new Mesh("res/models/audi.obj", t3));
	obj->setModel(translate(scale(obj->getModelMat(), vec3(0.7, 0.7, 0.7)), vec3(0.0, 0.0, 0.0)));
	ge.addMesh(obj);*/

	ge.verify();

	// animation variables
	float speed_1 = 2.0, speed_2 = 4.0, speed_3 = 7.0, speed_4 = 4.0;
	float current_speed_1 = speed_1, current_speed_2 = speed_2, current_speed_3 = speed_3, current_speed_4 = speed_4;

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
		animateY(sphere1, elapsed_seconds, speed_1, current_speed_1, 2.0);
		animateY(sphere2, elapsed_seconds, speed_2, current_speed_2, 1.5);
		animateY(sphere3, elapsed_seconds, speed_3, current_speed_3, 3.0);
		animateY(obj, elapsed_seconds, speed_4, current_speed_4, 4.0);


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
void animateY(std::shared_ptr<Mesh> mesh, double elapsed_seconds, float speed, float &current_speed, float limit)
{
	float s = 0.0;

	if (mesh->getModelMat().m[13] > limit) s = -speed;
	else if (mesh->getModelMat().m[13] < -limit) s = speed;
	else s = current_speed;
	mesh->setModel(translate(mesh->getModelMat(), vec3(0.0f, s * elapsed_seconds, 0.0f)));
	current_speed = s;
}