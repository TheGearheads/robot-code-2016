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
	float prevPos;
public:
	void setSpeed(float speed);
	void move(float speed);
	double potToAngle(double pot);
	double angleToPot(double angle);
	double getAngle();
	void moveTo(float position);
	void fire();
	void intake(bool on); // automatically lower shooter and reverse wheels
	void periodic();
	void enable(bool on);
	bool isEnabled() { return shootState; }
	bool isIntaking() { return intakeState; }
	bool isFiring() { return piston->Get() == DoubleSolenoid::kForward; }
	bool getLimitTop() { return pivot->IsFwdLimitSwitchClosed(); }
	bool getLimitBottom() { return pivot->IsRevLimitSwitchClosed(); }
	double getPosition();

	static Shooter* GetInstance();
};





#endif /* SRC_SHOOTER_H_ */
