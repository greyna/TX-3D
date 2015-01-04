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

	std::shared_ptr<Uniform> Uniform::createUniformVec3fv(const std::string &name, const GLfloat * const value) {
		return std::shared_ptr<Uniform>(new Uniform(name, value, Uniform::vec3fv));
	}

	std::shared_ptr<Uniform> Uniform::createUniformTex1i(const std::string &name, const GLint * const value) {
		return std::shared_ptr<Uniform>(new Uniform(name, value, Uniform::tex1i));
	}

	void Uniform::glBind(GLuint program, GLint location)
	{
		if (changed) {
			if (type == Type::matrix4fv)
			{
				glProgramUniformMatrix4fv(program, location, 1, GL_FALSE, static_cast<const GLfloat*>(value));
			}
			else if (type == Type::vec3fv)
			{
				glProgramUniform3fv(program, location, 1, static_cast<const GLfloat*>(value));
			}
			else if (type == Type::tex1i)
			{
				// bind uniform sampler2D to texture slot *value
				glProgramUniform1i(program, location, *static_cast<const GLint*>(value));
			}
			changed = false;
		}
	}
	GLint Uniform::getLocation(GLuint program_id)
	{
		return glGetUniformLocation(program_id, name.c_str());
	}
}