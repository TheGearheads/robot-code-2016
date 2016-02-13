/*
 * Auton.cpp
 *
 *  Created on: Feb 13, 2016
 *      Author: Gearhead
 */
#include "Auton.h"
#include "Drive.h"

Auton* Auton::GetInstance() {
	if (instance == nullptr) {
			instance = new Auton();
	}
	return instance;
}

void Auton::aim() {
	double imageWidth = roboRealm->GetNumber("IMAGE_WIDTH", 320); //get image width
	double xPosition;
	double distFromCenter;
	while(IsTest() && IsEnabled())
	{
		xPosition = roboRealm->GetNumber("COG_X", -1.0); //get center of gravity of vision target
		distFromCenter = imageWidth/2.0 - xPosition; //positive means object too far right, negative means too far left
		printf("xPosition: %f, distFromCenter: %f\n", xPosition, distFromCenter);

		if(xPosition != -1) //if set to default value, network table not found
		{
			if(distFromCenter > 15) //vision target too far right, spin right
				mRobot->ArcadeDrive(0.0,0.40);
			else if(distFromCenter < -15) //vision target too far left, spin left
				mRobot->ArcadeDrive(0.0,-0.40);
			else
			{
				mRobot->ArcadeDrive(0.0,0.0); //stop, centered within 15 pixels
				printf("Centered!  "); //lines up with xPosition print above
			}
		}
		else
		{
			printf("Network table error!!!\n");
		}
	}
}
}

void Auton::align() {

}
