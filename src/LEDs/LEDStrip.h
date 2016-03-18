#ifndef SRC_LEDSTRIP_H_
#define SRC_LEDSTRIP_H_

#include <vector>
#include <algorithm>
#include "WPILib.h"
#include "LEDs/Color.h"
#include "LEDs/LEDController.h"

class LEDStrip {
	private:
		uint8_t start_idx;
		uint8_t count;
		bool reverse;
		std::vector<Util::Color> buffer;
		std::vector<Util::Color> shownBuffer;
	public:
		LEDStrip(int idx, int count, bool reverse = false);
		void Set(Util::Color led, int idx, bool show = false);
		void Set(std::vector<Util::Color> values, int idx = 0, bool show = false);
		Util::Color Get(int idx);
		void Clear();
		void Show();
		void Rotate();
};

#endif /* SRC_LEDSTRIP_H_ */
