// diaryIO.cpp: Responsible for saving/loading/encrypting/decrypting diary entries with JSON
#include <fstream>
#include <string>
#include "sodium.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

extern bool isEncrypted;
std::string password = "Hase12774#3??23";
const char* pwd = password.data();

namespace WDDsave 
{
	void encryptEntry(std::string textData) 
	{
		// INIT DATA
		json data;
		std::ofstream encFile;
		encFile.open("example.enc", std::ofstream::binary | std::ofstream::app);
		data["diaryEntryText"] = textData;

		unsigned char key[crypto_box_SEEDBYTES];
		unsigned char salt[crypto_pwhash_SALTBYTES];
		unsigned char header[crypto_secretstream_xchacha20poly1305_HEADERBYTES];

		randombytes_buf(salt, sizeof(salt));


		if(std::filesystem::exists("example.enc"))
		{
			encFile.close();
			if (remove("example.enc"))
			{
				int g = 0;
			}
			else 
			{
				encFile.open("example.enc", std::ofstream::binary | std::ofstream::app);
			}
		}

		bool tempTrue = true;
		if (tempTrue) {
			if (crypto_pwhash(key, sizeof(key), pwd, strlen(pwd), salt, crypto_pwhash_OPSLIMIT_INTERACTIVE, crypto_pwhash_MEMLIMIT_INTERACTIVE, crypto_pwhash_ALG_DEFAULT) != 0)
				int sdf = 0; //Placeholder, fix later
			else
			{

				crypto_secretstream_xchacha20poly1305_state state;
				crypto_secretstream_xchacha20poly1305_init_push(&state, header, key);

				encFile.write((const char*)header, crypto_secretstream_xchacha20poly1305_HEADERBYTES);
				encFile.write((const char*)salt, crypto_pwhash_SALTBYTES);

				std::string dataString = data.dump();

				//size_t abced = dataString.size();
				//const char* def2 = (const char*)dataString.size();
				//encFile.write((const char*)def2, 8);

				unsigned char* entryData = reinterpret_cast<unsigned char*>(&dataString[0]);

				unsigned char * encryptedMessage = new unsigned char[dataString.size() + crypto_secretstream_xchacha20poly1305_ABYTES];

				unsigned long long textLen = strlen((char*)entryData);

				crypto_secretstream_xchacha20poly1305_push(&state, encryptedMessage, NULL, entryData, textLen + crypto_secretstream_xchacha20poly1305_ABYTES, NULL, 0, crypto_secretstream_xchacha20poly1305_TAG_FINAL);

				if (encFile.is_open())
				{
					encFile.write((const char*)encryptedMessage, dataString.size() + crypto_secretstream_xchacha20poly1305_ABYTES);
				}
				else
				{
					assert(false);
				}
				encFile.close();

			}
		}
		else
		{
			assert(false);
		}
	}

	std::string decryptEntry() 
	{
		json data;
		std::ifstream encFile;
		std::string vec;

		encFile.open("example.enc", std::ofstream::binary);

		unsigned char tag = 0;
		unsigned char key[crypto_secretstream_xchacha20poly1305_KEYBYTES];
		unsigned char header[crypto_secretstream_xchacha20poly1305_HEADERBYTES];
		encFile.read((char *)header, crypto_secretstream_xchacha20poly1305_HEADERBYTES);
		unsigned char salt[crypto_pwhash_SALTBYTES];
		encFile.read((char*)salt, crypto_pwhash_SALTBYTES);

		crypto_secretstream_xchacha20poly1305_state state;


		crypto_pwhash(key, sizeof(key), pwd, strlen(pwd), salt, crypto_pwhash_OPSLIMIT_INTERACTIVE, crypto_pwhash_MEMLIMIT_INTERACTIVE, crypto_pwhash_ALG_DEFAULT);
		
		encFile.seekg(0, encFile.end);
		int length = encFile.tellg();
		length -= 40;
		encFile.seekg(40);
		int current = encFile.tellg();

		unsigned char * c1 = new unsigned char[length - 30]; // TODO: is the encrypted lenght = unencrypted lenght?
		encFile.read((char *) c1, 80);

		if (crypto_secretstream_xchacha20poly1305_init_pull(&state, header, key) != 0)
		{
			int asdfjk = 0;
		}

		assert(tag == 0);

		unsigned char m1[10000];
		unsigned long long m1Len = 10000;
		//Problem Code
		crypto_secretstream_xchacha20poly1305_pull(&state, m1, &m1Len, &tag, c1, length, NULL, 0);
		assert(tag == 0);

		data = m1;

		vec = data["diaryEntryText"];
		return vec;
	}

	int saveDiaryEntry(std::string textData)
	{
		
		if (isEncrypted)
		{
			encryptEntry(textData);
			return 0;
		}
		else 
		{
			json data;
			std::ofstream jsonFile;

			data["diaryEntryText"] = textData;

			jsonFile.open("example.json");


			if (jsonFile.is_open())
			{
				jsonFile << data;
			}
			else
			{
				return 1;
			}

			jsonFile.close();
		}

		return 0;
	}

	std::string loadDiaryEntry()
	{

		if(isEncrypted)
		{
			return decryptEntry();
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