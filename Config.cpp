#include "Config.h"

#include <string>
#include <sstream>
#include <fstream>

template <typename T>
T strTo(std::string str)
{
	T value;
	std::stringstream(str) >> value;
	return value;
}

std::string loadFile(const char* path)
{
	std::string data;

	std::ifstream fileI(path);
	if(fileI.is_open())
	{
		std::string line;
		while(std::getline(fileI, line))
			data+=line+' ';
	}

	else
		SDL_Log("Unable to open %s!", path);
}

Config::Config(int argc, char** argv)
{
	configDataMultiple["-sensitivity"];
	configDataSingle["-players"] = 1;
	configDataSingle["-maxapples"] = 1;

	if(argc <= 1)
	{
		SDL_Log("No arguments passed!");
	}

	else if(argc >= 2)
	{
		SDL_Log("%d arguments passed!", argc - 1);
		parseArguments(argc, argv);
	}
}

void Config::parseArguments(int argc, char** argv)
{
	for(size_t i = 1; i < (size_t)argc; i++)
	{
		if(configDataSingle.find(argv[i]) != configDataSingle.end())
		{
			std::string key = argv[i];
			i++;

			float currentValue = 0.0f;


			configDataSingle[key] = strTo<float>(argv[i]); 

			float value = configDataSingle[ argv[i - 1] ];
			SDL_Log("%s set to %.2f!", key.c_str(), value);
		}

		else if(configDataMultiple.find(argv[i]) != configDataMultiple.end())
		{
			std::string key = argv[i];
			i++;

			size_t playerIndex = strTo<size_t>(argv[i]);
			i++;
			float value = strTo<float>(argv[i]);

			if(playerIndex >= configDataMultiple[key].size())
			{
				SDL_Log("Pushed %.2f to %s at %d!", value, key.c_str(), (int)playerIndex);
				configDataMultiple[key].push_back(value);
			}

			else
			{
				SDL_Log("Modified index %d of %s!", (int)playerIndex - 1, key.c_str());
				configDataMultiple[key][playerIndex - 1] = value;
			}
		}

		else
		{
			SDL_Log("'%s' is not a valid argument!", argv[i]);
		}
	}
}

int Config::getPlayers()
{
	return configDataSingle["-players"];
}

float Config::getSensitivity(size_t index)
{
	return configDataMultiple["-sensitivity"][index];
}
