#include "Shooter.h"
#include "Util.h"

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

void Shooter::moveTo(float pos) {
	auto pref = Preferences::GetInstance();
	if (pivot->GetControlMode() != CANTalon::ControlMode::kPosition) {
		pivot->SetControlMode(CANTalon::ControlMode::kPosition);
		pivot->SetP(pref->GetDouble("shooter.pivot.p", 0));
		pivot->SetI(pref->GetDouble("shooter.pivot.i", 0));
		pivot->SetIzone(pref->GetInt("shooter.pivot.izone", 20));
		pivot->SetAllowableClosedLoopErr(pref->GetInt("shooter.pivot.allowableError", 2));
	}
	pivot->Set((float)angleToPot((double)pos));
}


void Shooter::periodic() {
	auto pref = Preferences::GetInstance();
	auto table = NetworkTable::GetTable("Shooter");

	if(std::chrono::system_clock::now()>shooterTimeout)
		piston->Set(DoubleSolenoid::kReverse);

	if (RobotState::IsOperatorControl()){
		float stickIn = -ghLib::Deadband(stick->GetY(), pref->GetFloat("shooter.pivot.deadband", 0.05));
		table->PutNumber("stick.in", stickIn);
		table->PutNumber("stick.raw", stick->GetY());
		table->PutNumber("setpoint", pivot->GetSetpoint());
		table->PutNumber("pivot/error", pivot->GetClosedLoopError());
		if (stickIn != 0.0f) {
			pivot->SetControlMode(CANTalon::ControlMode::kPercentVbus);
			move(stickIn);
		} else {
			if (pivot->GetControlMode() == CANTalon::ControlMode::kPercentVbus) {
				move(0);
			}
		}
		if (stick->GetRawButton(7)) {
			moveTo(0);
		}
		if (stick->GetRawButton(6)) {
			moveTo(40);
		}
		if (!shootButtonPrev && stick->GetRawButton(2) ){
			enable(!shootState);
		}
		shootButtonPrev = stick->GetRawButton(2);
		if (!fireButtonPrev && stick->GetRawButton(1) && shootState){
			fire();
		}
		fireButtonPrev = stick->GetRawButton(1);
	}

	if (intakeState){
		setSpeed(pref->GetFloat("shooter.intake.speed", -0.4f));
		shootState = false;
	}else if (shootState){
		setSpeed(pref->GetFloat("shooter.speed", 1.0f));
	}else {
		setSpeed(0);
	}


	table->PutNumber("pivot/pot", pivot->GetPosition());
	table->PutNumber("angle", getAngle());
	table->PutBoolean("state", shootState);
	table->PutBoolean("fire", piston->Get() == DoubleSolenoid::kForward);
	table->PutNumber("pivot/bus", pivot->GetBusVoltage());
	table->PutNumber("pivot/out", pivot->GetOutputVoltage());
	table->PutNumber("pivot/current", pivot->GetOutputCurrent());
}

double Shooter::potToAngle(double pot) {
	auto pref = Preferences::GetInstance();
	double pot_min = pref->GetDouble("shooter.pivot.pot.min", 500);
	double pot_max = pref->GetDouble("shooter.pivot.pot.max", 500);
	double angle_min = pref->GetDouble("shooter.pivot.angle.min", -20);
	double angle_max = pref->GetDouble("shooter.pivot.angle.max", 50);
	return (pot - pot_min) * (angle_max - angle_min) / (pot_max - pot_min) + angle_min;
}

double Shooter::angleToPot(double angle) {
	auto pref = Preferences::GetInstance();
	double pot_min = pref->GetDouble("shooter.pivot.pot.min", 500);
	double pot_max = pref->GetDouble("shooter.pivot.pot.max", 500);
	double angle_min = pref->GetDouble("shooter.pivot.angle.min", -20);
	double angle_max = pref->GetDouble("shooter.pivot.angle.max", 50);
	return (angle - angle_min) * (pot_max - pot_min) / (angle_max - angle_min) + pot_min;
}

double Shooter::getAngle() {
	return potToAngle(pivot->GetPosition());
}

void Shooter::fire() {
	auto pref = Preferences::GetInstance();
	if (!intakeState){
		piston->Set(DoubleSolenoid::kForward);
		shooterTimeout=std::chrono::system_clock::now() + std::chrono::milliseconds(pref->GetInt("shooter.timeout", 500));
	}
}

void Shooter::intake(bool on) {
	if (intakeState && !on) {
		moveTo(prevPos);
	} else if (!intakeState && on) {
		prevPos = potToAngle(pivot->GetPosition());
		auto pref = Preferences::GetInstance();
		moveTo(pref->GetFloat("shooter.intake.pos", -15.0f));
	}
	intakeState = on;
}

void Shooter::enable(bool on) {
	shootState = on;
}
