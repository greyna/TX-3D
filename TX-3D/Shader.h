#pragma once
#include "shaders.h"
#include <map>
#include <memory>
#include <list>

namespace graphics {
	class Shader
	{
	public:
		enum Type { vertex = GL_VERTEX_SHADER, fragment = GL_FRAGMENT_SHADER };

		GLuint id();
		const std::list<std::string> getUniformNames() {
			return uniformNames;
		};

		static std::shared_ptr<Shader> createShader(
			Type type, std::string file_name, std::list<std::string> uniformNames);
		static void releaseAll();

		~Shader();
	private:
		// map<file_name, shader>
		static std::map<std::string, std::shared_ptr<Shader>> shaders;

		void compile();
		void release();

		bool compiled;
		const Type type;
		GLuint _id;
		std::list<std::string> uniformNames;
		std::string file_name;

		Shader(Type type, std::string file_name, std::list<std::string> uniformNames);
	};

	std::string getFileContent(const std::string& path);
}