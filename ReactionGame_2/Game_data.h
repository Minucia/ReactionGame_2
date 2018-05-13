#pragma once


// ____________________________ Enumerations ____________________________

enum class States { preperation, reaction_led, result, finish };


// ____________________________ Global variables - comment out ____________________________

static constexpr auto led1_pin = 8;
static constexpr auto led2_pin = 9;
static constexpr auto led_reaction_pin = 7;
static constexpr auto button1_pin = 0;
static constexpr auto button2_pin = 2;
static constexpr auto minimum_wait = 5;
static constexpr auto maximum_wait = 10;
static constexpr int64_t termination_time = 3;

States states = States::preperation;

bool loop = true;
bool button1_flag = false;
bool button2_flag = false;

int played_rounds;

int64_t desired_delay;
int64_t delay_start;
int64_t actual_delay;



/*
//class Game_data {
public:
Game_data();
void set_state(States states);
void set_loop(bool loop);
void set_button1_flag(); //true
void reset_button1_flag(); //false
void set_button2_flag(); //true
void reset_button2_flag(); //false

void reset_played_rounds();
void increase_played_rounds();
void get_played_rounds();

void set_desired_delay();
void get_desired_delay();
void set_delay_start();
void get_delay_start();
void set_actual_delay();
void get_actual_delay();


private:
static constexpr auto led1_pin = 8;
static constexpr auto led2_pin = 9;
static constexpr auto led_reaction_pin = 7;
static constexpr auto button1_pin = 0;
static constexpr auto button2_pin = 2;
static constexpr auto minimum_wait = 5;
static constexpr auto maximum_wait = 10;
static constexpr int64_t termination_time = 3;

States states_;

bool loop_;
bool button1_flag_;
bool button2_flag_;

int played_rounds_;

int64_t desired_delay_;
int64_t delay_start_;
int64_t actual_delay_;
};
*/

/*
into game setup:

Game_data game;
game.set_state(States::preperation);
game.set_loop(true);
game.reset_button1_flag(); //false
game.reset_button2_flag(); //false
*/