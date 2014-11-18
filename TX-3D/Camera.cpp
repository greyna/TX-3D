#include "Camera.h"
#include <math.h>

namespace graphics {
	Camera::Camera() :
		hasMoved(true), move(0.0f, 0.0f, 0.0f), yaw(0.0f), pitch(0.0f), roll(0.0f),
		speed(3.0f), heading_speed(30.0f),
		pos(0.0f, 0.0f, 5.0f), ori(quat_from_axis_deg(0.0f, 0.0f, 1.0f, 0.0f)),
		fwd(0.0f, 0.0f, -1.0f, 0.0f), rgt(1.0f, 0.0f, 0.0f, 0.0f), up(0.0f, 1.0f, 0.0f, 0.0f),
		near(0.1f), far(100.0f), fov(67.0f * ONE_DEG_IN_RAD), aspect((float)g_gl_width / (float)g_gl_height)
	{

		calcProj();

		mat4 R = quat_to_mat4(ori);
		mat4 T = translate(identity_mat4(), vec3(-pos.v[0], -pos.v[1], -pos.v[2]));
		view_mat = R*T;
	}

	Camera::~Camera()
	{
	}

	void Camera::calcProj() {
		float range = tan(fov * 0.5f) * near;
		float Sx = (2.0f * near) / (range * aspect + range * aspect);
		float Sy = near / range;
		float Sz = -(far + near) / (far - near);
		float Pz = -(2.0f * far * near) / (far - near);

		for (int i = 0; i < 16; ++i) {
			proj_mat[i] = 0.0f;
		}
		proj_mat[0] = Sx;
		proj_mat[5] = Sy;
		proj_mat[10] = Sz;
		proj_mat[11] = -1.0f;
		proj_mat[14] = Pz;
	}

	void Camera::update() {
		if (hasMoved) {
			mat4 R = quat_to_mat4(ori);

			fwd = R * vec4(0.0, 0.0, -1.0, 0.0);
			rgt = R * vec4(1.0, 0.0, 0.0, 0.0);
			up = R * vec4(0.0, 1.0, 0.0, 0.0);

			pos = pos + vec3(fwd) * -move.v[2];
			pos = pos + vec3(up) * move.v[1];
			pos = pos + vec3(rgt) * move.v[0];

			mat4 T = translate(identity_mat4(), pos);

			view_mat = inverse(R) * inverse(T);

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

	GLfloat * Camera::getViewMatrix() {
		update();
		return view_mat.m;
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
}