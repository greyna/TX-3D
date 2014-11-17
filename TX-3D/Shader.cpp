#include "Shader.h"
#include <stdio.h>
#include <assert.h>
#include <stdexcept>
#include <string>


namespace graphics {
	Shader::Shader(Type type, const char* file_name) : type(type)
	{
		char shader_string[1024 * 256];
		assert(parse_file_into_str(file_name, shader_string, 1024 * 256));

		_id = glCreateShader(type);
		const GLchar* p = (const GLchar*)shader_string;
		glShaderSource(_id, 1, &p, NULL);
		glCompileShader(_id);

		// check for compile errors
		int params = -1;
		glGetShaderiv(_id, GL_COMPILE_STATUS, &params);
		if (GL_TRUE != params) {
			fprintf(stderr, "ERROR: GL shader index %i did not compile\n", _id);
			print_shader_info_log(_id);
			throw std::logic_error("GL shader index "+std::to_string(_id)+" did not compile");
		}

		if (debug) print_shader_info_log(_id);
	}


	Shader::~Shader()
	{
		glDeleteShader(_id);
	}
}
