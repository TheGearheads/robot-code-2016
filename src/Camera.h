#pragma once

#include <unordered_map>

#include "WPILib.h"
#include "tables/ITable.h"
#include "tables/ITableListener.h"

class Camera : public ITableListener, public USBCamera {
private:
	std::shared_ptr<NetworkTable> m_table;
public:
	static std::unordered_map<std::string, IMAQdxCameraInformation> camInfo;
	static uint16_t cameraCount;
	Camera(std::string name);
	~Camera();
	virtual std::string GetName();
	void ValueChanged(ITable* source, llvm::StringRef key,
			std::shared_ptr<nt::Value> value, bool isNew) override;
	void UpdateSettings();
	static int EnumerateCameras();
};

