/*
 * Drive.h
 *
 *  Created on: Jan 17, 2015
 *      Author: Gabs
 */

#ifndef SRC_DRIVE_H_
#define SRC_DRIVE_H_

#include <string>
#include "WPILib.h"

class Drive {

	CANTalon* frontLeft;
	CANTalon* midLeft;
	CANTalon* rearLeft;
	CANTalon* frontRight;
	CANTalon* midRight;
	CANTalon* rearRight;
	RobotDrive* robotDrive;
	static Drive* instance;

public:
	Drive();
	void doDrive(float x, float y);
	static Drive* GetInstance();
};




#endif /* SRC_DRIVE_H_ */
