#pragma once
#include "Shader.h"
#include "Camera.h"
#include <list>

namespace graphics {



	class GE
	{
	public:
		GE();
		~GE();
		//TODO make program a first-class citizen
		void loadProgram(std::list<Shader> shaders);
		void setUniformMatrix4fv(std::string name, const float *value);
		GLuint prog_id() { return _prog_id; };
		void update_fps_counter();
		void draw(GLuint vao);
		double elasped_time();
		Camera& getCamera() {
			return camera;
		};
	private:
		Camera camera;
		std::list<Uniform> uniforms;
		GLuint _prog_id;
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