#include "graphics.h"
#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include <stdarg.h>
#include <iostream>

namespace graphics {
	GLFWwindow* g_window = NULL;

	/*--------------------------------GLFW3 and GLEW------------------------------*/
	bool start_gl() {
		gl_log("starting GLFW %s", glfwGetVersionString());

		glfwSetErrorCallback(glfw_error_callback);
		if (!glfwInit()) {
			fprintf(stderr, "ERROR: could not start GLFW3\n");
			return false;
		}

		// These lines are for fullscreen
		/*GLFWmonitor* mon = glfwGetPrimaryMonitor ();
		const GLFWvidmode* vmode = glfwGetVideoMode (mon);
		g_window = glfwCreateWindow (
		vmode->width, vmode->height, "Extended GL Init", mon, NULL
		);*/

		g_window = glfwCreateWindow(
			g_gl_width, g_gl_height, "Extended Init.", NULL, NULL
			);
		if (!g_window) {
			fprintf(stderr, "ERROR: could not open window with GLFW3\n");
			glfwTerminate();
			return false;
		}
		glfwSetWindowSizeCallback(g_window, glfw_window_size_callback);
		glfwMakeContextCurrent(g_window);

		glfwWindowHint(GLFW_SAMPLES, 4);

		// start GLEW extension handler
		glewExperimental = GL_TRUE;
		glewInit();

		// get version info
		const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
		const GLubyte* version = glGetString(GL_VERSION); // version as a string
		printf("Renderer: %s\n", renderer);
		printf("OpenGL version supported %s\n", version);
		gl_log("renderer: %s\nversion: %s\n", renderer, version);

		return true;
	}

	void glfw_error_callback(int error, const char* description) {
		fputs(description, stderr);
		std::cerr << description << std::endl;
		gl_log("%s\n", description);
	}
	// a call-back function
	void glfw_window_size_callback(GLFWwindow* window, int width, int height) {
		g_gl_width = width;
		g_gl_height = height;
		printf("width %i height %i\n", width, height);
		/* update any perspective matrices used here */
	}


	/*--------------------------------LOG FUNCTIONS-------------------------------*/
	//TODO refactor in one class see http://www.cprogramming.com/tutorial/c++-iostreams.html
	bool restart_gl_log() {
		FILE* file = fopen(GL_LOG_FILE, "w");
		if (!file) {
			fprintf(
				stderr,
				"ERROR: could not open GL_LOG_FILE log file %s for writing\n",
				GL_LOG_FILE
				);
			return false;
		}
		time_t now = time(NULL);
		char* date = ctime(&now);
		fprintf(file, "GL_LOG_FILE log. local time %s\n", date);
		fclose(file);
		return true;
	}

	bool gl_log(const char* message, ...) {
		va_list argptr;
		FILE* file = fopen(GL_LOG_FILE, "a");
		if (!file) {
			fprintf(
				stderr,
				"ERROR: could not open GL_LOG_FILE %s file for appending\n",
				GL_LOG_FILE
				);
			return false;
		}
		va_start(argptr, message);
		vfprintf(file, message, argptr);
		va_end(argptr);
		fclose(file);
		return true;
	}
	/* we can use a function like this to print some GL capabilities of our adapter
	to the log file. handy if we want to debug problems on other people's computers
	*/
	void log_gl_params() {
		int i;
		int v[2];
		unsigned char s = 0;
		GLenum params[] = {
			GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
			GL_MAX_CUBE_MAP_TEXTURE_SIZE,
			GL_MAX_DRAW_BUFFERS,
			GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,
			GL_MAX_TEXTURE_IMAGE_UNITS,
			GL_MAX_TEXTURE_SIZE,
			GL_MAX_VARYING_FLOATS,
			GL_MAX_VERTEX_ATTRIBS,
			GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,
			GL_MAX_VERTEX_UNIFORM_COMPONENTS,
			GL_MAX_VIEWPORT_DIMS,
			GL_STEREO,
		};
		const char* names[] = {
			"GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS",
			"GL_MAX_CUBE_MAP_TEXTURE_SIZE",
			"GL_MAX_DRAW_BUFFERS",
			"GL_MAX_FRAGMENT_UNIFORM_COMPONENTS",
			"GL_MAX_TEXTURE_IMAGE_UNITS",
			"GL_MAX_TEXTURE_SIZE",
			"GL_MAX_VARYING_FLOATS",
			"GL_MAX_VERTEX_ATTRIBS",
			"GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS",
			"GL_MAX_VERTEX_UNIFORM_COMPONENTS",
			"GL_MAX_VIEWPORT_DIMS",
			"GL_STEREO",
		};
		gl_log("\nGL Context Params:\n");
		// integers - only works if the order is 0-10 integer return types
		for (i = 0; i < 10; i++) {
			int v = 0;
			glGetIntegerv(params[i], &v);
			gl_log("%s = %i\n", names[i], v);
		}
		// others
		v[0] = v[1] = 0;
		glGetIntegerv(params[10], v);
		gl_log("%s = %i, %i\n", names[10], v[0], v[1]);
		glGetBooleanv(params[11], &s);
		gl_log("%s = %i\n", names[11], (unsigned int)s);
		gl_log("-----------------------------\n");
	}

}