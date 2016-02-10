/*
 * Shooter.h
 *
 *  Created on: Feb 9, 2016
 *      Author: Gearhead
 */

#ifndef SRC_SHOOTER_H_
#define SRC_SHOOTER_H_

#include <string>
#include "WPILib.h"
#include <chrono>

class Shooter {

	CANTalon* left;
	CANTalon* right;
	CANTalon* lift;
	Solenoid* piston;
	static Shooter* instance;
	std::chrono::system_clock::time_point shooterTimeout;

public:
	Shooter();
	void setSpeed(float speed);
	void moveTo(float position);
	void fire();
	void intake(); // automatically lower shooter and reverse wheels
	void periodic();

	static Shooter* GetInstance();
};





#endif /* SRC_SHOOTER_H_ */
