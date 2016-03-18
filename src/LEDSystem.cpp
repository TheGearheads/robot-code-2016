/*
 * LEDs.cpp
 */

#include "LEDSystem.h"
#include "Shooter.h"
#include "Intake.h"

/**
 * Task to handle the display of LEDs
 */
void LEDSystem::Run() {
	// This is kind of ugly :(
	auto ds = &DriverStation::GetInstance();
	//float hueOutput = 0;
	auto leds = LEDController::GetInstance();
	leds->SetBrightness(30);

	enum TimeLeftMode {kMoreThan30, k30Seconds, k20Seconds, k10Seconds};
	TimeLeftMode timeleftMode = kMoreThan30;

	struct Blink {
		int time;
		bool status;
	};
	std::vector<Blink> blinkPattern = {{500, true}, {250, false}, {-1, true}};
	const std::vector<Blink> blinkingOn = {{-1, true}};
	std::vector<Blink> blinking = blinkingOn;
	auto blinkingTime = std::chrono::system_clock::now();
	bool blinkStatus = true;

	LEDStrip sideLeft(0, 13);
	LEDStrip topLeft(13, 14);
	LEDStrip topRight(27, 14, true);
	LEDStrip sideRight(41, 13, true);
	std::vector<Util::Color> tmp;
	tmp.resize(14);
	tmp[0] = 0xffffff;
	tmp[1] = 0xffff00;
	topRight.Set(tmp);
	topLeft.Set(tmp);
	while (1) {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		if (ds->IsDisabled()) {
			tmp.clear(); tmp.resize(14, 0xffff00);
			if (!ds->IsDSAttached()) {
				// Not attached to the DS, so, alternate yellow/off
				for (int i = 0; i < 8; i++) {
					tmp[i * 2] = 0x000000;
				}
			}

			topLeft.Set(tmp);
			topRight.Set(tmp);
			tmp.resize(13);
			sideRight.Set(tmp);
			sideLeft.Set(tmp);

			auto pref = Preferences::GetInstance();
			float angleDiff = std::fabs(Shooter::GetInstance()->getAngle() - pref->GetFloat("shooter.auton.angle", 42.0f));
			if (angleDiff < pref->GetFloat("shooter.auton.threshold", 1.0f)) {
				tmp.clear();
				tmp.resize(14, 0x00ff00);
				topLeft.Set(tmp);
				topRight.Set(tmp);
			}

			tmp.clear(); tmp.resize(6);
			tmp[0] = ds->IsDSAttached() ? 0x00ff00 : 0x00;
			tmp[1] = ds->IsFMSAttached() ? 0x00ff00 : 0x00;
			float hueOutput = ghLib::Interpolate(ds->GetBatteryVoltage(), 12.0f, 13.0f, 0.0f, 120.0f);
			tmp[2] = Util::Color(hueOutput, 1.0, 0.75);

			auto position = ds->GetLocation();
			auto alliance = ds->GetAlliance();
			Util::Color allianceColor = alliance == DriverStation::kRed ? 0xff0000 : (alliance == DriverStation::kBlue ? 0x0000ff : 0xffff00);
			if (position) {
				tmp[2 + position] = allianceColor;
			}
			sideLeft.Set(tmp);

		} else {
			if (ds->IsAutonomous()) {
				auto alliance = ds->GetAlliance();
				Util::Color allianceColor = alliance == DriverStation::kRed ? 0xff0000 : (alliance == DriverStation::kBlue ? 0x0000ff : 0xffff00);
				tmp.clear();
				tmp.resize(14, allianceColor);
				topLeft.Set(tmp);
				topRight.Set(tmp);
				tmp.resize(13);
				sideLeft.Set(tmp);
				sideRight.Set(tmp);
			} else if (ds->IsOperatorControl()) {
				double timeLeft = ds->GetMatchTime();
				tmp.clear();
				tmp.resize(16, 0xffff00);
				Util::Color timeleftColor = 0xffff00;
				if (timeLeft < 0 || timeLeft > 30) {
					// All yellow
					blinking = blinkingOn;
					timeleftMode = kMoreThan30;
				} else if (timeLeft > 20) { // 30 seconds left
					if (timeleftMode == kMoreThan30) {
						blinking = blinkPattern;
						timeleftMode = k30Seconds;
					}
					timeleftColor = 0x00ff00;
				} else if (timeLeft > 10) { // 20 seconds left
					if (timeleftMode == k30Seconds) {
						blinking = blinkPattern;
						timeleftMode = k20Seconds;
					}
					timeleftColor = 0xff9900;
				} else {                    // 10 seconds left
					if (timeleftMode == k20Seconds) {
						blinking = blinkPattern;
						timeleftMode = k10Seconds;
					}
					timeleftColor = 0xff0000;
				}

				auto now = std::chrono::system_clock::now();
				if (blinkingTime < now) {
					blinkStatus = blinking[0].status;
					if (blinking[0].time != -1) {
						blinkingTime = now + std::chrono::milliseconds(blinking[0].time);
						//printf("Match Time: %f (%x for %dms (%s))\n", ds->GetMatchTime(), (int)timeleftColor, blinking[0].time, blinkStatus ? "on" : "off");
						blinking.erase(blinking.begin());
					}
				}

				if (blinkStatus == false) {
					timeleftColor = 0x000000;
				}

				tmp.clear();
				tmp.resize(6, timeleftColor);
				sideLeft.Set(tmp);
				sideRight.Set(tmp);
				topLeft.Set(tmp, 8);
				topRight.Set(tmp, 8);
			}

			tmp.clear();
			auto shooter = Shooter::GetInstance();
			if (shooter->isIntaking()) {
				tmp.resize(8, 0xff0000);
			} else if (shooter->isEnabled()) {
				tmp.resize(8, 0x00ff00);
			} else {
				tmp.resize(8, 0xffff00);
			}
			if (shooter->isFiring()) {
				tmp[0] = tmp[7] = 0xff00ff;
			}
			topLeft.Set(tmp);
			topRight.Set(tmp);

			tmp.clear();
			auto intake = Intake::GetInstance();
			if (intake->isDown()) {
				tmp.resize(7, 0x00ff00);
			} else {
				tmp.resize(7, 0xffff00);
			}

			sideLeft.Set(tmp, 7);
			sideRight.Set(tmp, 7);
		}
		topLeft.Show();
		sideLeft.Show();
		topRight.Show();
		sideRight.Show();
		LEDController::GetInstance()->Show();
	}
}

LEDSystem* LEDSystem::GetInstance() {
	static LEDSystem instance;
	return &instance;
}

LEDSystem::LEDSystem() {
	task = std::async(std::launch::async, &LEDSystem::Run, this);
}

