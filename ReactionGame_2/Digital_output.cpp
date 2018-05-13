
#include <wiringPi.h>
#include <chrono>
#include <thread>
#include <iostream>

#include "Digital_output.h"


Digital_output::Digital_output(int pin, bool state) : pin_{ pin }, state_{ state }
{
	wiringPiSetup();
	pinMode(pin_, OUTPUT);
	digitalWrite(pin_, state_);
}

void Digital_output::turn_on()
{
	digitalWrite(pin_, HIGH);
	state_ = HIGH;
}

void Digital_output::turn_off()
{
	digitalWrite(pin_, LOW);
	state_ = LOW;
}

void Digital_output::turn_on_3_sec()
{
		digitalWrite(pin_, HIGH);
		std::this_thread::sleep_for(std::chrono::milliseconds(3000));
		digitalWrite(pin_, LOW);
}

void Digital_output::blink_5_sec()
{
	for (int i = 0; i < 5; i++)
	{
		digitalWrite(pin_, HIGH);
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		digitalWrite(pin_, LOW);
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}

bool const Digital_output::get_state()
{
	return state_; //digitalRead(pin_);
}
