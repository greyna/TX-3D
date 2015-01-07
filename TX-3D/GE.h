#pragma once

#include <GL/glew.h>
#include <list>
#include <map>
#include <memory>


class Camera;
class Light;

namespace graphics
{
	class Program;
	class Uniform;
	class Mesh;

	class GE
	{
	public:
		GE();
		~GE();
		
		void setUniform(const std::shared_ptr<Uniform> &uniform);
		void verify() const;
		void logAll() const;

		void update_fps_counter();
		void draw();
		double elasped_time();
		std::shared_ptr<Camera> getCamera() {
			return camera;
		};
		void addMesh(const std::shared_ptr<Mesh> &mesh);

		// Oculus specific code
		void setOculusWindowResolutionPosition(int w, int h, int x, int y);
		GLuint setOculusRenderToTexture(int w, int h); 
		void drawOculusFromViewport(int w, int h, int x, int y);
		void clearOculus();
	private:
		bool oculus_mode;
		GLuint oculus_frameBuffer;
		GLuint oculus_texture;
		GLuint oculus_renderBuffer;

		std::shared_ptr<Camera> camera;
		std::shared_ptr<Light> light;
		// TODO transform in a program list
		std::unique_ptr<Program> program;
		std::list < std::shared_ptr<Mesh> > scene;
		double elapsed_seconds;
	};

}