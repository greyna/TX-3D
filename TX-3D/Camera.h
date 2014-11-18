#pragma once
#include "maths_funcs.h"
#include "graphics.h"

namespace graphics {
	class Camera
	{
	public:
		Camera();
		~Camera();
		GLfloat * getViewMatrix();
		GLfloat * getProjMatrix(){ return proj_mat; };
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
		float yaw; // y-rotation in degrees
		float pitch;
		float roll;

		float speed; // 1 unit per second
		float heading_speed; // 10 degrees per second

		GLfloat proj_mat[16];
		mat4 view_mat;

		vec3 pos; // position, don't start at zero, or we will be too close
		versor ori; // orientation
		
		vec4 fwd;
		vec4 rgt;
		vec4 up;

		void calcProj();
		float near; // clipping plane
		float far; // clipping plane
		float fov; // field of view in radians
		float aspect; // aspect ratio
	};

}
