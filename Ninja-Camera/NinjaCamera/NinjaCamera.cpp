// NinjaCamera.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <openvr.h>
#include <iostream>
#include <conio.h>
#include <fstream>
#include <thread>
#include <chrono>
#include <inttypes.h>
#include <QtGui\qimage.h>
#include <QtCore\qstring.h>

using namespace std;

class NinjaCamera
{
public:
	NinjaCamera(int pictures, int sleep_interval);
	~NinjaCamera();

	vr::CameraVideoStreamFrameHeader_t frameHeader;
	vr::TrackedCameraHandle_t	m_hTrackedCamera;
	vr::IVRTrackedCamera *camera;
	vr::EVRTrackedCameraError cError = vr::VRTrackedCameraError_None;

	uint32_t				m_nCameraFrameWidth;
	uint32_t				m_nCameraFrameHeight;
	uint32_t				m_nCameraFrameBufferSize;
	uint8_t					*m_pCameraFrameBuffer;

	uint32_t				m_nLastFrameSequence;

	QImage *image;
	int numImages = 0;

private:
	void NinjaCamera::SetFrameImage(const uint8_t *pFrameImage, uint32_t nFrameWidth, uint32_t nFrameHeight, const vr::CameraVideoStreamFrameHeader_t *pFrameHeader)
	{

		if (!image)
		{
			// allocate to expected dimensions
			image = new QImage(nFrameWidth, nFrameHeight, QImage::Format_RGB32);
		}

		for (uint32_t y = 0; y < nFrameHeight; y++)
		{
			for (uint32_t x = 0; x < nFrameWidth; x++)
			{
				image->setPixel(x, y, QColor(pFrameImage[0], pFrameImage[1], pFrameImage[2]).rgba());
				pFrameImage += 4;
			}
		}
		
		image->save(QString("Pic\QPicture%1.png").arg(numImages), "PNG", 100);
		numImages++;
	}
};

NinjaCamera::NinjaCamera(int pictures, int sleep_interval)
{
	//Initialize OpenVR
	vr::IVRSystem *m_pHMD = NULL;
	vr::EVRInitError eError = vr::VRInitError_None;
	m_pHMD = vr::VR_Init(&eError, vr::VRApplication_Background);

	if (eError != vr::VRInitError_None)
	{
		m_pHMD = NULL;
		char buf[1024];
		sprintf_s(buf, sizeof(buf), "Unable to init VR runtime: %s", vr::VR_GetVRInitErrorAsEnglishDescription(eError));
		printf_s(buf);
	}


	m_nCameraFrameWidth = 0;
	m_nCameraFrameHeight = 0;
	m_nCameraFrameBufferSize = 0;
	m_pCameraFrameBuffer = nullptr;

	m_hTrackedCamera = INVALID_TRACKED_CAMERA_HANDLE;

	camera = vr::VRTrackedCamera();
	if (!camera)
	{
		printf_s("Unable to get Tracked Camera interface.\n");
		return;
	}

	camera->AcquireVideoStreamingService(vr::k_unTrackedDeviceIndex_Hmd, &m_hTrackedCamera);
	if (m_hTrackedCamera == INVALID_TRACKED_CAMERA_HANDLE)
	{
		printf_s("AcquireVideoStreamingService() Failed!\n");
		return;
	}
	else printf_s("Got the handle!!");

	uint32_t nCameraFrameBufferSize = 0;
	if (camera->GetCameraFrameSize(vr::k_unTrackedDeviceIndex_Hmd, vr::VRTrackedCameraFrameType_Undistorted, &m_nCameraFrameWidth, &m_nCameraFrameHeight, &nCameraFrameBufferSize) != vr::VRTrackedCameraError_None)
	{
		printf_s("GetCameraFrameBounds() Failed!\n");
		return;
	}

	if (nCameraFrameBufferSize && nCameraFrameBufferSize != m_nCameraFrameBufferSize)
	{
		delete[] m_pCameraFrameBuffer;
		m_nCameraFrameBufferSize = nCameraFrameBufferSize;
		m_pCameraFrameBuffer = new uint8_t[m_nCameraFrameBufferSize];
		memset(m_pCameraFrameBuffer, 0, m_nCameraFrameBufferSize);
	}

	
	while (pictures > 0)
	{
		cError = camera->GetVideoStreamFrameBuffer(m_hTrackedCamera, vr::VRTrackedCameraFrameType_Undistorted, m_pCameraFrameBuffer, m_nCameraFrameBufferSize, &frameHeader, sizeof(frameHeader));
		if (cError != vr::VRTrackedCameraError_None)
		{
			printf_s("There is an error\n");

			switch (cError)
			{
			case vr::VRTrackedCameraError_InvalidHandle:
				printf_s("Invalid Handle");
			case vr::VRTrackedCameraError_InvalidFrameBufferSize:
				printf_s("Frame Buffer Size");
				break;
			case vr::VRTrackedCameraError_OperationFailed:
				printf_s("Operation Failed");
			case vr::VRTrackedCameraError_IPCFailure:
				printf_s("IPC");
			case vr::VRTrackedCameraError_InvalidArgument:
				printf_s("Invalid argument");
			case vr::VRTrackedCameraError_NotSupportedForThisDevice:
				printf_s("Not supported");
			case vr::VRTrackedCameraError_InvalidSharedTextureHandle:
				printf_s("shared handle");
			case vr::VRTrackedCameraError_SharedMemoryFailure:
				printf_s("Shared memory");
			case vr::VRTrackedCameraError_SharedTextureFailure:
				printf_s("Shared texture");
			case vr::VRTrackedCameraError_StreamSetupFailure:
				printf_s("Stream setup");
			case vr::VRTrackedCameraError_OutOfHandles:
				printf_s("out of handles");
			case vr::VRTrackedCameraError_InvalidFrameHeaderVersion:
				printf_s("Frame header version");
			default:
				printf_s("havent found it yet\n");
				break;
			}
			if (cError == vr::VRTrackedCameraError_NoFrameAvailable)
			{
				printf_s("No frame available");
			}
			else printf_s("No errors with the camera\n");
		}

		printf("Width: %" PRIu32 "\n", frameHeader.nWidth);
		printf("Height: %" PRIu32 "\n", frameHeader.nHeight);

		if (frameHeader.nHeight)
		{
			SetFrameImage(m_pCameraFrameBuffer, frameHeader.nWidth, frameHeader.nHeight, &frameHeader);
			printf_s("Saved an image\n");
			pictures--;
		}


		if (GetAsyncKeyState(VK_ESCAPE))
		{
			break;
		}
		

		std::this_thread::sleep_for(std::chrono::milliseconds(sleep_interval));
	}

	camera->ReleaseVideoStreamingService(m_hTrackedCamera);
}

NinjaCamera::~NinjaCamera()
{
	camera->ReleaseVideoStreamingService(m_hTrackedCamera);
}



int main(int argc, char *argv[])
{
	printf_s(argv[1]);
	printf_s(argv[2]);
	int pictures = std::stoi(argv[1]);
	int sleep_interval = std::stoi(argv[1]);
	NinjaCamera* myCam = new NinjaCamera(pictures, sleep_interval);

    return 0;



}

