// diaryIO.cpp: Responsible for saving/loading/encrypting/decrypting diary entries with JSON
// Written by: Letoonik
// Last edit: 12.18.2024
#include <fstream>
#include <string>
#include "nlohmann/json.hpp"
#include <botan/sodium.h>

#include <botan/hex.h>
#include <botan/hash.h>
#include <botan/mac.h>
#include <botan/aead.h>
#include <botan/rng.h>
#include <botan/kdf.h>
#include <botan/auto_rng.h>
#include <botan/mem_ops.h>
#include "diaryIO.hpp"

using json = nlohmann::json;

extern bool isEncrypted;
std::string password;
//const char* pwd = password.data();

namespace WDDio 
{
	std::string encryptEntryFile(std::string entryToEncrypt) 
	{
		//std::vector<uint8_t> entryVector(entryToEncrypt.data(), entryToEncrypt.data() + entryToEncrypt.length());
		//auto botanHash = Botan::HashFunction::create_or_throw("Blake2b(256)");
		//botanHash->update(entryToEncrypt);
		//Botan::SecureVector<uint8_t> hash = botanHash->final();

		//std::string a = Botan::hex_encode(hash);
		//auto botanEncrypt = Botan::AEAD_Mode::create_or_throw("ChaCha20Poly1305", Botan::Cipher_Dir::Encryption);
		//Botan::AutoSeeded_RNG rng;
		//Botan::secure_vector<uint8_t> nonce = rng.random_vec(botanEncrypt->default_nonce_length());
		//botanEncrypt->start(nonce.bits_of())
		////botanEncrypt->start(nonce, botanEncrypt->default_nonce_length());
		//InitializationVector iv(rng, 12);
		////botanEncrypt->set_key();

		return entryToEncrypt;
	}

	std::string decryptEntryFile(std::string entryToDecrypt) 
	{
		return entryToDecrypt;
	}

	int saveDiaryEntry(std::string textData, std::string entryTitle)
	{
		// read and parse JSON present in file
		std::ifstream jsonFile;
		jsonFile.open("main.json");
		std::string f;
		std::getline(jsonFile, f);
		json data = json::parse(f);

		//get time & date
		time_t result = time(NULL);
		char date[26];
		ctime_s(date, sizeof(date), &result);

		//append entry to "entries" array
		data["entries"] +=
		{ 
			{
				"title", entryTitle
			},
			{
				"text", textData
			},
			{
				"date", date
			}
		};

		if (isEncrypted)
		{
			encryptEntryFile(data.dump());
		}

		std::ofstream out("main.json");
		out << data;

		out.close();
		jsonFile.close();

		return 0;
	}

	json loadDiaryEntries()
	{
		json data;
		std::ifstream jsonFile;
		std::string vec;

		jsonFile.open("main.json");

		if (jsonFile.is_open())
		{
			jsonFile >> data;
			vec = data["entries"][0]["date"];
			jsonFile.close();
		}
		else
		{
			return "Failed to load/open JSON file";
		}
		return data;
	}

	std::string loadEntry(int entriesIndex, json jsonData) {
		return jsonData["entries"][entriesIndex]["text"];
	}
}