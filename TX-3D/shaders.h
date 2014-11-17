#pragma once

#include "graphics.h"

namespace graphics {
	const int MAX_SHADER_LENGTH = 262144;

	// shaders creation
	bool create_shader(const char* file_name, GLuint* shader, GLenum type);
	//bool create_programme(GLuint vert, GLuint frag, GLuint* programme);
	//GLuint create_programme_from_files(
		//const char* vert_file_name, const char* frag_file_name);

	// Validation
	bool is_programme_valid(GLuint sp);

	// Logging and debugging
	void print_shader_info_log(GLuint shader_index);
	void print_programme_info_log(GLuint sp);
	void print_all(GLuint sp);

	// Conveniency functions
	bool parse_file_into_str(
		const char* file_name, char* shader_str, int max_len);
	const char* GL_type_to_string(GLenum type);
}