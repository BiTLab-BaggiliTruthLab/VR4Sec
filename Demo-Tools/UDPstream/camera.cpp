#include "camera.h"



camera::camera()
{

	vr::EVRInitError eError = vr::VRInitError_None;
	m_pHMD = vr::VR_Init(&eError, vr::VRApplication_Background);
	

	if (eError != vr::VRInitError_None)
	{
		m_pHMD = NULL;
		sprintf_s(buf, sizeof(buf), "Unable to init VR runtime: %s", vr::VR_GetVRInitErrorAsEnglishDescription(eError));
		printf_s(buf);
	}

	m_hTrackedCamera = INVALID_TRACKED_CAMERA_HANDLE;

	//Initializes the Camera
	camVr = vr::VRTrackedCamera();
	if (!camVr)
	{
		printf_s("Unable to get Tracked Camera interface.\n");
		return;
	}

	//Gets the Camera Feed, occasionally if you are waking up the camera the first few frames will be bad
	camVr->AcquireVideoStreamingService(vr::k_unTrackedDeviceIndex_Hmd, &m_hTrackedCamera);
	if (m_hTrackedCamera == INVALID_TRACKED_CAMERA_HANDLE)
	{
		printf_s("AcquireVideoStreamingService() Failed!\n");
		return;
	}
	else printf_s("Got the handle!!");

	//Gets the size of the frame, Used to check before creating an image or sending.
	nCameraFrameBufferSize = 0;
	if (camVr->GetCameraFrameSize(vr::k_unTrackedDeviceIndex_Hmd, vr::VRTrackedCameraFrameType_Undistorted, &m_nCameraFrameWidth, &m_nCameraFrameHeight, &nCameraFrameBufferSize) != vr::VRTrackedCameraError_None)
	{
		printf_s("GetCameraFrameBounds() Failed!\n");
		return;
	}

	//Sets the frame buffer size, clears memory space for the buffer.
	if (nCameraFrameBufferSize && nCameraFrameBufferSize != m_nCameraFrameBufferSize)
	{
		delete[] m_pCameraFrameBuffer;
		m_nCameraFrameBufferSize = nCameraFrameBufferSize;
		m_pCameraFrameBuffer = new uint8_t[m_nCameraFrameBufferSize];
		memset(m_pCameraFrameBuffer, 0, m_nCameraFrameBufferSize);
	}
}


camera::~camera()
{
}

bool camera::getBuff() {
	cError = camVr->GetVideoStreamFrameBuffer(m_hTrackedCamera, vr::VRTrackedCameraFrameType_Undistorted, m_pCameraFrameBuffer, m_nCameraFrameBufferSize, &frameHeader, sizeof(frameHeader));
	
	if (cError != vr::VRTrackedCameraError_None)
	{
		printf_s("There is an error\n");
		return false;
	}

	if (frameHeader.nHeight)
	{
		printf_s("Captured an image\n");
		return true;
	}

	return false;
}

camera::pose_info camera::getPose(int device_num) {
	m_pHMD->GetDeviceToAbsoluteTrackingPose(vr::TrackingUniverseOrigin::TrackingUniverseStanding, 0.0f, pose_array, vr::k_unMaxTrackedDeviceCount);
	pose_info m_poseinfo;
	m_poseinfo.device_type = m_pHMD->GetTrackedDeviceClass(device_num);
	m_poseinfo.pose = pose_array[device_num];
	m_poseinfo.device_number = device_num;
	return m_poseinfo;
}

camera::chap_info camera::getChap() {
	
	vr::IVRChaperoneSetup *setup = vr::VRChaperoneSetup();
	vr::IVRChaperone *chaperone = vr::VRChaperone();
	//setup->ReloadFromDisk(vr::EChaperoneConfigFile_Live);
	setup->GetLiveCollisionBoundsInfo(m_chap_info.quad_buffer, &m_chap_info.num_quads);
	return m_chap_info;
}