#include "shaders.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

namespace graphics {
	// Validation
	bool is_programme_valid(GLuint sp) {
		glValidateProgram(sp);
		GLint params = -1;
		glGetProgramiv(sp, GL_VALIDATE_STATUS, &params);
		if (GL_TRUE != params) {
			gl_log_err("program %i GL_VALIDATE_STATUS = GL_FALSE\n", sp);
			print_programme_info_log(sp);
			return false;
		}
		gl_log("program %i GL_VALIDATE_STATUS = GL_TRUE\n", sp);
		return true;
	}

	// Logging and debugging
	void print_shader_info_log(GLuint shader_index) {
		int max_length = 2048;
		int actual_length = 0;
		char log[2048];
		glGetShaderInfoLog(shader_index, max_length, &actual_length, log);
		printf("shader info log for GL index %i:\n%s\n", shader_index, log);
		gl_log("shader info log for GL index %i:\n%s\n", shader_index, log);
	}

	void print_programme_info_log(GLuint sp) {
		int max_length = 2048;
		int actual_length = 0;
		char log[2048];
		glGetProgramInfoLog(sp, max_length, &actual_length, log);
		printf("program info log for GL index %u:\n%s", sp, log);
		gl_log("program info log for GL index %u:\n%s", sp, log);
	}

	/* print absolutely everything about a shader - only useful if you get really
	stuck wondering why a shader isn't working properly */
	void print_all(GLuint sp) {
		int params = -1;
		int i;

		printf("--------------------\nshader programme %i info:\n", sp);
		glGetProgramiv(sp, GL_LINK_STATUS, &params);
		printf("GL_LINK_STATUS = %i\n", params);

		glGetProgramiv(sp, GL_ATTACHED_SHADERS, &params);
		printf("GL_ATTACHED_SHADERS = %i\n", params);

		glGetProgramiv(sp, GL_ACTIVE_ATTRIBUTES, &params);
		printf("GL_ACTIVE_ATTRIBUTES = %i\n", params);

		for (i = 0; i < params; i++) {
			char name[64];
			int max_length = 64;
			int actual_length = 0;
			int size = 0;
			GLenum type;
			glGetActiveAttrib(sp, i, max_length, &actual_length, &size, &type, name);
			if (size > 1) {
				int j;
				for (j = 0; j < size; j++) {
					char long_name[64];
					int location;

					sprintf(long_name, "%s[%i]", name, j);
					location = glGetAttribLocation(sp, long_name);
					printf("  %i) type:%s name:%s location:%i\n",
						i, GL_type_to_string(type), long_name, location);
				}
			}
			else {
				int location = glGetAttribLocation(sp, name);
				printf("  %i) type:%s name:%s location:%i\n",
					i, GL_type_to_string(type), name, location);
			}
		}

		glGetProgramiv(sp, GL_ACTIVE_UNIFORMS, &params);
		printf("GL_ACTIVE_UNIFORMS = %i\n", params);
		for (i = 0; i < params; i++) {
			char name[64];
			int max_length = 64;
			int actual_length = 0;
			int size = 0;
			GLenum type;
			glGetActiveUniform(sp, i, max_length, &actual_length, &size, &type, name);
			if (size > 1) {
				int j;
				for (j = 0; j < size; j++) {
					char long_name[64];
					int location;

					sprintf(long_name, "%s[%i]", name, j);
					location = glGetUniformLocation(sp, long_name);
					printf("  %i) type:%s name:%s location:%i\n",
						i, GL_type_to_string(type), long_name, location);
				}
			}
			else {
				int location = glGetUniformLocation(sp, name);
				printf("  %i) type:%s name:%s location:%i\n",
					i, GL_type_to_string(type), name, location);
			}
		}

		print_programme_info_log(sp);
	}


	// Conveniency functions
	const char* GL_type_to_string(GLenum type) {
		switch (type) {
		case GL_BOOL: return "bool";
		case GL_INT: return "int";
		case GL_FLOAT: return "float";
		case GL_FLOAT_VEC2: return "vec2";
		case GL_FLOAT_VEC3: return "vec3";
		case GL_FLOAT_VEC4: return "vec4";
		case GL_FLOAT_MAT2: return "mat2";
		case GL_FLOAT_MAT3: return "mat3";
		case GL_FLOAT_MAT4: return "mat4";
		case GL_SAMPLER_2D: return "sampler2D";
		case GL_SAMPLER_3D: return "sampler3D";
		case GL_SAMPLER_CUBE: return "samplerCube";
		case GL_SAMPLER_2D_SHADOW: return "sampler2DShadow";
		default: break;
		}
		return "other";
	}
}