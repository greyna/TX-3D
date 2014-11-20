#include "Shader.h"
#include <stdio.h>
#include <assert.h>
#include <stdexcept>
#include <string>
#include <fstream>


namespace graphics {
	std::map<std::string, std::shared_ptr<Shader>> Shader::shaders = std::map<std::string, std::shared_ptr<Shader>>();

	Shader::Shader(Type type, std::string file_name, std::list<std::string> uniformNames):
		type(type), file_name(file_name), compiled(false), uniformNames(uniformNames)
	{
		compile();
	}
	std::shared_ptr<Shader> Shader::createShader(
		Type type, std::string file_name, std::list<std::string> uniformNames)
	{
		auto shader_it = shaders.find(file_name);
		if (shader_it == shaders.end())
		{
			auto shader_ptr = std::shared_ptr<Shader>(new Shader(type, file_name, uniformNames));
			shaders.insert(
				std::make_pair (file_name, shader_ptr));
			
			//the return by copy creates a new shader_ptr for the caller
			return shader_ptr;
		}
		else
		{
			// if shader already exist, recycle it
			shader_it->second->uniformNames = uniformNames;
			return shader_it->second;
		}
	}
	void Shader::releaseAll() {
		for (auto shader_pair : shaders) {
			shader_pair.second->release();
		}
	}


	Shader::~Shader()
	{
		release();
	}

	GLuint Shader::id() {
		compile();
		return _id;
	}

	void Shader::release() {
		if (compiled) {
			glDeleteShader(_id);
			compiled = false;
		}
	}

	void Shader::compile() {
		if (!compiled) {
			std::string shader_string(getFileContent(file_name));

			_id = glCreateShader(type);
			const GLchar* p = (const GLchar*) shader_string.c_str();
			glShaderSource(_id, 1, &p, NULL);
			glCompileShader(_id);

			// check for compile errors
			int params = -1;
			glGetShaderiv(_id, GL_COMPILE_STATUS, &params);
			if (GL_TRUE != params) {
				fprintf(stderr, "ERROR: GL shader index %i did not compile\n", _id);
				print_shader_info_log(_id);
				throw std::logic_error("GL shader index " + std::to_string(_id) + " did not compile");
			}

			compiled = true;
		}
	}
	std::string getFileContent(const std::string& path)
	{
		std::ifstream file(path);
		std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		return content;
	}
}
