#include "Oculus.h"
#include "OVR_Kernel.h"
#include <iostream>

using namespace OVR;

Oculus::Oculus() : hmd(), posTracked(false), oriTracked(false), pose()
{
	ovr_Initialize();

	hmd = ovrHmd_Create(0);
	if (hmd)
	{
		// Get more details about the HMD.
		ovrSizei resolution = hmd->Resolution;
		// Extract tracking frustum parameters. (external camera)
		float frustomHorizontalFOV = hmd->CameraFrustumHFovInRadians;

		// Start the sensor which provides the Rift’s pose and motion.
		ovrHmd_ConfigureTracking(hmd, ovrTrackingCap_Orientation |
			ovrTrackingCap_MagYawCorrection |
			ovrTrackingCap_Position, 0);

		
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

bool Oculus::isSupported()
{
	return hmd;
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