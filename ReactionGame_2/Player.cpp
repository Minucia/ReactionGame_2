
#include "Player.h"

using namespace std;


Player::Player() : name_{ "MusterMaxi" }, score_{ 0 }
{
}

Player::Player(string name, int score) : name_{ name }, score_{ score }
{
}

void Player::set_name(string name)
{
	name_ = name;
}

string const Player::get_name()
{
	return name_;
}

void Player::increase_score()
{
	score_++;
}

void Player::reset_score()
{
	score_ = 0;
}

int const Player::get_score()
{
	return score_;
}

