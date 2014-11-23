#include "GE.h"

#include "graphics.h"
#include "obj_parser.h"
#include "Uniform.h"
#include "Camera.h"

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

	/*------------------------------CREATE GEOMETRY-------------------------------*/

	// Sphere
	GLfloat* vp = NULL; // array of vertex points
	GLfloat* vn = NULL; // array of vertex normals
	GLfloat* vt = NULL; // array of texture coordinates
	int point_count = 0;
	assert(load_obj_file("sphere.obj", vp, vt, vn, point_count));
	ge.setPoint_count(point_count);
	GLuint sphere_points_vbo = create_vbo(vp, point_count * 3);
	GLuint sphere_normals_vbo = create_vbo(vn, point_count * 3);
	GLuint vao = create_vao({ sphere_points_vbo, sphere_normals_vbo });

	// Triangle
	/*GLfloat points[] = {
		0.0f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f
	};
	GLfloat colours[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};
	GLuint points_vbo = create_vbo(points, 9);
	GLuint colours_vbo = create_vbo(colours, 9);
	ge.setPoint_count(3);
	GLuint vao = create_vao({ points_vbo, colours_vbo });
	GLuint vao = create_vao({ points_vbo });*/

	GLfloat model[] = {
		1.0f, 0.0f, 0.0f, 0.0f, // first column
		0.0f, 1.0f, 0.0f, 0.0f, // second column
		0.0f, 0.0f, 1.0f, 0.0f, // third column
		0.5f, 0.0f, 0.0f, 1.0f // fourth column
	};
	//ge.setUniformMatrix4fv("model", model);
	auto model_uniform = Uniform::createUniformMatrix4fv("model", model);
	ge.setUniform(model_uniform);
	ge.verify();
	//ge.logAll();

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

		// MOVE triangle LEFT-RIGHT
		/*if (last_position >= 1.0f || last_position <= -1.0f)
			speed = -speed;
		matrix[12] = elapsed_seconds * speed + last_position;
		last_position = matrix[12];*/


		ge.update_fps_counter();
		ge.draw(vao);


		// put the stuff we've been drawing onto the display
		glfwSwapBuffers(g_window);
	}

	// Free load_obj_file function mallocs
	free(vp);
	free(vn);
	free(vt);

	return 0;
}
