#include "MultiCameraServer.h"
#include "Preferences.h"

MultiCameraServer* MultiCameraServer::GetInstance() {
	static MultiCameraServer instance;
	return &instance;
}


MultiCameraServer::MultiCameraServer() : CameraServer() {}

void MultiCameraServer::AutoCapture() {
	Image* frame = imaqCreateImage(IMAQ_IMAGE_RGB, 0);

	while (true) {
		//auto pref = Preferences::GetInstance();
		//SelectCamera(pref->GetString("camera.selected", ""));

		{
			std::lock_guard<priority_recursive_mutex> lockImage(m_imageMutex);
			std::lock_guard<std::mutex> lockCameras(camerasMutex);

			if (cameras.count(selectedCam)) {
				auto camera = cameras[selectedCam];
				camera->GetImage(frame);
				const OverlayTextOptions options = {"Arial", 12, 0, 0, 0, 0, IMAQ_LEFT, IMAQ_TOP, {255, 255, 255, 255}, 0};
				imaqOverlayText(frame, {5, 5}, selectedCam.c_str(), &(IMAQ_RGB_WHITE), &options, nullptr);
			} else {



				imaqSetImageSize(frame, 320, 240);
				// TODO: Make blank
				//imaqDrawTextOnImage(frame, frame, {20, 20}, "No camera found", nullptr, nullptr);
				const OverlayTextOptions options = {"Arial", 16, 0, 0, 0, 0, IMAQ_CENTER, IMAQ_BOTTOM, {255, 255, 255, 255}, 0};
				imaqOverlayText(frame, {320 / 2, 80}, (selectedCam + " not  found").c_str(), &(IMAQ_RGB_WHITE), &options, nullptr);
			}
			imaqMergeOverlay(frame, frame, nullptr, 0, nullptr);
			SetImage(frame);
		}
	}
}

void MultiCameraServer::StartAutomaticCapture() {
	std::lock_guard<priority_recursive_mutex> lock(m_imageMutex);
	if (m_autoCaptureStarted) return;

	m_captureThread = std::thread(&MultiCameraServer::AutoCapture, this);
	m_captureThread.detach();
	m_autoCaptureStarted = true;
}

void MultiCameraServer::AddCamera(std::shared_ptr<Camera> camera) {
	AddCamera(camera->GetName(), camera);
}

void MultiCameraServer::AddCamera(std::string name, std::shared_ptr<USBCamera> camera) {
	std::lock_guard<std::mutex> lock(camerasMutex);
	printf("Adding Camera \"%s\" (%p)\n", name.c_str(), (void*)camera.get());
	cameras[name] = camera;
}

void MultiCameraServer::SelectCamera(std::string name) {
	std::lock_guard<std::mutex> lockCameras(camerasMutex);
	if (cameras.count(name) && selectedCam != name) {
		printf("Selecting Cam: %s\n", name.c_str());
		if (cameras.count(selectedCam)) {
			cameras[selectedCam]->StopCapture();
		}
		selectedCam = name;
		cameras[selectedCam]->StartCapture();
	}
}

void MultiCameraServer::StartAllCameras() {
	Camera::EnumerateCameras();
	for (auto& cam: Camera::camInfo) {
		AddCamera(std::make_shared<Camera>(cam.first));
	}
}
