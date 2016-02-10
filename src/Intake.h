/*
 * Intake.h
 *
 *  Created on: Feb 9, 2016
 *      Author: Gearhead
 */

#ifndef SRC_INTAKE_H_
#define SRC_INTAKE_H_

#include <string>
#include "WPILib.h"

class Intake {

	CANTalon* rollMotor;
	CANTalon* liftMotor;

	static Intake* instance;

public:
	Intake();
	void roll(bool on);
	void lift(bool position);
	void doDrive(float x, float y);
	static Intake* GetInstance();
};

#endif /* SRC_INTAKE_H_ */
