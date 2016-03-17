#include "CameraSystem.h"

CameraSystem* CameraSystem::GetInstance() {
	static CameraSystem instance;
	return &instance;
}

CameraSystem::CameraSystem() {
	auto server = MultiCameraServer::GetInstance();
	server->StartAllCameras();
	server->SelectCamera("cam0");
	server->StartAutomaticCapture();
}

void CameraSystem::periodic() {
	auto stick = Joystick::GetStickForPort(1);
	if (stick->GetRawButton(3) && !switchPrev) {
		selectedCam = (selectedCam == "cam0" ? "cam1" : "cam0");
		MultiCameraServer::GetInstance()->SelectCamera(selectedCam);
	}
	switchPrev = stick->GetRawButton(3);
	auto table = NetworkTable::GetTable("Camera");
	table->PutString("selected", selectedCam);
}
