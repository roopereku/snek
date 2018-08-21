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

	// Configuration data getters
	float fromSingle(std::string index);
	float fromMulti(std::string index, size_t i);

private:
	void parseArguments(std::vector <std::string> args);
	void parseFile(const char* path);

	/*	The configuration data is stored in 2 maps.
	 *
	 *	The first one contains things such as -maxapples and -players
	 *	which contain a single value.
	 *
	 *	The second one contains things such as -sensitivity which requires
	 *	2 values (ex. -sensitivity playerIndex value).
	 */

	std::map <std::string, float> configDataSingle;
	std::map <std::string, std::vector <float>> configDataMultiple;
};

#endif
