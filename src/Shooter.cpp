#include "Shooter.h"

Shooter* Shooter::GetInstance() {
	if (instance == nullptr) {
			instance = new Shooter();
	}
	return instance;
}

Shooter::Shooter() {
	auto pref = Preferences::GetInstance();
	enum MotorPos {kLeft, kRight, kLift};
	const int CANIds[] = {31, 32, 34};
	left = new CANTalon(pref->GetInt("shooter.left", CANIds[kLeft]));
	right = new CANTalon(pref->GetInt("shooter.right", CANIds[kRight]));
	lift = new CANTalon(pref->GetInt("shooter.lift", CANIds[kLift]));
	piston = new Solenoid(pref->GetInt("shooter.piston", 0));

	left->SetInverted(true);
}

void Shooter::setSpeed(float speed) {
	left->Set(speed);
	right->Set(speed);
}

void Shooter::moveTo(float position) {
	lift->Set(position);
}

void Shooter::periodic() {
	if(std::chrono::system_clock::now()>shooterTimeout)
		piston->Set(false);
}

void Shooter::fire() {
	auto pref = Preferences::GetInstance();
	piston->Set(true);
	shooterTimeout=std::chrono::system_clock::now() + std::chrono::milliseconds(pref->GetInt("shooter.timeout", 500));
}

void Shooter::intake() {
	moveTo(0);
	setSpeed(-1);
}

Shooter* Shooter::instance = nullptr;
