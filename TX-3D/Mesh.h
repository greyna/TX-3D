#pragma once

#include "GL\glew.h"
#include "maths_funcs.h";
#include <string>
#include <memory>
#include <iostream>
#include <list>

namespace graphics
{
	class Uniform;

	class Mesh
	{
	private:
		GLfloat* vp; // array of vertex points
		GLfloat* vn; // array of vertex normals
		GLfloat* vt; // array of texture coordinates
		int point_count;
		mat4 model;
		std::shared_ptr<Uniform> model_uniform;
		GLuint vao;
	public:
		Mesh(const std::string filename);
		void draw();
		std::shared_ptr<Uniform> getModel() { return model_uniform; };
		void setModel(mat4 model);
		~Mesh();
	};

	template <typename T> GLuint create_vbo(T* data, size_t size)
	{
		GLuint vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, size * sizeof(T), data, GL_STATIC_DRAW);
		return vbo;
	};
	GLuint create_vao(std::list<GLuint> vbo_ids);
}
