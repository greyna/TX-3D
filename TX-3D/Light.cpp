#include "Light.h"
#include "Uniform.h"


Light::Light():
	pos(0.0, 3.0, 0.0), spec(1.0, 1.0, 1.0), dif(0.7, 0.7, 0.7), amb(0.10, 0.10, 0.10)
{
	this->position = Uniform::createUniformVec3fv("light_position", pos.v);
	this->specular = Uniform::createUniformVec3fv("Ls", spec.v);
	this->diffuse = Uniform::createUniformVec3fv("Ld", dif.v);
	this->ambient = Uniform::createUniformVec3fv("La", amb.v);
}


Light::~Light()
{
}

void Light::setDiffuseLight(vec3 dif_color)
{
	dif = dif_color;
	diffuse->setChanged();
}
