#include "Camera.h"

std::unordered_map<std::string, IMAQdxCameraInformation> Camera::camInfo;
uint16_t Camera::cameraCount;

void Camera::ValueChanged(ITable* source, llvm::StringRef key, std::shared_ptr<nt::Value> value, bool isNew) {
	std::string keyBase = std::string("camera.") + m_name;
	if (key == keyBase + ".brightness" ||
			key == keyBase + ".exposure" ||
			key == keyBase + ".whitebalance") {
		UpdateSettings();
	}
	//if (key == keyBase + ".dump" && value->GetBoolean()) {
		//DumpAttrs();
	//}
}

Camera::Camera(std::string name) : USBCamera(name, true) {
	EnumerateCameras();

	if (strstr(camInfo[name].ModelName, "LifeCam") != nullptr) {
		SetSize(320, 240);
		SetFPS(15);
	}

	OpenCamera();
	m_table = NetworkTable::GetTable("Preferences");
	m_table->AddTableListener(this);
	UpdateSettings();
}

Camera::~Camera() {
	m_table->RemoveTableListener(this);
}

std::string Camera::GetName() {
	return m_name;
}

void Camera::UpdateSettings() {
	auto pref = Preferences::GetInstance();

	std::string keyBase = std::string("camera.") + m_name;
	std::cout << "Updating camera settings of " << m_name << ": Brightness = " << pref->GetInt(keyBase + ".brightness", 80);
	std::cout << ", WhiteBalance = " << pref->GetInt(keyBase + ".whitebalance", -1);
	std::cout << ", Exposure = " << pref->GetInt(keyBase + ".exposure", -1) << std::endl;
	SetBrightness(pref->GetInt(keyBase + ".brightness", 80));
	auto value = pref->GetInt(keyBase + ".whitebalance", -1);
	if (value == -1) {
		SetWhiteBalanceAuto();
	} else {
		SetWhiteBalanceManual(value);
	}
	value = pref->GetInt(keyBase + ".exposure", -1);
	if (value == -1) {
		SetExposureAuto();
	} else {
		SetExposureManual(value);
	}
}

int Camera::EnumerateCameras() {
	if (cameraCount > 0) { return cameraCount; }
	IMAQdxCameraInformation cams[6];
	cameraCount = 0;
	uInt32 count = sizeof(cams) / sizeof(cams[0]);
	IMAQdxEnumerateCameras(cams, &count, true);
	cameraCount = count;
	printf("Cameras (%u):\n", (unsigned int)cameraCount);
	for (uInt32 i=0; i != cameraCount; i++) {
		printf("  %s / %s / %s\n", cams[i].VendorName, cams[i].ModelName, cams[i].InterfaceName);
		camInfo[std::string(cams[i].InterfaceName)] = cams[i];
	}
	return cameraCount;
}
