// myOverlay.cpp : Defines the entry point for the console application.
//

#include <Windows.h>
#include "stdafx.h"
#include <openvr.h>
#include <GL\glew.h>
#include <gl\GL.h>
#include <GL\GLU.h>
#include <thread>
#include <chrono>

#include "lodepng.h"
#include <vector>
#include <string>


class MyOverlay
{
public:
	MyOverlay();
	~MyOverlay();
};

MyOverlay::MyOverlay()
{

	vr::IVRSystem *m_pHMD = NULL;
	vr::EVRInitError eError = vr::VRInitError_None;
	m_pHMD = vr::VR_Init(&eError, vr::VRApplication_Overlay);

	if (eError != vr::VRInitError_None)
	{
		m_pHMD = NULL;
		char buf[1024];
		sprintf_s(buf, sizeof(buf), "Unable to init VR runtime: %s", vr::VR_GetVRInitErrorAsEnglishDescription(eError));
		printf_s(buf);
	}

	vr::EVROverlayError oError;
	vr::VROverlayHandle_t oHandle = vr::k_ulOverlayHandleInvalid;

	std::string oKey = "Peters Overlay";

	vr::IVROverlay *myOverlay;
	myOverlay = vr::VROverlay();
	vr::VROverlay()->CreateOverlay(oKey.c_str(), "Peters Overlay", &oHandle);
	
	vr::VROverlay()->SetOverlayFromFile(oHandle, "cube_texture.png");
	vr::VROverlay()->SetOverlayWidthInMeters(oHandle, 0.5f);
	vr::VROverlay()->SetOverlayInputMethod(oHandle, vr::VROverlayInputMethod_Mouse);

	vr::HmdMatrix34_t curPos;
	vr::VRChaperoneSetup()->GetWorkingStandingZeroPoseToRawTrackingPose(&curPos);

	curPos.m[0][3] =  0;
	curPos.m[1][3] = 0;
	curPos.m[2][3] = -1;
	vr::VROverlay()->SetOverlayTransformTrackedDeviceRelative(oHandle, vr::k_unTrackedDeviceIndex_Hmd, &curPos);
	vr::VROverlay()->SetOverlayFromFile(oHandle, "C:\\Users\\UNHcFREG\\source\\repos\\myOverlay\\myOverlay\\unhcfreg.jpg");
	vr::VROverlay()->ShowOverlay(oHandle);
	
}

MyOverlay::~MyOverlay()
{
}


int main(int argc, char *argv[])
{
	printf_s(argv[1]);
	int sleep_time = std::stoi(argv[1]);
	MyOverlay *overlay = new MyOverlay();
	std::this_thread::sleep_for(std::chrono::seconds(sleep_time));

    return 0;
}

