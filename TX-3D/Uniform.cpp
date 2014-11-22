#include "Uniform.h"

namespace graphics
{
	Uniform::Uniform(const std::string &name, const void * const value, const Type &type) :
		name(name), value(value), type(type), changed(true)
	{
	}

	Uniform::~Uniform()
	{
	}

	std::shared_ptr<Uniform> Uniform::createUniformMatrix4fv(const std::string &name, const GLfloat * const value) {
		return std::shared_ptr<Uniform>(new Uniform(name, value, Uniform::matrix4fv));
	}

	void Uniform::glBind(GLuint program, GLint location)
	{
		if (changed) {
			if (type == Type::matrix4fv)
			{
				glProgramUniformMatrix4fv(program, location, 1, GL_FALSE, static_cast<const GLfloat*>(value));
			}
			/*else if()  => other OpenGL uniform types
			{

			}*/
			changed = false;
		}
	}
	GLint Uniform::getLocation(GLuint program_id)
	{
		return glGetUniformLocation(program_id, name.c_str());
	}
}