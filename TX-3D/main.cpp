#include "GE.h"

#include "graphics.h"
#include "obj_parser.h"
#include "Uniform.h"
#include "Camera.h"
#include "Mesh.h"
#include "Texture.h"

#include <stdio.h>
#include <assert.h>

namespace graphics {
	// keep track of window size for things like the viewport and the mouse cursor
	int g_gl_width = 640;
	int g_gl_height = 480;
	char* GL_LOG_FILE = "gl.log";
}

int main() {
	using namespace graphics;

	// instantiate and initialize graphics engine, GL, logging and compiles shaders
	// RAII design
	GE ge;
	Camera& camera = *(ge.getCamera());

	
	auto sphere1 = std::shared_ptr<Mesh>(new Mesh("sphere.obj"));
	auto sphere2 = std::shared_ptr<Mesh>(new Mesh("sphere.obj"));
	sphere2->setModel(translate(identity_mat4(), vec3(3.5f, 0.0f, 0.0f)));
	auto sphere3 = std::shared_ptr<Mesh>(new Mesh("sphere.obj"));
	sphere3->setModel(translate(identity_mat4(), vec3(-3.5f, 0.0f, 0.0f)));

	ge.addMesh(sphere1);
	ge.addMesh(sphere2);
	ge.addMesh(sphere3);
	

	// Simple square
	/*GLfloat points[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f
	};
	GLfloat normals[] = {
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f
	};
	GLfloat texcoords[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f
	};
	auto carre = std::shared_ptr<Mesh>(new Mesh(points, normals, texcoords, 6));
	ge.addMesh(carre);*/

	//Texture t("skulluvmap.png", 4, 0);
	Texture t("arthur_texture.png", 4, 0);

	ge.verify();

	while (!glfwWindowShouldClose(g_window)) {
		double elapsed_seconds = ge.elasped_time();

		// update events like input handling 
		glfwPollEvents();
		if (GLFW_PRESS == glfwGetKey(g_window, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(g_window, 1);
		}
		if (GLFW_PRESS == glfwGetKey(g_window, GLFW_KEY_F5)) {
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

		ge.update_fps_counter();
		ge.draw();

		// put the stuff we've been drawing onto the display
		glfwSwapBuffers(g_window);
	}

	return 0;
}
