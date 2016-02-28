#include "Intake.h"

Intake* Intake::GetInstance() {
	if (instance == nullptr) {
			instance = new Intake();
	}
	return instance;
}

Intake::Intake() {
	auto pref = Preferences::GetInstance();
	enum MotorPos {kIntakeRoll, kIntakePivotUp, kIntakePivotDown};
	const int IDs[] = {36, 2, 3};
	rollMotor = new CANTalon(pref->GetInt("intake.roll", IDs[kIntakeRoll]));
	rollMotor->SetInverted(true);
	pivotPiston = new DoubleSolenoid(
			pref->GetInt("intake.pivot.up", IDs[kIntakePivotUp]),
			pref->GetInt("intake.pivot.down", IDs[kIntakePivotDown]));
	stick = Joystick::GetStickForPort(1);
	intakeButtonPrev = pivotButtonPrev = false;
	intakeState = pivotState = rollState = false;
}

void Intake::periodic() {
	//auto pref = Preferences::GetInstance();
	auto table = NetworkTable::GetTable("Intake");

	if (RobotState::IsOperatorControl()) {

		if (!intakeButtonPrev && stick->GetRawButton(4) ){
			intakeState = !intakeState;
			pivotState = intakeState;
		}
		intakeButtonPrev = stick->GetRawButton(4);

		if (!pivotButtonPrev && stick->GetRawButton(5) ){
			pivotState = !pivotState;
		}
		pivotButtonPrev = stick->GetRawButton(5);

		rollState = pivotState && intakeState;

		Shooter::GetInstance()->intake(intakeState);
		lift(pivotState);
		roll(rollState);

		table->PutBoolean("pivot", pivotState);
		table->PutBoolean("intake", intakeState);
		table->PutBoolean("roll", rollState);
	}
}

void Intake::roll(bool on) {
	if(on)
		rollMotor->Set(1);
	else
		rollMotor->Set(0);
}

void Intake::lift(bool position) {
	if(position)
		pivotPiston->Set(DoubleSolenoid::kForward);
	else
		pivotPiston->Set(DoubleSolenoid::kReverse);
}

Intake* Intake::instance = nullptr;
