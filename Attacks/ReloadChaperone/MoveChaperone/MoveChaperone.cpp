// MoveChaperone.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <openvr.h>
#include <windows.data.json.h>

int main()
{
	
	vr::IVRSystem *m_pHMD = NULL;
	vr::EVRInitError eError = vr::VRInitError_None;
	m_pHMD = vr::VR_Init(&eError, vr::VRApplication_Background);

	if (eError != vr::VRInitError_None)
	{
		m_pHMD = NULL;
		char buf[1024];
		sprintf_s(buf, sizeof(buf), "Unable to init VR runtime: %s", vr::VR_GetVRInitErrorAsEnglishDescription(eError));
		printf_s(buf);
		return 0;
	}

	bool hmd = vr::VR_IsHmdPresent();
	if (hmd)
	{
		printf_s("Got the HMD\n");
	}


	printf_s("No Errors\n");

	vr::VRChaperoneSetup()->ReloadFromDisk(vr::EChaperoneConfigFile_Live);
	vr::VRChaperoneSetup()->CommitWorkingCopy(vr::EChaperoneConfigFile_Live);
	
    return 0;
}

