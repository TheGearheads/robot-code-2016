/*
 * Camera.h
 *
 *  Created on: Mar 4, 2016
 *      Author: Gearhead
 */

#ifndef SRC_CAMERA_H_
#define SRC_CAMERA_H_

#include "WPILib.h"
#include "tables/ITable.h"
#include "tables/ITableListener.h"


class Camera : public ITableListener {
private:
	std::shared_ptr<USBCamera> camera;
	CameraServer* server;
	static Camera* instance;
	std::shared_ptr<NetworkTable> m_table;
	Camera();
	~Camera();
public:
	void ValueChanged(ITable* source, llvm::StringRef key,
	                    std::shared_ptr<nt::Value> value, bool isNew) override;
	void UpdateSettings();
	static Camera* GetInstance();
};

#endif /* SRC_CAMERA_H_ */
