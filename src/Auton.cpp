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
	autons[0] = "Drive Forward";
	autons[1] = "Do Nothing";
	autons[2] = "Spy Shoot";
	table->PutStringArray("Auto List", autons);
	drive = Drive::GetInstance();
	shooter = Shooter::GetInstance();
	autonMode = ""; //May be unnecessary
	timer.Start(); //This should be the last line in the constructor
	state = 0;
}

void Auton::reset() {
	timer.Reset();
	auto table = NetworkTable::GetTable("SmartDashboard");
	autonMode = table->GetString("Auto Selector","");
	state = 0;
}

//Should NOT be called in TeleopPeriodic. Only in AutonomousPeriodic
void Auton::periodic() {

		//Drive forward
		if (autonMode == autons[0]) {
			switch (state) {
			case 0:
				drive->doDrive(0,0.5);

				if (timer.HasPeriodPassed(2)) {
					state++;
				}
				break;
			default:
				drive->doDrive(0,0);
			}
		}
		// Do nothing
		else if (autonMode == autons[1]) {
			//DriverStation::ReportError("Drive and Shoot not implemented");
			/*
			if (timer.HasPeriodPassed(3)) {
				drive->doDrive(0,0);
			}
			else if (timer.HasPeriodPassed(2)) {
				shooter->enable(false);
				drive->doDrive(0,0.5);
			}
			else if (timer.HasPeriodPassed(1)){
				shooter->fire();
			}
			else {
				shooter->enable(true);
			}
			 */
		}
		// Shoot
		else if (autonMode == autons[2]){
			switch (state) {
			case 0://enables shooter
				shooter->enable(true);
				if (timer.HasPeriodPassed(1)){
					state++;
				}
				break;
			case 1:
				shooter->fire(); //Fire and retract?
				if (timer.HasPeriodPassed(1)){
					state++;
				}
				break;
			/*
			case 2:// Stops shooter
			if (timer.HasPeriodPassed(1)){
					shooter->enable(false);
					state++;
				}
				break;
			*/
			default:
				shooter->enable(false);
			}
		}

		else { //If no auton is found stop the robot just in case and put an error in the smart dashboard
			drive->doDrive(0,0);
			DriverStation::ReportError("Auton mode not found");
		}
}
