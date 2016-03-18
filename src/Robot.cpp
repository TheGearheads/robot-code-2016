//Hashtag Includes
#include "WPILib.h"
#include "Auton.h"
#include "Drive.h"
#include "Intake.h"
#include "Shooter.h"
#include "CameraSystem.h"
#include "LEDSystem.h"
#include "TestSubsystem.h"

class Robot: public IterativeRobot
{
private:
	LiveWindow *lw = LiveWindow::GetInstance();
	Auton* auton;
	Joystick* stick;
	Drive* drive;
	Shooter* shooter;
	Intake* intake;
	CameraSystem* camSystem;
	TestSubsystem* testSystem;

	void RobotInit() {
		LEDSystem::GetInstance();
		stick = Joystick::GetStickForPort(0);
		auton = Auton::GetInstance();
		drive = Drive::GetInstance();
		shooter = Shooter::GetInstance();
		intake = Intake::GetInstance();
		camSystem = CameraSystem::GetInstance();
		testSystem = TestSubsystem::GetInstance();
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
		camSystem->periodic();
	}

	void TestPeriodic() {
		shooter->periodic();
		camSystem->periodic();
		testSystem->periodic();
	}
};

START_ROBOT_CLASS(Robot)
