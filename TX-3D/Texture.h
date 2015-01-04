#pragma once
#include <string>
#include <memory>
#include <GL\glew.h>

namespace graphics
{
	class Uniform;

	class Texture
	{
	private:
		unsigned char* image_data;
		GLuint tex_id;
		std::shared_ptr<Uniform> sampler2D_uniform;
		int width, height;
		const int slot;
		const int nb_channels;
		void load_image(const std::string file_name);
		void reverse_image();
		void glBuild();
	public:
		GLuint id() { return tex_id; };
		void glBind();
		std::shared_ptr<Uniform> getSampler2D();
		Texture(const std::string file_name, const int nb_channels, const int tex_slot);
		~Texture();
	};
}