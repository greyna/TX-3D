#include "Oculus.h"
#include <iostream>

Oculus::Oculus():
hmd()
{
	ovr_Initialize();

	hmd = ovrHmd_Create(0);
	if (hmd)
	{
		// Get more details about the HMD.
		ovrSizei resolution = hmd->Resolution;
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
