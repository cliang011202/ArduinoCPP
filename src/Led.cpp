#include "Led.hpp"

Led::Led(uint8_t pin)
	:pin_(pin)
	,is_on_(false)
{
	pinMode(pin_, OUTPUT);
	off();
}

void Led::on()
{
	digitalWrite(pin_, HIGH);
	is_on_ = true;
}

void Led::off()
{
	digitalWrite(pin_, LOW);
	is_on_ = false;
}

void Led::toggle()
{
	if (is_on_){
		off();
	} else {
		on();
	}
}

bool Led::isOn() const
{
	return is_on_;
}


