#include "CubeMap.h"
#include "stb_image.h"
#include <exception>

namespace graphics {
	CubeMap::CubeMap(const std::string front, const std::string back, const std::string top, const std::string bottom, const std::string left, const std::string right):
		vao(0), texture(0)
	{
		make_big_cube();

		// generate a cube-map texture to hold all the sides
		glActiveTexture(GL_TEXTURE0);
		glGenTextures(1, &texture);

		// load each image and copy into a side of the cube-map texture
		load_cube_map_side(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, front);
		load_cube_map_side(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, back);
		load_cube_map_side(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, top);
		load_cube_map_side(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, bottom);
		load_cube_map_side(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, left);
		load_cube_map_side(GL_TEXTURE_CUBE_MAP_POSITIVE_X, right);
		// format cube map texture
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	CubeMap::~CubeMap()
	{
	}

	void CubeMap::draw()
	{
		glDepthMask(GL_FALSE);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDepthMask(GL_TRUE);
	}

	void CubeMap::load_cube_map_side(GLenum side_target, const std::string file_name)
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

		int x, y, n;
		int force_channels = 4;
		unsigned char*  image_data = stbi_load(
			file_name.c_str(), &x, &y, &n, force_channels);
		if (!image_data) {
			fprintf(stderr, "ERROR: could not load %s\n", file_name.c_str());
			throw std::logic_error("ERROR: could not load image");
		}
		// non-power-of-2 dimensions check
		if ((x & (x - 1)) != 0 || (y & (y - 1)) != 0) {
			fprintf(
				stderr, "WARNING: image %s is not power-of-2 dimensions\n", file_name
				);
		}

		// copy image data into 'target' side of cube map
		glTexImage2D(
			side_target,
			0,
			GL_RGBA,
			x,
			y,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			image_data
			);
		free(image_data);
	}

	void CubeMap::make_big_cube()
	{
		float points[] = {
			-1.0f, 1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, 1.0f, -1.0f,
			-1.0f, 1.0f, -1.0f,

			-1.0f, -1.0f, 1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f, 1.0f, -1.0f,
			-1.0f, 1.0f, -1.0f,
			-1.0f, 1.0f, 1.0f,
			-1.0f, -1.0f, 1.0f,

			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 1.0f,
			-1.0f, -1.0f, 1.0f,

			-1.0f, 1.0f, -1.0f,
			1.0f, 1.0f, -1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f, 1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f, 1.0f,
			1.0f, -1.0f, 1.0f
		};
		GLuint vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(
			GL_ARRAY_BUFFER, 3 * 36 * sizeof(GLfloat), &points, GL_STATIC_DRAW);

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	}
}