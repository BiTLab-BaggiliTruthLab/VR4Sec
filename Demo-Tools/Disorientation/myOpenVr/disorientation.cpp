// myOpenVr.cpp : Defines the entry point for the console application.
//

#include <windows.h>
#include <cstdlib>
#include "lodepng.h"
#include "stdafx.h"
#include "openvr.h"
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <cmath>
#include <thread>
#include <chrono>
#include "SFML\Network.hpp"



using namespace std;


vr::HmdQuad_t quad_buffer[10];
uint32_t quad_count;
vr::HmdMatrix34_t matrix;
vr::TrackedDevicePose_t pose_array[vr::k_unMaxTrackedDeviceCount];

const int BUF_LEN = 65540;
uint8_t buffer[BUF_LEN];
size_t recieved;
sf::IpAddress rIp;
sf::TcpSocket socket;
char* server_ip = "localhost";
unsigned short port = 8888;

int main()
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
		exit(EXIT_FAILURE);
	}

	bool hmd = vr::VR_IsHmdPresent();
	if (hmd)
	{
		printf_s("Got the HMD\n");
	}

	printf_s("No Errors\n");
	
	vr::IVRChaperoneSetup *setup = vr::VRChaperoneSetup();
	vr::IVRChaperone *chaperone = vr::VRChaperone();

	vr::ChaperoneCalibrationState state =  chaperone->GetCalibrationState();
	

	/*
	switch (state)
	{
	case vr::ChaperoneCalibrationState_OK:
		OutputDebugStringW(L"OK state");
		break;
	case vr::ChaperoneCalibrationState_Warning:
		OutputDebugStringW(L"Warning");
		break;
	case vr::ChaperoneCalibrationState_Warning_BaseStationMayHaveMoved:
		OutputDebugStringW(L"Base station moved");
		break;
	case vr::ChaperoneCalibrationState_Warning_BaseStationRemoved:
		OutputDebugStringW(L"Base station removed");
		break;
	case vr::ChaperoneCalibrationState_Warning_SeatedBoundsInvalid:
		OutputDebugStringW(L"Seated Bounds invalid");
		break;
	case vr::ChaperoneCalibrationState_Error:
		OutputDebugStringW(L"Error bad universe\n");
		break;
	case vr::ChaperoneCalibrationState_Error_BaseStationUninitialized:
		OutputDebugStringW(L"Base station un init\n");
		break;
	case vr::ChaperoneCalibrationState_Error_BaseStationConflict:
		OutputDebugStringW(L"Base station conflict\n");
		break;
	case vr::ChaperoneCalibrationState_Error_PlayAreaInvalid:
		OutputDebugStringW(L"Play area invalid\n");
		break;
	case vr::ChaperoneCalibrationState_Error_CollisionBoundsInvalid:
		OutputDebugStringW(L"Collision bounds invalid\n");
		break;
	default:
		break;
	}
	*/


	sf::Socket::Status status = socket.connect("", 8888);
	if (status != sf::Socket::Done)
	{
		cout << "Connection failed" << endl;
		// error...
	}
	cout << "Connection successfull" << endl;
	socket.receive(buffer, sizeof(buffer), recieved);
	if (recieved > 0)
	{
		std::cout << "Connected to " << rIp << "Port: " << port << "Buffer:" << buffer << endl;
	}

	int counter = 0;
	while (true) {
		counter++;
		socket.receive(buffer, sizeof(buffer), recieved);
		cout << "Packet: " << counter << "    " << buffer << endl;
		setup->ReloadFromDisk(vr::EChaperoneConfigFile_Live);
		setup->CommitWorkingCopy(vr::EChaperoneConfigFile_Live);
		socket.send(buffer, 50);
		//this_thread::sleep_for(chrono::milliseconds(100));


		/*
		setup->GetLiveCollisionBoundsInfo(NULL, &quad_count);
		setup->GetLiveCollisionBoundsInfo(quad_buffer, &quad_count);
		if (quad_count < 10){
			for (int i = 0; i < quad_count; i++) {
				for (int j = 0; j < 4; j++) {
					quad_buffer[i].vCorners[j].v[0] += 0.1;
				}
			}
		}

		//setup->SetWorkingCollisionBoundsInfo(quad_buffer, quad_count);
		m_pHMD->GetDeviceToAbsoluteTrackingPose(vr::TrackingUniverseOrigin::TrackingUniverseStanding, 0.0f, pose_array, vr::k_unMaxTrackedDeviceCount);
		setup->GetWorkingStandingZeroPoseToRawTrackingPose(&matrix);
		double shift = (rand() % 100 - 50) / 600.0;
		shift = 0.01;
		cout << shift << endl;
		//matrix.m[1][1] += cos(shift);
		//matrix.m[2][2] += cos(shift);
		//matrix.m[1][2] += -sin(shift);
		//matrix.m[2][1] += sin(shift);
		//matrix.m[0][3] -= shift;
		//matrix.m[1][3] -= shift;
	//	matrix.m[2][3] -= shift;

		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 4; j++) {
				float controller = pose_array[1].mDeviceToAbsoluteTracking.m[i][j];
				matrix.m[i][j] += controller / 10.0;
 			}
		}
		*/
		//setup->SetWorkingStandingZeroPoseToRawTrackingPose(&pose_array[1].mDeviceToAbsoluteTracking);
		
		//setup->CommitWorkingCopy(vr::EChaperoneConfigFile_Live);
	

		//cout << "Commited" << endl;
	}

	//
	//
	
	printf_s("End\n");

    return 0;
}


