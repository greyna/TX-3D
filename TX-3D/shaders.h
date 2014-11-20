#pragma once

#include "graphics.h"

namespace graphics {
	// Validation
	bool is_programme_valid(GLuint sp);

	// Logging and debugging
	void print_shader_info_log(GLuint shader_index);
	void print_programme_info_log(GLuint sp);
	void print_all(GLuint sp);

	// Conveniency functions
	const char* GL_type_to_string(GLenum type);
}