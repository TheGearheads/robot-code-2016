/*
 * Auton.h
 *
 *  Created on: Feb 20, 2016
 *      Author: Gearhead
 */

#ifndef SRC_AUTON_H_
#define SRC_AUTON_H_

#include <string>
#include "WPILib.h"
#include "Auton.h"
#include "Drive.h"
#include "Intake.h"
#include "Shooter.h"

class Auton {
private:
	Auton();

	static Auton* instance;

	Drive* drive;
	std::string autonMode;
	std::string autons[1] = {"Drive Forward"};
	Timer timer;
public:
	void reset();
	void periodic();

	static Auton* GetInstance();
};

#endif /* SRC_AUTON_H_ */
