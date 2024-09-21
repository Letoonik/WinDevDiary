#include <fstream>
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;


namespace WDDsave 
{
	int saveDiaryEntry(std::string textData)
	{
		json data;
		std::ofstream jsonFile;
		
		data["diaryEntryText"] = textData;

		jsonFile.open("example.json");

		if(jsonFile.is_open()) 
		{
			jsonFile << data;
		}
		else 
		{
			return 1;
		}

		jsonFile.close();

		return 0;
	}

	std::string loadDiaryEntry()
	{
		json data;
		std::ifstream jsonFile;
		std::string vec;

		jsonFile.open("example.json");

		if (jsonFile.is_open())
		{
			jsonFile >> data;
		}
		else
		{
			int a = 0; // placeholder, make better later
		}

		vec = data["diaryEntryText"];

		jsonFile.close();

		return vec;
	}
}