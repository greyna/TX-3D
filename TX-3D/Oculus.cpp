#include "Oculus.h"
#include "OVR_Kernel.h"
#include <iostream>

using namespace OVR;

Oculus::Oculus() : hmd(), posTracked(false), oriTracked(false), pose(), renderTargetSize(), EyeRenderDesc(),
	eyesFov(), distortionCaps(), eyeTexture(), EyeRenderViewport(), textureId(), headPose(), proj(), orientation(), position(), eyeViewOffset()
{
	ovr_Initialize();

	hmd = ovrHmd_Create(0);
	if (hmd)
	{
		eyesFov[0] = hmd->DefaultEyeFov[0];
		eyesFov[1] = hmd->DefaultEyeFov[1];

		//distortionCaps = hmd->DistortionCaps;
		distortionCaps = ovrDistortionCap_Chromatic | ovrDistortionCap_TimeWarp | ovrDistortionCap_Overdrive;

		// Extract tracking frustum parameters. (external camera)
		//float frustomHorizontalFOV = hmd->CameraFrustumHFovInRadians;

		// Start the sensor which provides the Rift’s pose and motion.
		ovrHmd_ConfigureTracking(hmd, ovrTrackingCap_Orientation |
			ovrTrackingCap_MagYawCorrection |
			ovrTrackingCap_Position, 0);

		// Configure Stereo settings.
		Sizei recommenedTex0Size = ovrHmd_GetFovTextureSize(hmd, ovrEye_Left, hmd->DefaultEyeFov[0], 1.0f);
		Sizei recommenedTex1Size = ovrHmd_GetFovTextureSize(hmd, ovrEye_Right, hmd->DefaultEyeFov[1], 1.0f);

		renderTargetSize.w = recommenedTex0Size.w + recommenedTex1Size.w;
		renderTargetSize.h = (recommenedTex0Size.h > recommenedTex1Size.h ? recommenedTex0Size.h : recommenedTex1Size.h);
	}
	else std::cerr << "hmd not detected" << std::endl;
}

Oculus::~Oculus()
{
	if (hmd)
	{
		ovrHmd_Destroy(hmd);
	}
	ovr_Shutdown();
}

void Oculus::querySensors()
{
	// Query the HMD for the current tracking state.
	ovrTrackingState ts = ovrHmd_GetTrackingState(hmd, getTimeSec());
	posTracked = ts.StatusFlags & ovrStatus_PositionTracked;
	oriTracked = ts.StatusFlags & ovrStatus_OrientationTracked;

	//if (ts.StatusFlags & (ovrStatus_OrientationTracked | ovrStatus_PositionTracked))
	if (posTracked || oriTracked)
	{
		pose = ts.HeadPose.ThePose;
		// example
		float yaw, eyePitch, eyeRoll;
		pose.Rotation.GetEulerAngles<Axis_Y, Axis_X, Axis_Z>(&yaw, &eyePitch, &eyeRoll);
	}
}

void Oculus::renderConfig(GLuint tex_id, HWND w)
{
	textureId = tex_id;

	// Configure OpenGL.
	ovrGLConfig cfg;
	cfg.OGL.Header.API = ovrRenderAPI_OpenGL;
	cfg.OGL.Header.BackBufferSize = Sizei(hmd->Resolution.w, hmd->Resolution.h);
	cfg.OGL.Header.Multisample = 1;
	cfg.OGL.Window = w;
	cfg.OGL.DC = NULL;

	//if (!(hmd->HmdCaps & ovrHmdCap_ExtendDesktop)) // set this flag in oculus config utility (extended mode or direct mode)
	//	ovrHmd_AttachToWindow(hmd, w, NULL, NULL); // Works only on Windows with nvidia GC

	ovrBool result = ovrHmd_ConfigureRendering(hmd, &cfg.Config, distortionCaps, eyesFov, EyeRenderDesc);
	

	EyeRenderViewport[0].Pos = Vector2i(0, 0);
	EyeRenderViewport[0].Size = Sizei(renderTargetSize.w / 2, renderTargetSize.h);
	EyeRenderViewport[1].Pos = Vector2i((renderTargetSize.w + 1) / 2, 0);
	EyeRenderViewport[1].Size = EyeRenderViewport[0].Size;

	eyeTexture[0].OGL.Header.API = ovrRenderAPI_OpenGL;
	eyeTexture[0].OGL.Header.TextureSize = renderTargetSize;
	eyeTexture[0].OGL.Header.RenderViewport = EyeRenderViewport[0];
	eyeTexture[0].OGL.TexId = textureId;

	// Right eye uses the same texture information but a different rendering viewport.
	eyeTexture[1] = eyeTexture[0];
	eyeTexture[1].OGL.Header.RenderViewport = EyeRenderViewport[1];

	proj[0] = ovrMatrix4f_Projection(EyeRenderDesc[0].Fov,
		0.01f, // near plane
		10000.0f, // far plane
		true); // false for left-handed like in OpenGL

	proj[1] = ovrMatrix4f_Projection(EyeRenderDesc[0].Fov,
		0.01f, // near plane
		10000.0f, // far plane
		true); // false for left-handed like in OpenGL
}

bool Oculus::isSupported()
{
	return hmd;
}


double Oculus::beginFrame()
{
	ovrFrameTiming hmdFrameTiming = ovrHmd_BeginFrame(hmd, 0);

	static double previous_seconds = hmdFrameTiming.ThisFrameSeconds;
	double current_seconds = hmdFrameTiming.ThisFrameSeconds;
	double elapsed_seconds = current_seconds - previous_seconds;
	previous_seconds = current_seconds;

	for (int eyeIndex = 0; eyeIndex < ovrEye_Count; eyeIndex++)
	{
		ovrEyeType eye = hmd->EyeRenderOrder[eyeIndex];
		headPose[eye] = ovrHmd_GetHmdPosePerEye(hmd, eye);
		orientation[eye] = Quatf(headPose[eye].Orientation);
		position[eye] = Vector3f(headPose[eye].Position);
		eyeViewOffset[eye] = Vector3f(EyeRenderDesc[eye].HmdToEyeViewOffset);
	}

	return elapsed_seconds;
}

void Oculus::endFrame()
{
	ovrHmd_EndFrame(hmd, headPose, &eyeTexture[0].Texture);
}

void Oculus::recenter() {
	ovrHmd_RecenterPose(hmd);
}

double Oculus::getTimeSec()
{
	return ovr_GetTimeInSeconds();
}

void Oculus::dismissWarning()
{
	ovrHmd_DismissHSWDisplay(hmd);
}

versor Oculus::getOrientation(int eye) {
	return versor(orientation[eye].x, orientation[eye].y, orientation[eye].z, orientation[eye].w);
}
vec3 Oculus::getPosition(int eye) {
	return vec3(position[eye].x, position[eye].y, position[eye].z);
}
vec3 Oculus::getViewOffset(int eye) {
	return vec3(eyeViewOffset[eye].x, eyeViewOffset[eye].y, eyeViewOffset[eye].z);
}
mat4 Oculus::getProj(int eye) {
	return mat4(
		proj[eye].M[0][0], proj[eye].M[1][0], proj[eye].M[2][0], proj[eye].M[3][0],
		proj[eye].M[0][1], proj[eye].M[1][1], proj[eye].M[2][1], proj[eye].M[3][1],
		proj[eye].M[0][2], proj[eye].M[1][2], proj[eye].M[2][2], proj[eye].M[3][2],
		proj[eye].M[0][3], proj[eye].M[1][3], proj[eye].M[2][3], proj[eye].M[3][3]
	);
	/*return mat4(
		proj[eye].M[0][0], proj[eye].M[0][1], proj[eye].M[0][2], proj[eye].M[0][3],
		proj[eye].M[1][0], proj[eye].M[1][1], proj[eye].M[1][2], proj[eye].M[1][3],
		proj[eye].M[2][0], proj[eye].M[2][1], proj[eye].M[2][2], proj[eye].M[2][3],
		proj[eye].M[3][0], proj[eye].M[3][1], proj[eye].M[3][2], proj[eye].M[3][3]
	);*/
}
ovrSizei Oculus::getViewportSize(int eye) {
	return EyeRenderViewport[eye].Size;
}
ovrVector2i Oculus::getViewportPos(int eye) {
	return EyeRenderViewport[eye].Pos;
}