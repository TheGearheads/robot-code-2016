/*
 * Camera.cpp
 *
 *  Created on: Mar 4, 2016
 *      Author: Gearhead
 */

#include <Camera.h>

Camera* Camera::GetInstance() {
	if (instance == nullptr) {
			instance = new Camera();
	}
	return instance;
}

void Camera::ValueChanged(ITable* source, llvm::StringRef key,
                            std::shared_ptr<nt::Value> value, bool isNew) {
	if (key == "camera.brightness" ||
			key == "camera.exposure" ||
			key == "camera.whitebalance") {
		UpdateSettings();
	}
}

Camera::Camera() {
	camera = std::make_shared<USBCamera>("cam0", true);
	camera->OpenCamera();
	server = CameraServer::GetInstance();
	server->StartAutomaticCapture(camera);
	m_table = NetworkTable::GetTable("Preferences");
	m_table->AddTableListener(this);
	UpdateSettings();
}

Camera::~Camera() {
	m_table->RemoveTableListener(this);
}
void Camera::UpdateSettings() {
	auto pref = Preferences::GetInstance();

	std::cout << "Updating camera settings: Brightness = " << pref->GetInt("camera.brightness", 80);
	std::cout << ", WhiteBalance = " << pref->GetInt("camera.whitebalance", -1);
	std::cout << ", Exposure = " << pref->GetInt("camera.exposure", -1) << std::endl;
	camera->SetBrightness(pref->GetInt("camera.brightness", 80));
	auto value = pref->GetInt("camera.whitebalance", -1);
	if (value == -1) {
		camera->SetWhiteBalanceAuto();
	} else {
		camera->SetWhiteBalanceManual(value);
	}
	value = pref->GetInt("camera.exposure", -1);
	if (value == -1) {
		camera->SetExposureAuto();
	} else {
		camera->SetExposureManual(value);
	}
}

Camera* Camera::instance = nullptr;

