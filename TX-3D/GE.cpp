#include "GE.h"

#include "graphics.h"
#include "Shader.h"
#include "Camera.h"
#include "Light.h"
#include "Program.h"
#include "Mesh.h"
#include "Texture.h"

#include <assert.h>
#include <stdexcept>
#include <string>

namespace graphics {

	GE::GE():
		camera(), program(), oculus_mode(false), oculus_frameBuffer(0)
	{
		assert(restart_gl_log());

		// all the start-up code for GLFW and GLEW is called here
		assert(start_gl());

		log_gl_params();

		// GL State machine
		glEnable(GL_DEPTH_TEST); // enable depth-testing
		glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
		glEnable(GL_CULL_FACE); // cull face
		glCullFace(GL_BACK); // cull back face
		glFrontFace(GL_CCW); // GL_CCW for counter clock-wise
		glClearColor(0.2, 0.2, 0.35, 1.0); // grey background to help spot mistakes

		camera = std::shared_ptr<Camera>(new Camera);
		light = std::shared_ptr<Light>(new Light);
		program = std::unique_ptr<Program>(new Program);

		std::shared_ptr<Shader> vs(Shader::createShader(Shader::vertex, "transform.vert", { "model", "view", "proj" }));
		std::shared_ptr<Shader> fs(Shader::createShader(Shader::fragment, "color.frag", { }));

		program->addNextShader(vs);
		program->addNextShader(fs);

		program->load();

		setUniform(camera->getViewUniform());
		setUniform(camera->getProjUniform());
		setUniform(light->getPositionUniform());
		setUniform(light->getSpecularUniform());
		setUniform(light->getDiffuseUniform());
		setUniform(light->getAmbientUniform());

		Shader::releaseAll();
	}

	GE::~GE()
	{
		if (oculus_mode) {
			glDeleteFramebuffers(1, &oculus_frameBuffer);
			glDeleteTextures(1, &oculus_texture);
			glDeleteRenderbuffers(1, &oculus_renderBuffer);
		}
		// close GL context and any other GLFW resources
		glfwTerminate();
	}

	void GE::verify() const
	{
		if (!program->verifyUniforms()) throw std::logic_error("Uniforms not correctly set in program used in GE::GE()");
		if (!program->validate()) throw std::logic_error("Program not validated!");
	}
	void GE::logAll() const
	{
		program->logAll();
	}

	void GE::draw() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, g_gl_width, g_gl_height);

		program->use();
		
		for (auto mesh : scene) {
			program->setUniform(mesh->getModel());
			program->setUniform(mesh->getTexture()->getSampler2D());
			mesh->draw();
		}
	}

	void GE::setUniform(const std::shared_ptr<Uniform> &uniform)
	{
		//TODO every program in GE
		program->setUniform(uniform);
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
			sprintf(tmp, "TX-3D               @ fps: %.2f", fps);
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
	
	void GE::addMesh(const std::shared_ptr<Mesh> &mesh)
	{
		scene.push_back(mesh);
		program->setUniform(mesh->getModel());
	}


	// Oculus specific code
	void GE::setOculusWindowResolutionPosition(int w, int h, int x, int y)
	{
		glfwSetWindowSize(g_window, w, h);
		glfwSetWindowPos(g_window, x, y);
	}
	GLuint GE::setOculusRenderToTexture(int w, int h)
	{
		oculus_mode = true;
		glGenFramebuffers(1, &oculus_frameBuffer);

		glGenTextures(1, &oculus_texture);

		glBindFramebuffer(GL_FRAMEBUFFER, oculus_frameBuffer);
		glBindTexture(GL_TEXTURE_2D, oculus_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, oculus_texture, 0);

		glGenRenderbuffers(1, &oculus_renderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, oculus_renderBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, w, h);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, oculus_renderBuffer);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			throw std::exception("FRAMEBUFFER ERROR");
		}

		return oculus_texture;
	}
	void GE::drawOculusFromViewport(int w, int h, int x, int y)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, oculus_frameBuffer);
		glViewport(x, y, w, h);

		program->use();

		for (auto mesh : scene) {
			program->setUniform(mesh->getModel());
			program->setUniform(mesh->getTexture()->getSampler2D());
			mesh->draw();
		}
	}
	void GE::clearOculus()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, oculus_frameBuffer);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}