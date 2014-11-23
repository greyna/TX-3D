#include "Program.h"

#include "graphics.h"
#include "Shader.h"
#include "Uniform.h"

#include <vector>
#include <iostream>
#include <sstream>


namespace graphics
{
	Program::Program() :
		_id(glCreateProgram()), uniforms(), shaders(), loaded(false)
	{
	}

	Program::~Program()
	{
		glDeleteProgram(_id);
	}

	// Must be called before load and use
	void Program::addNextShader(const std::shared_ptr < Shader > &shader)
	{
		shaders.push_back(shader);
	}

	// Must be called after load and before use
	void Program::setUniform(const std::shared_ptr < Uniform > &uniform)
	{
		if (!loaded) throw std::logic_error("You should load the program before setting uniforms.");
		
		// Stop if uniform already added
		for (auto uniform_pair : uniforms)
		{
			// Comparison by pointer address
			if (uniform_pair.second == uniform)
			{
				return;
			}
		}

		GLint location = uniform->getLocation(_id);
		// if existing uniform (same name or same location), write over it
		uniforms[location] = uniform;

		uniform->glBind(_id, location);
	}

	void Program::use()
	{
		if (!loaded) throw std::logic_error("You should load the program, then set uniforms, before using it.");

		for (auto uniform_pair : uniforms)
		{
			uniform_pair.second->glBind(_id, uniform_pair.first);
		}

		glUseProgram(_id);
	}

	void Program::load()
	{
		if (loaded) return;

		for (auto shader : shaders) {
			glAttachShader(_id, shader->id());
		}
		glLinkProgram(_id);

		int params = -1;
		glGetProgramiv(_id, GL_LINK_STATUS, &params);
		if (GL_TRUE != params) {
			log();
			throw std::logic_error("GL program #" + std::to_string(_id) + "failed at linking");
		}
		
		loaded = true;
	}

	bool Program::verifyUniforms() const
	{
		for (auto shader_ptr : shaders)
		{
			for (std::string needed_uniform_name : shader_ptr->getUniformNames())
			{
				bool found = false;
				for (auto uniform : uniforms)
				{
					if (needed_uniform_name == uniform.second->getName())
					{
						found = true;
						break;
					}
				}
				if (!found) return false;
			}
		}

		return true;
	}

	bool Program::validate() const
	{
		glValidateProgram(_id);
		GLint params = -1;
		glGetProgramiv(_id, GL_VALIDATE_STATUS, &params);
		if (GL_TRUE != params) {
			std::cerr << "program #" << _id << " GL_VALIDATE_STATUS = GL_FALSE" << std::endl;
			log();
			return false;
		}
		return true;
	}

	void Program::log() const
	{
		GLint maxLength = 0;
		glGetProgramiv(_id, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		glGetProgramInfoLog(_id, maxLength, &maxLength, &errorLog[0]);
		std::string log(errorLog.begin(), errorLog.end());

		std::cerr << "Program info log for GL index " << _id << ":" << std::endl << log << std::endl;
		gl_log("Program info log for GL index %i:\n%s\n", _id, log.c_str());
	}

	void Program::logAll() const
	{
		int params = -1;
		int i;

		std::cerr << "-------------------- Programme #" << _id << " logAll BEGIN:" << std::endl;
		glGetProgramiv(_id, GL_LINK_STATUS, &params);
		std::cerr << "\tGL_LINK_STATUS = " << params << std::endl;

		glGetProgramiv(_id, GL_ATTACHED_SHADERS, &params);
		std::cerr << "\tGL_ATTACHED_SHADERS = " << params << std::endl;

		glGetProgramiv(_id, GL_ACTIVE_ATTRIBUTES, &params);
		std::cerr << "\tGL_ACTIVE_ATTRIBUTES = " << params << std::endl;
		for (i = 0; i < params; i++) {
			char name[64];
			int max_length = 64;
			int actual_length = 0;
			int size = 0;
			GLenum type;
			glGetActiveAttrib(_id, i, max_length, &actual_length, &size, &type, name);
			if (size > 1) {
				int j;
				for (j = 0; j < size; j++) {
					std::stringstream ss;
					int location;

					ss << name << '[' << j << ']';
					std::string long_name(ss.str());
					location = glGetAttribLocation(_id, long_name.c_str());
					std::cerr << "  " << i << ") type:" << GL_type_to_string(type) << " name : " << long_name
						<< " location : " << location << std::endl;
				}
			}
			else {
				int location = glGetAttribLocation(_id, name);
				std::cerr << "  " << i << ") type:" << GL_type_to_string(type) << " name : " << name
					<< " location : " << location << std::endl;
			}
		}

		glGetProgramiv(_id, GL_ACTIVE_UNIFORMS, &params);
		std::cerr << "\tGL_ACTIVE_UNIFORMS = " << params << std::endl;
		for (i = 0; i < params; i++) {
			char name[64];
			int max_length = 64;
			int actual_length = 0;
			int size = 0;
			GLenum type;
			glGetActiveUniform(_id, i, max_length, &actual_length, &size, &type, name);
			if (size > 1) {
				int j;
				for (j = 0; j < size; j++) {
					std::stringstream ss;
					int location;

					ss << name << '[' << j << ']';
					std::string long_name(ss.str());
					location = glGetUniformLocation(_id, long_name.c_str());
					std::cerr << "  " << i << ") type:" << GL_type_to_string(type) << " name : " << long_name
						<< " location : " << location << std::endl;
				}
			}
			else {
				int location = glGetUniformLocation(_id, name);
				std::cerr << "  " << i << ") type:" << GL_type_to_string(type) << " name : " << name
					<< " location : " << location << std::endl;
			}
		}

		log();
		std::cerr << "-------------------- Programme #" << _id << " logAll FINISH.\n" << std::endl;
	}

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