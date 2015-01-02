#include "Texture.h"
#include "stb_image.h"
#include <stdexcept>


Texture::Texture(const std::string file_name, int nb_channels, int tex_slot):
tex_id(0), image_data(NULL), width(0), height(0), slot(tex_slot)
{
	int x, y, n;
	image_data = stbi_load(file_name.c_str(), &x, &y, &n, nb_channels);
	if (!image_data) {
		fprintf(stderr, "ERROR: could not load %s\n", file_name.c_str());
		throw std::logic_error("ERROR: could not load" + file_name);
	}

	width = x;
	height = y;

	// NPOT check
	if ((x & (x - 1)) != 0 || (y & (y - 1)) != 0) {
		fprintf(
			stderr, "WARNING: texture %s is not power-of-2 dimensions\n", file_name
			);
	}

	// reverse for OpenGL need
	int width_in_bytes = x * 4;
	unsigned char *top = NULL;
	unsigned char *bottom = NULL;
	unsigned char temp = 0;
	int half_height = y / 2;

	for (int row = 0; row < half_height; row++) {
		top = image_data + row * width_in_bytes;
		bottom = image_data + (y - row - 1) * width_in_bytes;
		for (int col = 0; col < width_in_bytes; col++) {
			temp = *top;
			*top = *bottom;
			*bottom = temp;
			top++;
			bottom++;
		}
	}

	// OpenGL calls
	glGenTextures(1, &tex_id);
	// active texture slot 0
	if (slot==0) glActiveTexture(GL_TEXTURE0);
	else throw std::logic_error("texture slot not available in code");
	// to be bound with the texture
	glBindTexture(GL_TEXTURE_2D, tex_id);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		x,
		y,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		image_data
		);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	GLfloat max_aniso = 0.0f;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_aniso);
	// set the maximum!
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, max_aniso);
}


Texture::~Texture()
{
}
