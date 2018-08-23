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

Config::Config(int argc, char** argv)
{
	// All valid arguments are declared here.

	configDataMultiple["-sensitivity"].resize(4, 2.5f);
	configDataMultiple["-direction"].resize(4, 0.0f);

	configDataSingle["-appledelay"] = 20.0f;
	configDataSingle["-players"] = 1;
	configDataSingle["-maxapples"] = 1;
	configDataSingle["-acceleration"] = 0.25f;
	configDataSingle["-initialspeed"] = 1.0f;

	// Parse the default config file.

	const char* defaultPath = "conf";
	parseFile(defaultPath);
	

	// Parse command-line arguments.

	if(argc > 1)
	{
		// If there are only 2 arguments and the second one is -help, print out the valid arguments
		if( argc == 2 && std::string(argv[1]) == "-help")
		{
			std::string helpMsg = "\n\nAvailable arguments:\n";

			for(auto& it : configDataSingle)
				helpMsg += "	" + it.first + "	value\n";

			helpMsg += "\n";

			for(auto& it : configDataMultiple)
				helpMsg += "	" + it.first + "	index" + "	value\n";

			SDL_Log("%s\n\n", helpMsg.c_str());

			return;
		}

		SDL_Log("%d arguments passed!", argc - 1);

		// Argv is put into a vector to simplify things later on.
		std::vector <std::string> args(argc);
		for(size_t i = 0; i < args.size(); i++)
			args[i] = argv[i];

		parseArguments(args);
	}
}

void Config::parseFile(const char* path)
{
	std::vector <std::string> data;

	/*	Open the config file and put the arguments in data.
	 *	If the file couldn't be opened, notify the user.
	 */

	std::ifstream fileI(path);
	if(fileI.is_open())
	{
		std::string line;
		std::string arg;

		while(std::getline(fileI, line))
		{
			/*	Loop through line.
			 *	If a space or the end of the line occurs,
			 *	push arg in data.
			 */

			line+=' ';
			for(size_t i = 0; i < line.length(); i++)
			{
				if(line[i] != ' ')
				{
					arg+=line[i];
				}
				else
				{
					data.push_back(arg);
					arg = "";
				}
			}
			data.push_back(arg);
		}

		parseArguments(data);
	}

	else
		SDL_Log("Unable to open %s!", path);
}

void Config::parseArguments(std::vector <std::string> args)
{
	SDL_Log("%d arguments found!", (int)args.size() - 1);

	for(size_t i = 0; i < args.size(); i++)
	{
		/*	Currently the game can crash if given arguments are
		 *	for ex. -players -sensitivity 1
		 *	Because the config parser assumes that the syntax to be correct.
		 */

		// Check if argument is found in configDataSingle.

		if(configDataSingle.find(args[i]) != configDataSingle.end())
		{
			std::string key = args[i];
			i++;

			configDataSingle[key] = strTo<float>(args[i]); 

			float value = configDataSingle[ args[i - 1] ];
			SDL_Log("%s set to %.2f!", key.c_str(), value);
		}
		
		// Check if argument is found in configDataMultiple.

		else if(configDataMultiple.find(args[i]) != configDataMultiple.end())
		{
			std::string key = args[i];
			i++;

			size_t playerIndex = strTo<size_t>(args[i]);
			i++;
			float value = strTo<float>(args[i]);

			// If index x doesn't exist in key z, push a new element in z
			if(playerIndex >= configDataMultiple[key].size())
			{
				SDL_Log("Pushed %.2f to %s at %d!", value, key.c_str(), (int)playerIndex);
				configDataMultiple[key].push_back(value);
			}

			// If index x exists in key z, modify value at index x
			else
			{
				SDL_Log("Modified index %d of %s!", (int)playerIndex - 1, key.c_str());
				configDataMultiple[key][playerIndex - 1] = value;
			}
		}

		else
		{
			if(args[i].length() > 0)
				SDL_Log("'%s' is not a valid argument!", args[i].c_str());
		}
	}
}

/*	TODO
 *	add a check to see if the indices are in range
 *	of the configuration data.
 */

float Config::fromSingle(std::string index)
{
	return configDataSingle[index];
}

float Config::fromMulti(std::string index, size_t i)
{
	return configDataMultiple[index][i];
}
