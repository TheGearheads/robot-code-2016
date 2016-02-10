#include "Intake.h"

Intake* Intake::GetInstance() {
	if (instance == nullptr) {
			instance = new Intake();
	}
	return instance;
}

Intake::Intake() {
	auto pref = Preferences::GetInstance();
	enum MotorPos {kIntakeRoll, kIntakeLift};
			const int CANIds[] = {40, 41};
			rollMotor = new CANTalon(pref->GetInt("intake.roll", CANIds[kIntakeRoll]));
			liftMotor = new CANTalon(pref->GetInt("intake.lift", CANIds[kIntakeLift]));
}

void Intake::roll(bool on) {
	if(on)
		rollMotor->Set(1);
	else
		rollMotor->Set(0);
}

void Intake::lift(bool position) {
	if(position)
		liftMotor->Set(1);
	else
		liftMotor->Set(-1);
}

Intake* Intake::instance = nullptr;
