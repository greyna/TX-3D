#pragma once

#include <windows.h>
#include "GL\glew.h"
#include "OVR.h"
#include "OVR_Version.h"
#include "OVR_CAPI_GL.h"
#include "maths_funcs.h"

using namespace OVR;

class Oculus
{
private:
	ovrHmd hmd;
	bool posTracked, oriTracked;
	Posef pose;
	Sizei renderTargetSize;
	ovrEyeRenderDesc EyeRenderDesc[2];
	ovrFovPort eyesFov[2];
	unsigned int distortionCaps;
	ovrRecti EyeRenderViewport[2];
	GLuint textureId;
	ovrPosef headPose[2];
	Matrix4f proj[2];
	Quatf orientation[2];
	Vector3f position[2];
	Vector3f eyeViewOffset[2];
	ovrGLTexture eyeTexture[2];

	void querySensors();
public:
	Oculus();
	~Oculus();

	// test & init after construction
	bool isSupported();
	ovrSizei getResolution() { return hmd->Resolution; };
	ovrVector2i getWindowPos() { return hmd->WindowsPos; };
	Sizei getTextureSize() { return renderTargetSize; };
	void renderConfig(GLuint tex_id, HWND w);

	// use
	double beginFrame(); // return elapsed time in seconds
	void endFrame();

	void recenter();
	double getTimeSec();
	void dismissWarning();

	mat4 Oculus::getOrientationMatrix(int eye);
	vec3 getOrientationEulerAnglesDeg(int eye);
	versor getOrientation(int eye);
	vec3 getPosition(int eye);
	vec3 getViewOffset(int eye);
	mat4 getProj(int eye);
	ovrSizei getViewportSize(int eye);
	ovrVector2i getViewportPos(int eye);
};

