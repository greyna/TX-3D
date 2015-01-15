#pragma once

#include "maths_funcs.h";

#include <memory>

namespace graphics {
	class Uniform;
}

using namespace graphics;

class Light
{
private:
	std::shared_ptr<Uniform> position, specular, diffuse, ambient;
	vec3 pos, spec, dif, amb;
public:
	std::shared_ptr<Uniform> getPositionUniform() { return position; };
	std::shared_ptr<Uniform> getSpecularUniform() { return specular; };
	std::shared_ptr<Uniform> getDiffuseUniform() { return diffuse; };
	std::shared_ptr<Uniform> getAmbientUniform() { return ambient; };
	Light();
	~Light();
	void setDiffuseLight(vec3 dif_color);
	vec3 getDiffuseLight() { return dif; };
};

