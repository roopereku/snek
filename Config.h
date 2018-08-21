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

	float fromSingle(std::string index);
	float fromMulti(std::string index, size_t i);

private:
	void parseArguments(std::vector <std::string> args);
	void parseFile(const char* path);

	std::map <std::string, float> configDataSingle;
	std::map <std::string, std::vector <float>> configDataMultiple;
};

#endif
