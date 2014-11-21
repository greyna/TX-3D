#include "Shader.h"
#include <stdio.h>
#include <assert.h>
#include <stdexcept>
#include <string>
#include <fstream>
#include <vector>
#include <iostream>


namespace graphics {
	std::map<std::string, std::shared_ptr<Shader>> Shader::shaders = std::map<std::string, std::shared_ptr<Shader>>();

	Shader::Shader(const Type &type, const std::string &file_name, const std::list<std::string> &uniformNames) :
		type(type), file_name(file_name), compiled(false), uniformNames(uniformNames)
	{
		compile();
	}
	std::shared_ptr<Shader> Shader::createShader(
		const Type &type, const std::string &file_name, const std::list<std::string> &uniformNames)
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

			compiled = true; // release will set it to false if an error occur

			const GLchar* p = static_cast<const GLchar*> (shader_string.c_str());
			glShaderSource(_id, 1, &p, NULL);
			glCompileShader(_id);

			// check for compile errors
			GLint isCompiled = 0;
			glGetShaderiv(_id, GL_COMPILE_STATUS, &isCompiled);
			if (GL_TRUE != isCompiled) {
				log();
				release();
				throw std::logic_error("GL shader index " + std::to_string(_id) + " did not compile");
			}
		}
	}
	std::string getFileContent(const std::string& path)
	{
		std::ifstream file(path);
		std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		return content;
	}

	void Shader::log() {
		GLint maxLength = 0;
		glGetShaderiv(id(), GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(id(), maxLength, &maxLength, &errorLog[0]);

		std::string log(errorLog.begin(), errorLog.end());
		std::cerr << "Shader info log for GL index " << _id << ":" << std::endl << log << std::endl;
		gl_log("Shader info log for GL index %i:\n%s\n", _id, log.c_str());
	}
}
