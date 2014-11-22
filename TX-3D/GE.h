#pragma once

#include <GL/glew.h>
#include <list>
#include <map>
#include <memory>


class Camera;

namespace graphics
{
	class Program;
	class Uniform;

	class GE
	{
	public:
		GE();
		~GE();
		
		void setUniform(const std::shared_ptr<Uniform> &uniform);
		void verify() const;

		void update_fps_counter();
		void draw(GLuint vao);
		double elasped_time();
		std::shared_ptr<Camera> getCamera() {
			return camera;
		};
		void setPoint_count(int count) { point_count = count; };
	private:
		int point_count;
		std::shared_ptr<Camera> camera;
		// TODO transform in a program list
		std::unique_ptr<Program> program;
		double elapsed_seconds;
	};

	//TODO make vbo and vao first-class citizen
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