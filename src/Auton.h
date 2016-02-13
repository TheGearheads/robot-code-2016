/*
 * Auton.h
 *
 *  Created on: Feb 13, 2016
 *      Author: Gearhead
 */

#ifndef SRC_AUTON_H_
#define SRC_AUTON_H_

class Auton {

	static Auton* instance;

public:
	Auton();
	void aim(); // adjusts robot aim
	void align(); // moves robot in front of goal
	static Auton* GetInstance();
	std::shared_ptr<NetworkTable> roboRealm; //Network table object, communicate with RoboRealm
};



#endif /* SRC_AUTON_H_ */
