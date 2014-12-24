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
	private:
		std::shared_ptr<Camera> camera;
		std::shared_ptr<Light> light;
		// TODO transform in a program list
		std::unique_ptr<Program> program;
		std::list < std::shared_ptr<Mesh> > scene;
		double elapsed_seconds;
	};

}