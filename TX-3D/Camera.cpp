#include "Camera.h"

#include "graphics.h"
#include "Uniform.h"
#include "Oculus.h"

#include <iostream>

using namespace graphics;

Camera::Camera() :
hasMoved(true), move(0.0f, 0.0f, 0.0f), yaw(0.0f), pitch(0.0f), roll(0.0f),
speed(3.0f), heading_speed(30.0f),
pos(0.0f, 2.0f, 5.0f), ori(quat_from_axis_deg(0.0f, 0.0f, 1.0f, 0.0f)),
fwd(0.0f, 0.0f, -1.0f, 0.0f), rgt(1.0f, 0.0f, 0.0f, 0.0f), up(0.0f, 1.0f, 0.0f, 0.0f),
nearP(0.1f), farP(100.0f), fov(67.0f * ONE_DEG_IN_RAD), aspect((float)g_gl_width / (float)g_gl_height)
{
	proj = Uniform::createUniformMatrix4fv("proj", proj_mat);
	view = Uniform::createUniformMatrix4fv("view", view_mat.m);
	rotview = Uniform::createUniformMatrix4fv("rotview", rotview_mat.m);

	calcProj();

	mat4 R = quat_to_mat4(ori);
	mat4 T = translate(identity_mat4(), vec3(-pos.v[0], -pos.v[1], -pos.v[2]));
	view_mat = R*T;
	view->setChanged();
	rotview_mat = R;
	rotview->setChanged();
}

Camera::~Camera()
{
}

void Camera::calcProj() {
	float range = tan(fov * 0.5f) * nearP;
	float Sx = (2.0f * nearP) / (range * aspect + range * aspect);
	float Sy = nearP / range;
	float Sz = -(farP + nearP) / (farP - nearP);
	float Pz = -(2.0f * farP * nearP) / (farP - nearP);

	for (int i = 0; i < 16; ++i) {
		proj_mat[i] = 0.0f;
	}
	proj_mat[0] = Sx;
	proj_mat[5] = Sy;
	proj_mat[10] = Sz;
	proj_mat[11] = -1.0f;
	proj_mat[14] = Pz;

	proj->setChanged();
}

void Camera::update() {
	if (hasMoved) {
		mat4 R = quat_to_mat4(ori);

		fwd = R * vec4(0.0, 0.0, -1.0, 0.0);
		rgt = R * vec4(1.0, 0.0, 0.0, 0.0);
		//up = R * vec4(0.0, 1.0, 0.0, 0.0); // useless when moving along the (x,z) plan

		// moving along the (x,z) plan
		vec3 front = vec3(fwd);
		vec3 side = vec3(rgt);
		front.v[1] = 0;
		side.v[1] = 0;

		pos = pos + front * -move.v[2];
		pos = pos + side * move.v[0];
		pos = pos + vec3(0, 1, 0) * move.v[1];

		mat4 T = translate(identity_mat4(), pos);

		view_mat = inverse(R) * inverse(T);
		view->setChanged();
		rotview_mat = inverse(R);
		rotview->setChanged();

		move = vec3(0.0f, 0.0f, 0.0f);
		yaw = 0.0f;
		pitch = 0.0f;
		roll = 0.0f;
		hasMoved = false;
	}

	float new_aspect = (float)g_gl_width / (float)g_gl_height;
	if (aspect != new_aspect) {
		aspect = new_aspect;
		calcProj();
	}
}

// to be called one time for each eye
void Camera::updateOculus(mat4 o, vec3 p, vec3 viewOffSet, mat4 pr) {
	// in case of the eyes have different proj matrix
	for (int i = 0; i < 16; ++i) {
		proj_mat[i] = pr.m[i];
	}
	proj->setChanged();

	//ori = o;
	//mat4 R = quat_to_mat4(ori);
	//mat4 R = rotate_z_deg(rotate_y_deg(rotate_x_deg(identity_mat4(), o.v[0]), o.v[1]), o.v[2]);
	mat4 R = o;

	fwd = R * vec4(0.0, 0.0, -1.0, 0.0);
	rgt = R * vec4(1.0, 0.0, 0.0, 0.0);
	//up = R * vec4(0.0, 1.0, 0.0, 0.0); // useless when moving along the (x,z) plan

	// moving along the (x,z) plan
	vec3 front = vec3(fwd);
	vec3 side = vec3(rgt);
	front.v[1] = 0;
	side.v[1] = 0;

	pos = pos + front * -move.v[2];
	pos = pos + side * move.v[0];
	pos = pos + vec3(0,1,0) * move.v[1];

	mat4 T = translate(identity_mat4(), pos + p);

	view_mat = inverse(R) * inverse(T);
	view_mat = translate(view_mat, viewOffSet);
	view->setChanged();
	rotview_mat = inverse(R);
	rotview->setChanged();

	move = vec3(0.0f, 0.0f, 0.0f);
	yaw = 0.0f;
	pitch = 0.0f;
	roll = 0.0f;
	hasMoved = false;
}



void Camera::moveLeft(double elapsed_seconds) {
	move.v[0] -= speed * elapsed_seconds;
	hasMoved = true;
}
void Camera::moveRight(double elapsed_seconds) {
	move.v[0] += speed * elapsed_seconds;
	hasMoved = true;
}
void Camera::moveTop(double elapsed_seconds) {
	move.v[1] += speed * elapsed_seconds;
	hasMoved = true;
}
void Camera::moveBottom(double elapsed_seconds) {
	move.v[1] -= speed * elapsed_seconds;
	hasMoved = true;
}
void Camera::moveForward(double elapsed_seconds) {
	move.v[2] -= speed * elapsed_seconds;
	hasMoved = true;
}
void Camera::moveBackward(double elapsed_seconds) {
	move.v[2] += speed * elapsed_seconds;
	hasMoved = true;
}
void Camera::yawLeft(double elapsed_seconds) {
	yaw += heading_speed * elapsed_seconds;
	hasMoved = true;

	// create a quaternion representing change in heading (the yaw)
	versor q_yaw = quat_from_axis_deg(yaw, up.v[0], up.v[1], up.v[2]);
	// add yaw rotation to the camera's current orientation
	ori = q_yaw * ori;
}
void Camera::yawRight(double elapsed_seconds) {
	yaw -= heading_speed * elapsed_seconds;
	hasMoved = true;

	versor q_yaw = quat_from_axis_deg(yaw, up.v[0], up.v[1], up.v[2]);
	ori = q_yaw * ori;
}
void Camera::pitchTop(double elapsed_seconds) {
	pitch += heading_speed * elapsed_seconds;
	hasMoved = true;

	versor q_pitch = quat_from_axis_deg(pitch, rgt.v[0], rgt.v[1], rgt.v[2]);
	ori = q_pitch * ori;
}
void Camera::pitchBottom(double elapsed_seconds) {
	pitch -= heading_speed * elapsed_seconds;
	hasMoved = true;

	versor q_pitch = quat_from_axis_deg(pitch, rgt.v[0], rgt.v[1], rgt.v[2]);
	ori = q_pitch * ori;
}
void Camera::rollLeft(double elapsed_seconds) {
	roll -= heading_speed * elapsed_seconds;
	hasMoved = true;

	versor q_roll = quat_from_axis_deg(roll, fwd.v[0], fwd.v[1], fwd.v[2]);
	ori = q_roll * ori;
}
void Camera::rollRight(double elapsed_seconds) {
	roll += heading_speed * elapsed_seconds;
	hasMoved = true;

	versor q_roll = quat_from_axis_deg(roll, fwd.v[0], fwd.v[1], fwd.v[2]);
	ori = q_roll * ori;
}