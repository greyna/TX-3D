#pragma once

#include <GL/glew.h>
#include "maths_funcs.h"

#include <memory>

namespace graphics {
	class Uniform;
}

using namespace graphics;

class Camera
{
public:
	Camera();
	~Camera();
	std::shared_ptr<Uniform> getViewUniform(){ return proj; };
	std::shared_ptr<Uniform> getProjUniform(){ return view; };
	void update();

	void moveLeft(double elapsed_seconds);
	void moveRight(double elapsed_seconds);
	void moveTop(double elapsed_seconds);
	void moveBottom(double elapsed_seconds);
	void moveForward(double elapsed_seconds);
	void moveBackward(double elapsed_seconds);
	void yawLeft(double elapsed_seconds);
	void yawRight(double elapsed_seconds);
	void pitchTop(double elapsed_seconds);
	void pitchBottom(double elapsed_seconds);
	void rollLeft(double elapsed_seconds);
	void rollRight(double elapsed_seconds);
private:
	bool hasMoved;
	vec3 move;
	float yaw; // y-rotation motion in degrees
	float pitch; // x
	float roll; // z

	float speed; // 1 unit per second
	float heading_speed; // 10 degrees per second

	GLfloat proj_mat[16];
	mat4 view_mat;
	std::shared_ptr<Uniform> proj, view;

	vec3 pos; // position, don't start at zero, or we will be too close
	versor ori; // orientation

	// yaw, pitch and roll are calculated against those vectors, that we then update
	vec4 fwd;
	vec4 rgt;
	vec4 up;

	void calcProj();
	float near; // clipping plane
	float far; // clipping plane
	float fov; // field of view in radians
	float aspect; // aspect ratio
};
