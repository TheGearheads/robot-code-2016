#pragma once

#include <unordered_map>

#include "CameraServer.h"
#include "nivision.h"

#include "Camera.h"

class MultiCameraServer : public CameraServer {
	private:
		static constexpr uint32_t kMaxImageSize = 200000;
		std::unordered_map<std::string, std::shared_ptr<USBCamera>> cameras;
		std::mutex camerasMutex;
		std::string selectedCam;
		MultiCameraServer();
	public:
		static MultiCameraServer* GetInstance();
		void AutoCapture();
		void StartAutomaticCapture();
		void AddCamera(std::shared_ptr<Camera> camera);
		void AddCamera(std::string name, std::shared_ptr<USBCamera> camera);
		void SelectCamera(std::string name);
		void StartAllCameras();
};
