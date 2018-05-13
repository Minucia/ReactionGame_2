

// ____________________________ Includes ____________________________

#include <wiringPi.h>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <fstream>
//#include "json.hpp"

#include "Player.h"
#include "Digital_output.h"
#include "Game_data.h"



// ____________________________ Function Prototypes ____________________________

void button_setup();
void button1_pressed();
void button2_pressed();
void both_leds_blink(int pin1, int pin2);


// ____________________________ Global namespace ____________________________

using namespace std;


// ____________________________ Main Function ____________________________

int main()
{
	// --------------------- Implementing Objects ---------------------

	Player player1;
	Player player2;

	Digital_output led_player1{ led1_pin, LOW  };
	Digital_output led_player2{ led2_pin, LOW };
	Digital_output led_reaction{ led_reaction_pin, LOW };


	// --------------------- Game Setup --------------------- 

	button_setup();

	played_rounds = 0;
	player1.reset_score();
	player2.reset_score();


	// --------------------- User Interface --------------------- 

	cout << "Welcome to reaction game!\n" << endl;

	cout << "Player 1, please enter your name." << endl;
	string input;
	getline(cin, input);
	player1.set_name(input);
	cout << "Hello " << player1.get_name() << ". Your button is at pin 0. Your status LED is at pin 8.\n" << endl;

	cout << "Player 2, please enter your name." << endl;
	getline(cin, input);
	player2.set_name(input);
	cout << "Hello " << player2.get_name() << ". Your button is at pin 2. Your status_LED is at pin 9.\n" << endl;

	cout << "How many rounds would you like to play?" << endl;
	int rounds_to_play;
	getline(cin, input);
	stringstream in_str{ input };
	in_str >> rounds_to_play;
	cout << "You are going to play " << rounds_to_play << " rounds. Let's start!" << endl;


	// --------------------- Statemachine ---------------------

	while (loop)
	{
		switch (states)
		{
		case States::preperation:
		{
			played_rounds++;
			if (played_rounds > rounds_to_play)
				states = States::finish;
			else
			{
				led_player1.turn_off();
				led_player2.turn_off();
				led_reaction.turn_off();

				cout << "Current score: " << player1.get_name() << " [" << player1.get_score() << " : "
					<< player2.get_score() << "] " << player2.get_name() << endl;

				std::this_thread::sleep_for(std::chrono::seconds(2));
				cout << "Get ready!" << endl;
				std::this_thread::sleep_for(std::chrono::seconds(1));
				cout << "Go!" << endl;

				desired_delay = rand() % (maximum_wait + 1 - minimum_wait) + minimum_wait; //setting a random delay between 5 and 10 seconds
				using s = std::chrono::seconds;
				delay_start = std::chrono::duration_cast<s>(std::chrono::system_clock::now().time_since_epoch()).count();
				
				states = States::reaction_led;
			}
		}
		break;

		case States::reaction_led:
		{
			using s = std::chrono::seconds;
			actual_delay = std::chrono::duration_cast<s>(std::chrono::system_clock::now().time_since_epoch()).count() - delay_start;
			
			if (actual_delay >= desired_delay) //waiting for random 5 to 10 seconds
				led_reaction.turn_on();

			if (actual_delay-desired_delay >= termination_time) //if no one presses a button for 3 seconds, the game terminates
				states = States::finish;
		}
		break;

		case States::result:
		{
			if (button1_flag)
			{
				if (led_reaction.get_state() == HIGH)
				{
					led_reaction.turn_off();
					player1.increase_score();
					cout << player1.get_name() << " gets a point." << endl;
					led_player1.turn_on_3_sec();
					states = States::preperation;
				}
				else if (led_reaction.get_state() == LOW)
				{
					player2.increase_score();
					cout << "Not so fast " << player1.get_name() << ". " << player2.get_name() << " gets a point." << endl;
					led_player2.turn_on_3_sec();
					states = States::preperation;
				}

				button1_flag = false;
			}
			else if (button2_flag)
			{
				if (led_reaction.get_state() == HIGH)
				{
					led_reaction.turn_off();
					player2.increase_score();
					cout << player2.get_name() << " gets a point." << endl;
					led_player2.turn_on_3_sec();
					states = States::preperation;
				}
				else if (led_reaction.get_state() == LOW)
				{
					player1.increase_score();
					cout << "Not so fast " << player2.get_name() << ". " << player1.get_name() << " gets a point." << endl;
					led_player1.turn_on_3_sec();
					states = States::preperation;
				}
				button2_flag = false;
			}
		}
		break;

		case States::finish:
		{
			if (played_rounds <= rounds_to_play)
			{
				led_reaction.turn_off();
				cout << "The game terminated due to inactivity." << endl;
			}
			else
			{
				cout << "Current score: " << player1.get_name() << " [" << player1.get_score() << " : "
					<< player2.get_score() << "] " << player2.get_name() << endl;
				if (player1.get_score() > player2.get_score())
				{
					cout << player1.get_name() << " wins!" << endl;
					led_player1.blink_5_sec();
				}
				else if (player2.get_score() > player1.get_score())
				{
					cout << player2.get_name() << " wins!" << endl;
					led_player2.blink_5_sec();
				}
				else
				{
					cout << "It's a tie!" << endl;
					both_leds_blink(led1_pin, led2_pin);
				}
			}
			
			loop = false;
		}
		break;

		default:
		{
			cout << "That wasn't supposed to happen." << endl;
		}
		break;
		}
	}
	return 0;
}

// --------------------- Functions ---------------------

void button_setup()
{
	wiringPiSetup();
	pinMode(button1_pin, INPUT);
	pinMode(button2_pin, INPUT);

	pullUpDnControl(button1_pin, PUD_DOWN); //pull to ground
	pullUpDnControl(button2_pin, PUD_DOWN); //pull to ground

	wiringPiISR(button1_pin, INT_EDGE_RISING, &button1_pressed); //interrupt
	wiringPiISR(button2_pin, INT_EDGE_RISING, &button2_pressed); //interrupt
}

void button1_pressed()
{
	button1_flag = true;
	states = States::result;
}

void button2_pressed()
{
	button2_flag = true;
	states = States::result;
}

void both_leds_blink(int pin1, int pin2) //both player_leds blink for 5 seconds
{
	for (int i = 0; i < 5; i++)
	{
		digitalWrite(pin1, HIGH);
		digitalWrite(pin2, HIGH);
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		digitalWrite(pin1, LOW);
		digitalWrite(pin2, LOW);
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}