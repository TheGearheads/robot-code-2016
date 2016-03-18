/*
 * TestSubsystem.cpp
 *
 *  Created on: Mar 18, 2016
 *      Author: Gearhead
 */

#include "TestSubsystem.h"
#include "Shooter.h"

TestSubsystem* TestSubsystem::GetInstance() {
	static TestSubsystem instance;
	return &instance;
}

TestSubsystem::TestSubsystem() {
	stage = 0;
	mode = Mode::kOff;
}

void TestSubsystem::periodic() {
	auto table = NetworkTable::GetTable("Shooter");
	auto stick = Joystick::GetStickForPort(1);
	if (stick->GetRawButton(1)) {
		stage = 0;
		mode = Mode::kPotCalibration;
	}
	if (mode == Mode::kPotCalibration) {
		table->PutNumber("stage", stage);
		table->PutString("mode", "PotCalibration");
		auto shooter = Shooter::GetInstance();
		auto pref = Preferences::GetInstance();
		auto now = std::chrono::system_clock::now();
		if (stage == 0) {
			shooter->move(-0.5);
			if (shooter->getLimitTop()) {
				stage++;
				testTimeout = now + std::chrono::seconds(1);
			}
		} else if (stage == 1) {
			if (now > testTimeout) {
				printf("Setting max to: %f\n", shooter->getPosition());
				pref->PutDouble("shooter.pivot.pot.max", shooter->getPosition());
				stage++;
			}
		} else if (stage == 2) {
			shooter->move(0.5);
			if (shooter->getLimitBottom()) {
				stage++;
				testTimeout = now + std::chrono::seconds(1);
			}
		} else if (stage == 3) {
			if (now > testTimeout) {
				printf("Setting min to: %f\n", shooter->getPosition());
				pref->PutDouble("shooter.pivot.pot.min", shooter->getPosition());
				stage++;
			}
		} else if (stage == 4) {
			shooter->moveTo(0);
			stage++;
			mode = Mode::kOff;
		}
	}
}


