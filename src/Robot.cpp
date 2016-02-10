#include "WPILib.h"
#include "Drive.h"
#include "Shooter.h"

class Robot: public IterativeRobot
{
private:
	LiveWindow *lw = LiveWindow::GetInstance();
	Joystick* stick;
	Drive* drive;
	Shooter* shooter;

	void RobotInit() {
		drive = Drive::GetInstance();
	}

	void AutonomousInit() {
	}

	void AutonomousPeriodic() {
	}

	void TeleopInit() {
	}

	void TeleopPeriodic() {
		drive->doDrive(stick->GetX(), stick->GetY());
		shooter->periodic();
	}

	void TestPeriodic() {
		lw->Run();
	}
};

START_ROBOT_CLASS(Robot)
