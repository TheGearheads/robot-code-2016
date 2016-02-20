#include "Shooter.h"

Shooter* Shooter::instance = nullptr;

Shooter* Shooter::GetInstance() {
	if (instance == nullptr) {
			instance = new Shooter();
	}
	return instance;
}

Shooter::Shooter() {
	shootButtonPrev = fireButtonPrev = false;
	intakeState = shootState = false;
	auto pref = Preferences::GetInstance();
	enum MotorPos {kLeft, kRight, kPivot};
	const int CANIds[] = {42, 40, 41};
	left = new CANTalon(pref->GetInt("shooter.left", CANIds[kLeft]));
	right = new CANTalon(pref->GetInt("shooter.right", CANIds[kRight]));
	pivot = new CANTalon(pref->GetInt("shooter.pivot", CANIds[kPivot]));
	pivot->SetFeedbackDevice(CANTalon::AnalogPot);
	piston = new DoubleSolenoid(
			pref->GetInt("shooter.piston.out", 0),
			pref->GetInt("shooter.piston.in", 1));

	right->SetInverted(true);
	pivot->SetInverted(true);
	stick = Joystick::GetStickForPort(1);
}

void Shooter::setSpeed(float speed) {
	left->Set(speed);
	right->Set(speed);
}

void Shooter::move(float speed) {
	auto pref = Preferences::GetInstance();
	pivot->Set(pref->GetFloat("shooter.pivot.modifier", 0.3f) * speed);
}

void Shooter::periodic() {
	auto pref = Preferences::GetInstance();
	auto table = NetworkTable::GetTable("Shooter");

	if(std::chrono::system_clock::now()>shooterTimeout)
		piston->Set(DoubleSolenoid::kReverse);

	if (RobotState::IsOperatorControl()){
		move(stick->GetY());
		if (!shootButtonPrev && stick->GetRawButton(2) ){
			enable(!shootState);
		}
		shootButtonPrev = stick->GetRawButton(2);
		if (!fireButtonPrev && stick->GetRawButton(1) ){
			fire();
		}
		fireButtonPrev = stick->GetRawButton(1);
	}

	if (intakeState){
		setSpeed(pref->GetFloat("shooter.intake.speed", -0.1f));
		shootState = false;
	}else if (shootState){
		setSpeed((stick->GetZ()));
	}else {
		setSpeed(0);
	}

	table->PutNumber("angle", pivot->GetPosition());
	table->PutBoolean("state", shootState);
	table->PutBoolean("fire", piston->Get() == DoubleSolenoid::kForward);
	table->PutNumber("pivot/bus", pivot->GetBusVoltage());
	table->PutNumber("pivot/out", pivot->GetOutputVoltage());
	table->PutNumber("pivot/current", pivot->GetOutputCurrent());
}

void Shooter::fire() {
	auto pref = Preferences::GetInstance();
	if (!intakeState){
		piston->Set(DoubleSolenoid::kForward);
		shooterTimeout=std::chrono::system_clock::now() + std::chrono::milliseconds(pref->GetInt("shooter.timeout", 500));
	}
}

void Shooter::intake(bool on) {
	move(0);
	intakeState = on;
}

void Shooter::enable(bool on) {
	shootState = on;
}
