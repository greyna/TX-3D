#include "GE.h"

#include "graphics.h"
#include "obj_parser.h"
#include "Uniform.h"
#include "Camera.h"
#include "Mesh.h"
#include "Texture.h"
#include "Oculus.h"

#include <stdio.h>
#include <assert.h>
#include <iostream>


namespace graphics {
	// keep track of window size for things like the viewport and the mouse cursor
	int g_gl_width = 640;
	int g_gl_height = 480;
	char* GL_LOG_FILE = "gl.log";
}

void animateY(std::shared_ptr<Mesh> mesh, double elapsed_seconds, float speed, float &current_speed, float limit);

int main() {
	using namespace graphics;

	Oculus oculus;

	// instantiate and initialize graphics engine, GL, logging and compiles shaders
	// RAII design
	GE ge;
	Camera& camera = *(ge.getCamera());


	auto t1 = std::shared_ptr<Texture>(new Texture("skulluvmap.png", 4, 0));
	auto t2 = std::shared_ptr<Texture>(new Texture("arthur_texture.png", 4, 0));

	auto sphere1 = std::shared_ptr<Mesh>(new Mesh("sphere.obj", t2));
	auto sphere2 = std::shared_ptr<Mesh>(new Mesh("sphere.obj", t1));
	sphere2->setModel(translate(identity_mat4(), vec3(3.5f, 0.0f, 0.0f)));
	auto sphere3 = std::shared_ptr<Mesh>(new Mesh("sphere.obj", t1));
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


	ge.verify();

	// animation variables
	float speed_1 = 2.0, speed_2 = 4.0, speed_3 = 7.0;
	float current_speed_1 = speed_1, current_speed_2 = speed_2, current_speed_3 = speed_3;

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

		// animate
		animateY(sphere1, elapsed_seconds, speed_1, current_speed_1, 2.0);
		animateY(sphere2, elapsed_seconds, speed_2, current_speed_2, 1.5);
		animateY(sphere3, elapsed_seconds, speed_3, current_speed_3, 3.0);

		// draw
		ge.update_fps_counter();
		ge.draw();

		// put the stuff we've been drawing onto the display
		glfwSwapBuffers(g_window);
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