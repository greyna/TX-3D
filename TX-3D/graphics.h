#pragma once
#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <string>

namespace graphics {
	// flag for logging
	extern const bool debug;

	const struct Uniform {
		std::string name;
		const float* value;
		int location;
		Uniform(std::string name, const float* value, int location)
			: name(name), value(value), location(location) {}
	};

	// keep track of window size for things like the viewport and the mouse cursor
	extern int g_gl_width;
	extern int g_gl_height;
	extern GLFWwindow* g_window;
	extern char* GL_LOG_FILE;

	/*--------------------------------GLFW3 and GLEW------------------------------*/
	bool start_gl();
	void glfw_error_callback(int error, const char* description);
	void glfw_window_size_callback(GLFWwindow* window, int width, int height);

	/*--------------------------------LOG FUNCTIONS-------------------------------*/
	bool restart_gl_log();
	bool gl_log(const char* message, ...);
	bool gl_log_err(const char* message, ...);
	void log_gl_params();
}