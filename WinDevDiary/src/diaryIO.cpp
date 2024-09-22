// diaryIO.cpp: Responsible for saving/loading/encrypting/decrypting diary entries with JSON
#include <fstream>
#include <string>
#include "sodium.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

extern bool isEncrypted;
std::string password = "Hase12774#3??23";

namespace WDDsave 
{

	void encryptEntry(std::string textData) 
	{
		// INIT DATA
		json data;
		std::ofstream jsonFile;

		data["diaryEntryText"] = textData;

		jsonFile.open("example.enc");

		unsigned char pwHash[crypto_box_SEEDBYTES];
		unsigned char nonce[crypto_secretbox_NONCEBYTES];
		unsigned char salt[crypto_pwhash_SALTBYTES];

		randombytes_buf(salt, sizeof salt);

		bool tempTrue = true;

		if (tempTrue) {
			if (crypto_pwhash(pwHash, sizeof(pwHash), password.data(), sizeof(password), salt, crypto_pwhash_OPSLIMIT_INTERACTIVE, crypto_pwhash_MEMLIMIT_INTERACTIVE,
				crypto_pwhash_ALG_DEFAULT) != 0)
				int sdf = 0; //Placeholder, fix later
			else
			{
				unsigned char header[crypto_secretstream_xchacha20poly1305_HEADERBYTES];

				crypto_secretstream_xchacha20poly1305_state state;

				crypto_secretstream_xchacha20poly1305_keygen(pwHash);
				crypto_secretstream_xchacha20poly1305_init_push(&state, header, pwHash);

				//PROBLEMSTELLE

				std::string dataString = data.dump();

				unsigned char* trap = new unsigned char[dataString.size()];
				std::copy(dataString.begin(), dataString.end(), trap);
				//PROBLEMSTELLE ENDE


				unsigned char trap2[1000] = "sfdkkkkkkkkkkkkkkkkkkkkkskldjfklsjöldkfjaklsjdklfjaskldjkfljöalsjdklfjsaeifasbxobcvoibiosofihioehoisdfkljsdklfjlöakshdfoiabsodivboiypxciovhosdihfiosehiorhoiahsifoahoshdpfhasdpf";
				unsigned char crypted[sizeof(trap2)] = "";


				crypto_secretstream_xchacha20poly1305_push(&state, crypted, NULL, trap2, sizeof(trap2) + crypto_secretstream_xchacha20poly1305_ABYTES, NULL, 0, crypto_secretstream_xchacha20poly1305_TAG_FINAL);

				if (jsonFile.is_open())
				{
					jsonFile << crypted;
				}
				else
				{
					int askldfjklasdf = 0; // Also just for testing
				}

				jsonFile.close();

			}
		}
		else
		{
			int a = 0; //Placeholder, fix later
		}
	}

	std::string decryptEntry() 
	{
		json data;
		std::ifstream jsonFile;
		std::string vec;
		unsigned char nonce[crypto_secretbox_NONCEBYTES];
		unsigned char salt[crypto_pwhash_SALTBYTES];

		randombytes_buf(salt, sizeof salt);

		jsonFile.open("example.enc");

		if (jsonFile.is_open())
		{
			int asdfksdf = 3;
		}
		else
		{
			return "Failed to load/open JSON file";
		}

		unsigned char tag = 0;
		std::string cyph;
		jsonFile >> cyph;
		unsigned char* c1 = new unsigned char[cyph.size()];

		crypto_secretstream_xchacha20poly1305_state state;
		unsigned char pwHash[crypto_secretstream_xchacha20poly1305_KEYBYTES];
		unsigned char header[crypto_secretstream_xchacha20poly1305_HEADERBYTES];

		crypto_pwhash(pwHash, sizeof(pwHash), password.data(), sizeof(password), salt, crypto_pwhash_OPSLIMIT_INTERACTIVE, crypto_pwhash_MEMLIMIT_INTERACTIVE,
			crypto_pwhash_ALG_DEFAULT);


		if (crypto_secretstream_xchacha20poly1305_init_pull(&state, header, pwHash) != 0)
		{
			int asdfjk = 0;
		}

		//assert(tag == 0);

		unsigned char m1[10000];

		if (crypto_secretstream_xchacha20poly1305_pull(&state, m1, NULL, &tag, c1, 106, NULL, 0) != 0)
		{
			int asdjklsfjklsdfjkl = 0;
		}
		//assert(tag == 0);

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