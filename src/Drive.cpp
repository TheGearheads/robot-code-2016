#include "Drive.h"

Drive* Drive::GetInstance() {
	if (instance == nullptr) {
			instance = new Drive();
	}
	return instance;
}

Drive::Drive() {
	auto pref = Preferences::GetInstance();
	enum MotorPos {kFrontLeft, kMidLeft, kRearLeft, kFrontRight, kMidRight, kRearRight};
			const int CANIds[] = {37, 38, 33, 30, 35, 39};
			frontLeft = new CANTalon(pref->GetInt("drive.frontLeft", CANIds[kFrontLeft]));
			midLeft = new CANTalon(pref->GetInt("drive.midLeft", CANIds[kMidLeft]));
			rearLeft = new CANTalon(pref->GetInt("drive.rearLeft", CANIds[kRearLeft]));

			frontRight = new CANTalon(pref->GetInt("drive.frontRight", CANIds[kFrontRight]));
			midRight = new CANTalon(pref->GetInt("drive.midRight", CANIds[kMidRight]));
			rearRight = new CANTalon(pref->GetInt("drive.rearRight", CANIds[kRearRight]));

			frontRight->SetInverted(true);
			midRight->SetInverted(true);
			rearRight->SetInverted(true);

			frontLeft->SetControlMode(CANTalon::ControlMode::kFollower);
			frontRight->SetControlMode(CANTalon::ControlMode::kFollower);

			frontLeft->Set((float)pref->GetInt("drive.midLeft", CANIds[kMidLeft]));
			frontRight->Set((float)pref->GetInt("drive.midRight", CANIds[kMidRight]));

			robotDrive = new RobotDrive(midLeft, rearLeft, midRight, rearRight);
			robotDrive->SetSafetyEnabled(false);
}

void Drive::doDrive(float x, float y) {
	robotDrive->ArcadeDrive(x, y);
}

Drive* Drive::instance = nullptr;



