#pragma once
#include <string>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

namespace WDDio
{
	std::string encryptEntryFile(std::string entryToEncrypt);
	std::string decryptEntryFile(std::string entryToDecrypt);
	json loadDiaryEntries();
	std::string loadEntry(int entriesIndex, json jsonData);
	int saveDiaryEntry(std::string textData, std::string entryTitle);
}