#pragma once
#include <openvr.h>
#include <iostream>

class camera
{
public:
	camera();
	~camera();

	bool getBuff();

	char buf[1024];

	vr::CameraVideoStreamFrameHeader_t frameHeader;
	vr::TrackedCameraHandle_t	m_hTrackedCamera;
	vr::IVRTrackedCamera *camVr;
	vr::EVRTrackedCameraError cError = vr::VRTrackedCameraError_None;

	uint32_t				m_nCameraFrameWidth;
	uint32_t				m_nCameraFrameHeight;
	uint32_t				m_nCameraFrameBufferSize;
	uint8_t					*m_pCameraFrameBuffer;
	uint32_t				m_nLastFrameSequence;

	uint32_t nCameraFrameBufferSize;


};

