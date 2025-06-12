#pragma once
#include <string>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

namespace WDDio
{
	std::string encryptEntryFile(std::string entryToEncrypt);
	std::string decryptEntryFile(std::string entryToDecrypt);
	std::string loadEntry(int entriesIndex, json jsonData);
	json loadDiaryEntries();
	int saveDiaryEntry(std::string textData, std::string entryTitle);
}