#include "GE.h"
#include "maths_funcs.h"
#include <stdio.h>
#include <assert.h>

namespace graphics {
	// keep track of window size for things like the viewport and the mouse cursor
	int g_gl_width = 640;
	int g_gl_height = 480;
	char* GL_LOG_FILE = "gl.log";
	const bool debug = true;
}

int main() {
	using namespace graphics;

	// instantiate and initialize graphics engine, GL, logging and compiles shaders
	// RAII design
	GE ge;
	Camera& camera = ge.getCamera();

	/* OTHER STUFF GOES HERE NEXT */
	GLfloat points[] = {
		0.0f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f
	};

	GLfloat colours[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};

	GLfloat matrix[] = {
		1.0f, 0.0f, 0.0f, 0.0f, // first column
		0.0f, 1.0f, 0.0f, 0.0f, // second column
		0.0f, 0.0f, 1.0f, 0.0f, // third column
		0.5f, 0.0f, 0.0f, 1.0f // fourth column
	};


	GLuint points_vbo = create_vbo(points, 9);
	GLuint colours_vbo = create_vbo(colours, 9);
	GLuint vao = create_vao({ points_vbo, colours_vbo });

	ge.setUniformMatrix4fv("matrix", matrix);

	float speed = 1.0f; // move at 1 unit per second
	float last_position = 0.0f;

	while (!glfwWindowShouldClose(g_window)) {
		double elapsed_seconds = ge.elasped_time();

		// update events like input handling 
		glfwPollEvents();
		if (GLFW_PRESS == glfwGetKey(g_window, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(g_window, 1);
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

		// MOVE LEFT-RIGHT
		/*if (last_position >= 1.0f || last_position <= -1.0f)
			speed = -speed;
		matrix[12] = elapsed_seconds * speed + last_position;
		last_position = matrix[12];*/


		ge.update_fps_counter();
		ge.draw(vao);


		// put the stuff we've been drawing onto the display
		glfwSwapBuffers(g_window);
	}

	return 0;
}
