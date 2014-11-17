#pragma once
#include "shaders.h"

namespace graphics {
	class Shader
	{
		//TODO turn in factory with a static map<filename, shader> to reuse shaders
	public:
		enum Type { vertex = GL_VERTEX_SHADER, fragment = GL_FRAGMENT_SHADER };
		Shader(Type type, const char* file_name);
		GLuint id() { return _id; };
		~Shader();
	private:
		const Type type;
		GLuint _id;
	};
}