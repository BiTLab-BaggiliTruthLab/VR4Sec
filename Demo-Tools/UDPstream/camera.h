#pragma once
#include <openvr.h>
#include <iostream>

class camera
{
public:
	camera();
	~camera();

	struct pose_info
	{
		int device_number;
		int device_type;
		vr::TrackedDevicePose_t pose;
	};

	struct chap_info
	{
		uint32_t num_quads;
		vr::HmdQuad_t quad_buffer[10];
	};

	bool getBuff();
	pose_info camera::getPose(int);
	chap_info getChap();

	char buf[1024];

	vr::CameraVideoStreamFrameHeader_t frameHeader;
	vr::TrackedCameraHandle_t	m_hTrackedCamera;
	vr::IVRTrackedCamera *camVr;
	vr::EVRTrackedCameraError cError = vr::VRTrackedCameraError_None;
	vr::IVRSystem *m_pHMD = NULL;

	uint32_t				m_nCameraFrameWidth;
	uint32_t				m_nCameraFrameHeight;
	uint32_t				m_nCameraFrameBufferSize;
	uint8_t					*m_pCameraFrameBuffer;
	uint32_t				m_nLastFrameSequence;

	uint32_t nCameraFrameBufferSize;
	vr::TrackedDevicePose_t pose_array[vr::k_unMaxTrackedDeviceCount];

	vr::HmdQuad_t quad_buffer[10];
	uint32_t quad_count;
	chap_info m_chap_info;


};

