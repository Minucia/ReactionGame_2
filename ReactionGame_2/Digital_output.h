#pragma once

class Digital_output {
public:
	bool state_;

	Digital_output(int pin, bool state);
	void turn_on();
	void turn_off();
	void turn_on_3_sec();
	void blink_5_sec();
	bool const get_state();

private:
	int pin_;
};
#pragma once
