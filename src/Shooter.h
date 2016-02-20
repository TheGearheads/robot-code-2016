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
private:
	Shooter();

	CANTalon* left;
	CANTalon* right;
	CANTalon* pivot;
	DoubleSolenoid* piston;
	static Shooter* instance;
	std::chrono::system_clock::time_point shooterTimeout;
	Joystick* stick;
	bool shootButtonPrev;
	bool shootState;
	bool intakeState;
	bool fireButtonPrev;
public:
	void setSpeed(float speed);
	void move(float position);
	void fire();
	void intake(bool on); // automatically lower shooter and reverse wheels
	void periodic();
	void enable(bool on);

	static Shooter* GetInstance();
};





#endif /* SRC_SHOOTER_H_ */
