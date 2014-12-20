#pragma once
#include <GL/glew.h>
#include <string>
#include <map>
#include <memory>

namespace graphics
{
	class Uniform
	{
	public:
		enum Type { matrix4fv, vec3fv };
		void glBind(GLuint program, GLint location);
		
		GLint getLocation(GLuint program_id);
		std::string getName() const { return name; };
		
		void setChanged() {
			changed = true;
		};
		bool hasChanged() {
			return changed;
		};

		static std::shared_ptr<Uniform> createUniformMatrix4fv(const std::string &name, const GLfloat * const value);
		static std::shared_ptr<Uniform> createUniformVec3fv(const std::string &name, const GLfloat * const value);

		~Uniform();
	private:
		Uniform(const Uniform& u);
		Uniform& operator=(const Uniform& u);
		Uniform(const std::string &name, const void * const value, const Type &type);

		const Type type;
		const std::string name;
		const void * const value;
		bool changed;
	};
}
