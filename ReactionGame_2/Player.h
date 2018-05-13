#pragma once
#include <iostream>
#include <sstream>


class Player {
public:
	Player();
	Player(std::string name, int score);
	void set_name(std::string name);
	std::string const get_name();
	void increase_score();
	void reset_score();
	int const get_score();

private:
	std::string name_;
	int score_;
};

