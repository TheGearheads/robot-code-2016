/*
 * Auton.cpp
 *
 *  Created on: Feb 20, 2016
 *      Author: Gearhead
 */

//Add a state feature like last year?

#include "Auton.h"

Auton* Auton::instance = nullptr;

Auton* Auton::GetInstance() {
	if (instance == nullptr) {
		instance = new Auton();
	}
	return instance;
}

Auton::Auton() {
	auto table = NetworkTable::GetTable("SmartDashboard");
	table->PutStringArray("Auto List", autons);
	drive = Drive::GetInstance();
	timer.Start(); //This should be the last line in the constructor
}

void Auton::reset() {
	timer.Reset();
	auto table = NetworkTable::GetTable("SmartDashboard");
	autonMode = table->GetString("Auto Selector","");
}

//Should NOT be called in TeleopPeriodic. Only in AutonomousPeriodic
void Auton::periodic() {
		//Drive forward
		if (autonMode == autons[0]){
			if (timer.HasPeriodPassed(0.5)) {
				drive->doDrive(0,0);
			} else {
				drive->doDrive(0,0.5);
			}
		}
		else { //If no auton is found stop the robot just in case and put an error in the smart dashboard
			drive->doDrive(0,0);
			DriverStation::ReportError("Auton mode not found");
		}
}
