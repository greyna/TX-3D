#pragma once
#include <GL/glew.h>
#include <map>
#include <memory>
#include <list>

namespace graphics {
	class Shader
	{
	public:
		enum Type { vertex = GL_VERTEX_SHADER, fragment = GL_FRAGMENT_SHADER };

		GLuint id();
		const std::list<std::string>& getUniformNames() const {
			return uniformNames;
		};

		static std::shared_ptr<Shader> createShader(
			const Type &type, const std::string &file_name, const std::list<std::string> &uniformNames);
		static void releaseAll();
		void Shader::log();

		~Shader();
	private:
		// map<file_name, shader>
		static std::map<std::string, std::shared_ptr<Shader>> shaders;

		void compile();
		void release();

		const Type type;
		const std::string file_name;
		std::list<std::string> uniformNames;
		GLuint _id;
		bool compiled;

		Shader(const Shader& s);
		Shader& operator=(const Shader& s);
		Shader(const Type &type, const std::string &file_name, const std::list<std::string> &uniformNames);
	};

	std::string getFileContent(const std::string& path);
}