#pragma once

#include <GL/glew.h>
#include <memory>
#include <map>
#include <list>

namespace graphics
{
	class Shader;
	class Uniform;

	class Program
	{
	public:
		Program();
		~Program();
		void addNextShader(const std::shared_ptr < Shader > &shader);
		void setUniform(const std::shared_ptr < Uniform > &uniform);
		void load();
		void use();
		void log() const;
		void logAll() const;
		bool verifyUniforms() const;
		bool validate() const;
	private:
		GLuint _id;
		bool loaded;
		std::map< GLint, std::shared_ptr < Uniform > > uniforms;
		std::list < std::shared_ptr < Shader > > shaders;
	};

	const char* GL_type_to_string(GLenum type);
}