#pragma once

#include "GL\glew.h"
#include <string>

namespace graphics {
	class CubeMap
	{
	private:
		GLuint vao;
		GLuint texture;

	public:
		CubeMap(const std::string front, const std::string back, const std::string top, const std::string bottom, const std::string left, const std::string right);
		~CubeMap();
		void draw();
		void load_cube_map_side(GLenum side_target, const std::string file_name);
		void make_big_cube();
	};
}

