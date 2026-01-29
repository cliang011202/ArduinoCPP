#pragma once
#include <arduino.h>

class Led {
public:
	explicit Led(uint8_t pin);

	void on();
	void off();
	void toggle();

  bool isOn() const;

private:
	const uint8_t pin_;
	bool is_on_;
};