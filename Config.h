#ifndef CONFIG_H
#define CONFIG_H

#include <SDL2/SDL.h>

#include <map>
#include <vector>
#include <string>

class Config
{
public:
	Config(int argc, char** argv);

	int getPlayers();
	float getSensitivity(size_t index);

private:
	void parseArguments(int argc, char** argv);

	std::map <std::string, float> configDataSingle;
	std::map <std::string, std::vector <float>> configDataMultiple;
};

#endif
