#include "Texture.h"
#include "Uniform.h"
#include "stb_image.h"
#include <iostream>
#include <stdexcept>

namespace graphics
{
	Texture::Texture(const std::string file_name, const int nb_channels, const int tex_slot) :
		tex_id(0), image_data(NULL), width(0), height(0), slot(tex_slot), sampler2D_uniform(), nb_channels(nb_channels)
	{
		load_image(file_name);
		reverse_image(); // reverse for OpenGL need
		glBuild();

		sampler2D_uniform = Uniform::createUniformTex1i("basic_texture", &slot);
	}

	Texture::~Texture()
	{
		free(image_data);
	}


	void Texture::load_image(const std::string file_name)
	{
		int n;
		image_data = stbi_load(file_name.c_str(), &width, &height, &n, nb_channels);
		if (!image_data) {
			fprintf(stderr, "ERROR: could not load %s\n", file_name.c_str());
			throw std::logic_error("ERROR: could not load" + file_name);
		}

		// NPOT check
		if ((width & (width - 1)) != 0 || (height & (height - 1)) != 0) {
			fprintf(
				stderr, "WARNING: texture %s is not power-of-2 dimensions\n", file_name
				);
		}
	}

	void Texture::reverse_image()
	{
		int width_in_bytes = width * 4;
		unsigned char *top = NULL;
		unsigned char *bottom = NULL;
		unsigned char temp = 0;
		int half_height = height / 2;

		for (int row = 0; row < half_height; row++) {
			top = image_data + row * width_in_bytes;
			bottom = image_data + (height - row - 1) * width_in_bytes;
			for (int col = 0; col < width_in_bytes; col++) {
				temp = *top;
				*top = *bottom;
				*bottom = temp;
				top++;
				bottom++;
			}
		}
	}

	void Texture::glBuild()
	{
		glGenTextures(1, &tex_id);
		// active texture slot
		if (slot == 0) glActiveTexture(GL_TEXTURE0);
		else throw std::logic_error("texture slot not available in code");

		// to be bound with the texture
		glBindTexture(GL_TEXTURE_2D, tex_id);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			width,
			height,
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

	void Texture::glBind()
	{
		// active texture slot
		if (slot == 0) glActiveTexture(GL_TEXTURE0);
		else throw std::logic_error("texture slot not available in code");

		// to be bound with the texture
		glBindTexture(GL_TEXTURE_2D, tex_id);
	}

	std::shared_ptr<Uniform> Texture::getSampler2D()
	{
		glBind(); // bind the texture to its slot if the uniform is needed

		return sampler2D_uniform;
	}

}