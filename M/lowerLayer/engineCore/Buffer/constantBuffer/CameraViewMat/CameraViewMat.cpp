#include "CameraViewMat.h"
#include "../WinApp.h"

CameraViewMat* CameraViewMat::Get()
{
	static CameraViewMat cameraViewMat;
	
	return &cameraViewMat;

}

CameraViewMat::CameraViewMat()
{
	viewMat.CreateAndMapping(WinApp::deviceSetUp.Getter_Device());
}

