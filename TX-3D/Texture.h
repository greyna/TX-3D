#pragma once
#include <string>
#include <GL\glew.h>

class Texture
{
private:
	unsigned char* image_data;
	GLuint tex_id;
	int width, height;
	int slot;
public:
	GLuint id() { return tex_id; };
	Texture(const std::string file_name, int nb_channels, int tex_slot);
	~Texture();
};

