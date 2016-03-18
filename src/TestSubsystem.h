/*
 * TestSubsystem.h
 *
 *  Created on: Mar 18, 2016
 *      Author: Gearhead
 */

#ifndef TESTSUBSYSTEM_H_
#define TESTSUBSYSTEM_H_

#include <chrono>

class TestSubsystem {
private:
	enum Mode { kOff, kPotCalibration };
	Mode mode;
	int stage = 0;
	std::chrono::system_clock::time_point testTimeout;
	TestSubsystem();

public:
	static TestSubsystem* GetInstance();
	void periodic();
};


#endif /* TESTSUBSYSTEM_H_ */
