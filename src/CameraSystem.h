#ifndef CAMERASYSTEM_H_
#define CAMERASYSTEM_H_

#include "WPILib.h"
#include "MultiCameraServer.h"

class CameraSystem {
private:
	CameraSystem();
	bool switchPrev = false;
	std::string selectedCam = "cam0";

public:
	static CameraSystem* GetInstance();
	void periodic();
};


#endif /* CAMERASYSTEM_H_ */
