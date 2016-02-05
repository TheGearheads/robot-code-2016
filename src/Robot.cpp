#include "WPILib.h"

class Robot: public IterativeRobot
{
private:
	LiveWindow *lw = LiveWindow::GetInstance();
	Joystick* stick;
	RobotDrive* drive;
	CANTalon* frontLeft;
	CANTalon* frontRight;
	CANTalon* rearLeft;
	CANTalon* rearRight;
	CANTalon* midLeft;
	CANTalon* midRight;

	void RobotInit() {
		enum MotorPos {kFrontLeft, kMidLeft, kRearLeft, kFrontRight, kMidRight, kRearRight};
		const int CANIds[] = {37, 38, 33, 30, 35, 39};
		stick = Joystick::GetStickForPort(0);
		frontLeft = new CANTalon(CANIds[kFrontLeft]);
		midLeft = new CANTalon(CANIds[kMidLeft]);
		rearLeft = new CANTalon(CANIds[kRearLeft]);

		frontRight = new CANTalon(CANIds[kFrontRight]);
		midRight = new CANTalon(CANIds[kMidRight]);
		rearRight = new CANTalon(CANIds[kRearRight]);

		frontRight->SetInverted(true);
		midRight->SetInverted(true);
		rearRight->SetInverted(true);

		frontLeft->SetControlMode(CANTalon::ControlMode::kFollower);
		frontRight->SetControlMode(CANTalon::ControlMode::kFollower);

		frontLeft->Set((float)CANIds[kMidLeft]);
		frontRight->Set((float)CANIds[kMidRight]);

		drive = new RobotDrive(midLeft, rearLeft, midRight, rearRight);
		drive->SetSafetyEnabled(false);
	}

	void AutonomousInit() {
	}

	void AutonomousPeriodic() {
	}

	void TeleopInit() {
	}

	void TeleopPeriodic() {
		drive->ArcadeDrive(stick->GetX(), stick->GetY());
	}

	void TestPeriodic() {
		lw->Run();
	}
};

START_ROBOT_CLASS(Robot)
