// diaryIO.cpp: Responsible for saving/loading/encrypting/decrypting diary entries with JSON
#include <fstream>
#include <string>
#include "nlohmann/json.hpp"
#include <botan/sodium.h>

#include <botan/hex.h>
#include <botan/aead.h>
#include <botan/rng.h>
#include <botan/sodium.h>
#include <botan/auto_rng.h>
#include <botan/mem_ops.h>

using json = nlohmann::json;

extern bool isEncrypted;
std::string password;
const char* pwd = password.data();

namespace WDDsave 
{
	std::string encryptEntryFile(std::string entryToEncrypt) 
	{
		auto botanEncrypt = Botan::AEAD_Mode::create_or_throw("XChaCha20Poly1305", Botan::Cipher_Dir::Encryption);
		Botan::AutoSeeded_RNG rng;
		Botan::secure_vector<uint8_t> nonce = rng.random_vec(botanEncrypt->default_nonce_length());
		

		return entryToEncrypt;
	}

	std::string decryptEntryFile(std::string entryToDecrypt) 
	{
		return entryToDecrypt;
	}

	int saveDiaryEntry(std::string textData)
	{
		json data;
		std::ofstream jsonFile;

		data["diaryEntryText"] = textData;

		jsonFile.open("example.json");

		//if (isEncrypted)
		//{
		//	encryptEntryFile(data, "HI");
		//	return 0;
		//}

		if (jsonFile.is_open())
		{
			jsonFile << data;
			jsonFile << password;
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

		if(isEncrypted)
		{
			//return decryptEntry();
			return "TEMP";
		}
		else 
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
				return "Failed to load/open JSON file";
			}

			vec = data["diaryEntryText"];

			jsonFile.close();

			return vec;
		}
	}
}