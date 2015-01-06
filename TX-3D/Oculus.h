#pragma once

#include "OVR.h"
#include "OVR_Version.h"

using namespace OVR;

class Oculus
{
private:
	ovrHmd hmd;
	bool posTracked, oriTracked;
	Posef pose;
	void querySensors();
public:
	Oculus();
	~Oculus();
	bool isSupported();
	void recenter();
	double getTimeSec();
	void dismissWarning();
};

