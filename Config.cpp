#include "Config.h"

#include <sstream>
#include <fstream>
#include <vector>

template <typename T>
T strTo(std::string str)
{
	T value;
	std::stringstream(str) >> value;
	return value;
}

void Config::parseFile(const char* path)
{
	std::vector <std::string> data(1);

	std::ifstream fileI(path);
	if(fileI.is_open())
	{
		std::string line;
		std::string arg;

		while(std::getline(fileI, line))
		{
			line+=' ';
			for(size_t i = 0; i < line.length(); i++)
			{
				if(line[i] != ' ')
				{
					arg+=line[i];
					SDL_Log("%c", line[i]);
				}
				else
				{
					data.push_back(arg);
					arg = "";
				}
			}
			data.push_back(arg);
		}

		for(size_t i = 0; i < data.size(); i++)
			SDL_Log("Arg : %s", data[i].c_str());

		parseArguments(data);
	}

	else
		SDL_Log("Unable to open %s!", path);
}

Config::Config(int argc, char** argv)
{
	configDataMultiple["-sensitivity"].resize(4, 2.5f);

	configDataSingle["-appledelay"] = 20.0f;
	configDataSingle["-players"] = 1;
	configDataSingle["-maxapples"] = 1;

	if(argc <= 1)
	{
		const char* defaultPath = "conf";
		SDL_Log("No arguments passed! Loading from %s", defaultPath);
		parseFile(defaultPath);
	}

	else if(argc >= 2)
	{
		SDL_Log("%d arguments passed!", argc - 1);

		std::vector <std::string> args(argc);
		for(size_t i = 0; i < args.size(); i++)
			args[i] = argv[i];

		parseArguments(args);
	}
}

void Config::parseArguments(std::vector <std::string> args)
{
	SDL_Log("%d arguments found!", (int)args.size());

	for(size_t i = 1; i < args.size(); i++)
	{
		if(configDataSingle.find(args[i]) != configDataSingle.end())
		{
			std::string key = args[i];
			i++;

			configDataSingle[key] = strTo<float>(args[i]); 

			float value = configDataSingle[ args[i - 1] ];
			SDL_Log("%s set to %.2f!", key.c_str(), value);
		}

		else if(configDataMultiple.find(args[i]) != configDataMultiple.end())
		{
			std::string key = args[i];
			i++;

			size_t playerIndex = strTo<size_t>(args[i]);
			i++;
			float value = strTo<float>(args[i]);

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
			
			SDL_Log("'%s' is not a valid argument!", args[i].c_str());
		}
	}
}

float Config::fromSingle(std::string index)
{
	return configDataSingle[index];
}

float Config::fromMulti(std::string index, size_t i)
{
	return configDataMultiple[index][i];
}
