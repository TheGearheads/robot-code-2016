#include "LEDs/LEDStrip.h"

LEDStrip::LEDStrip(int idx, int count, bool reverse /*= false*/) :
	start_idx(idx), count(count), reverse(reverse) {
		buffer.resize(count, 0x000000);
		shownBuffer.resize(count, 0x000000);
	}

void LEDStrip::Set(Util::Color led, int idx, bool show /*= false*/) {
	if (idx < (int)buffer.size() && led != buffer[idx]) {
		buffer[idx] = led;
	}
	if (show) {
		Show();
	}
}

void LEDStrip::Set(std::vector<Util::Color> values, int idx /*= 0*/, bool show /*= false*/) {
	for (int i = 0; i < (int)values.size(); i++) {
		Set(values[i], i + idx);
	}
	if (show) {
		Show();
	}
}

Util::Color LEDStrip::Get(int idx) {
	return LEDController::GetInstance()->Get(start_idx + idx);
}

void LEDStrip::Clear() {
	std::vector<Util::Color> tmp;
	tmp.resize(count);
	Set(tmp, start_idx, true);
}

void LEDStrip::Show() {
	bool dirty = false;
	for (int i = 0; i < (int)buffer.size(); i++) {
		dirty = dirty || (buffer[i] != shownBuffer[i]);
	}
	if (dirty) {
		shownBuffer = buffer;
		std::vector<Util::Color> outBuffer = shownBuffer;
		if (reverse) {
			std::reverse(outBuffer.begin(), outBuffer.end());
		}
		LEDController::GetInstance()->Set(outBuffer, start_idx);
		LEDController::GetInstance()->Show();
	}
}

void LEDStrip::Rotate() {
	std::rotate(buffer.begin(), buffer.begin() + 1, buffer.end());
}
