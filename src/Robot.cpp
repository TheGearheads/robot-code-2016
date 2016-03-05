//Hashtag Includes
#include "WPILib.h"
#include "Auton.h"
#include "Drive.h"
#include "Intake.h"
#include "Shooter.h"
#include "Camera.h"

class Robot: public IterativeRobot
{
private:
	LiveWindow *lw = LiveWindow::GetInstance();
	Auton* auton;
	Joystick* stick;
	Drive* drive;
	Shooter* shooter;
	Intake* intake;
	Camera* camera;

	void RobotInit() {
		stick = Joystick::GetStickForPort(0);
		auton = Auton::GetInstance();
		drive = Drive::GetInstance();
		shooter = Shooter::GetInstance();
		intake = Intake::GetInstance();
		camera = Camera::GetInstance();
		//lw->SetEnabled(true);
	}

	void AutonomousInit() {
		auton->reset();
	}

	void AutonomousPeriodic() {
		//Note doDrive() should not be called here
		auton->periodic();
		intake->periodic();
		shooter->periodic();
	}

	void TeleopInit() {
	}

	void TeleopPeriodic() {
		// Comment the next line out to disable movement
		drive->doDrive(stick->GetX(), -stick->GetY());
		intake->periodic();
		shooter->periodic();
		lw->Run();
	}

	void TestPeriodic() {
		lw->Run();
	}
};

START_ROBOT_CLASS(Robot)
