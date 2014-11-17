#include "GE.h"
#include <assert.h>
#include <stdexcept>
#include <string>

namespace graphics {

	GE::GE() : uniforms(std::list<Uniform>()), camera()
	{
		assert(restart_gl_log());

		// all the start-up code for GLFW and GLEW is called here
		assert(start_gl());

		if (debug)
			log_gl_params();

		// GL State machine
		glEnable(GL_DEPTH_TEST); // enable depth-testing
		glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
		glEnable(GL_CULL_FACE); // cull face
		glCullFace(GL_BACK); // cull back face
		glFrontFace(GL_CW); // GL_CCW for counter clock-wise

		Shader vs(Shader::vertex, "transform.vert");
		Shader fs(Shader::fragment, "color.frag");
		loadProgram({ vs, fs });

		setUniformMatrix4fv("view", camera.getViewMatrix());
		setUniformMatrix4fv("proj", camera.getProjMatrix());
	}

	GE::~GE()
	{
		// close GL context and any other GLFW resources
		glfwTerminate();
	}

	void GE::loadProgram(std::list<Shader> shaders) {
		_prog_id = glCreateProgram();

		for(auto shader : shaders) {
			glAttachShader(_prog_id, shader.id());
		}
		glLinkProgram(_prog_id);

		int params = -1;
		glGetProgramiv(_prog_id, GL_LINK_STATUS, &params);
		if (GL_TRUE != params) {
			fprintf(
				stderr,
				"ERROR: could not link shader programme GL index %i\n",
				_prog_id
				);
			print_programme_info_log(_prog_id);
			throw std::logic_error("could not link shader programme GL index " + std::to_string(_prog_id));
		}

		if (debug) {
			is_programme_valid(_prog_id);
			print_programme_info_log(_prog_id);
			print_all(_prog_id);
		}
	}


	void GE::setUniformMatrix4fv(std::string name, const float *value){
		int location = glGetUniformLocation(_prog_id, name.c_str());
		glUseProgram(_prog_id);
		glUniformMatrix4fv(location, 1, GL_FALSE, value);

		uniforms.push_back(Uniform(name, value, location));
	}

	void GE::update_fps_counter() {
		static double previous_seconds = glfwGetTime();
		static int frame_count;
		double current_seconds = glfwGetTime();
		double elapsed_seconds = current_seconds - previous_seconds;
		if (elapsed_seconds > 0.25) {
			previous_seconds = current_seconds;
			double fps = (double)frame_count / elapsed_seconds;
			char tmp[128];
			sprintf(tmp, "opengl @ fps: %.2f", fps);
			glfwSetWindowTitle(g_window, tmp);
			frame_count = 0;
		}
		frame_count++;
	}

	double GE::elasped_time() {
		static double previous_seconds = glfwGetTime();
		double current_seconds = glfwGetTime();
		elapsed_seconds = current_seconds - previous_seconds;
		previous_seconds = current_seconds;
		return elapsed_seconds;
	}

	void GE::draw(GLuint vao) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.2, 0.2, 0.2, 1.0); // grey background to help spot mistakes
		glViewport(0, 0, g_gl_width, g_gl_height);

		camera.update();

		for (auto uniform : uniforms) {
			glUseProgram(_prog_id);
			glUniformMatrix4fv(uniform.location, 1, GL_FALSE, uniform.value);
		}
		

		glUseProgram(_prog_id);
		glBindVertexArray(vao);
		// draw points 0-3 from the currently bound VAO with current in-use shader
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	GLuint create_vao(std::list<GLuint> vbo_ids) {
		GLuint vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		int i = 0;
		for (auto vbo_id : vbo_ids) {
			glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
			glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, 0, NULL);
			i++;
		}

		for (int j = 0; j < i; ++j) {
			glEnableVertexAttribArray(j);
		}

		return vao;
	}
}