#pragma once
#include <SFML\System\Clock.hpp>
#include <string>

using namespace std;

class TimerRecord
{
public:
	TimerRecord(string name);
	~TimerRecord();

	string Name;
	sf::Clock Clock;
};