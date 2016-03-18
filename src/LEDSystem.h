#ifndef SRC_LEDS_H_
#define SRC_LEDS_H_

#include <atomic>
#include "LEDs/LEDStrip.h"
#include "LEDs/Color.h"
#include "Util.h"
#include <cmath>
#include <thread>
#include <future>

class LEDSystem {
	public:
		static LEDSystem* GetInstance();
		LEDSystem();

	private:
		std::future<void> task;
		void Run();
};


#endif /* SRC_LEDS_H_ */
