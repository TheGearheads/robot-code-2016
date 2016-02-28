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
#include "Shooter.h"

class Intake {
	Intake();

	CANTalon* rollMotor;
	DoubleSolenoid* pivotPiston;
	bool intakeButtonPrev;
	bool intakeState;
	bool pivotState;
	bool pivotButtonPrev;
	bool rollState;
	static Intake* instance;
	Joystick* stick;

public:
	void periodic();
	void roll(bool on);
	void lift(bool position);
	static Intake* GetInstance();
};

#endif /* SRC_INTAKE_H_ */
