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
		data["diaryEntryText"] = textData;

		unsigned char key[crypto_box_SEEDBYTES];
		unsigned char nonce[crypto_secretbox_NONCEBYTES];
		unsigned char salt[crypto_pwhash_SALTBYTES];
		unsigned char header[crypto_secretstream_xchacha20poly1305_HEADERBYTES];

		randombytes_buf(nonce, sizeof(nonce));
		randombytes_buf(salt, sizeof(salt));

		if (std::filesystem::exists("example.enc"))
		{
			if (remove("example.enc"))
			{
				throw std::runtime_error("Unable to delete file. Does another program use this file?");
			}
			else
			{
				encFile.open("example.enc", std::ofstream::binary | std::ofstream::app);
			}
		}
		else
			encFile.open("example.enc", std::ofstream::binary | std::ofstream::app);

		if (sodium_init() != 0)
			throw std::runtime_error("Could not initialize encryption (libsodium)");

		if (crypto_pwhash(key, sizeof(key), pwd, strlen(pwd), salt, crypto_pwhash_OPSLIMIT_INTERACTIVE, crypto_pwhash_MEMLIMIT_INTERACTIVE, crypto_pwhash_ALG_DEFAULT) != 0)
			throw std::runtime_error("Could not initialize password hash");

		//crypto_secretstream_xchacha20poly1305_state state;
		//crypto_secretstream_xchacha20poly1305_init_push(&state, header, key);

		std::string dataString = data.dump();

		//size_t abced = dataString.size();
		//const char* def2 = (const char*)dataString.size();
		//encFile.write((const char*)def2, 8);

		int entryLength = dataString.size() + crypto_secretstream_xchacha20poly1305_ABYTES;
		unsigned char* entryData = reinterpret_cast<unsigned char*>(&dataString[0]);
		unsigned char encryptedMessage[56];
		unsigned char m1[23];

		crypto_secretbox_easy(encryptedMessage, entryData, entryLength, nonce, key);
		//crypto_secretstream_xchacha20poly1305_push(&state, encryptedMessage, &cryptLen, entryData, entryLength, NULL, 0, crypto_secretstream_xchacha20poly1305_TAG_FINAL);
		unsigned long long cryptLen = strlen((const char*)encryptedMessage);

		if (crypto_secretbox_open_easy(m1, encryptedMessage, sizeof((const char *) encryptedMessage), nonce, key) != 0)
			//throw std::runtime_error("Decryption failed");

		if (encFile.is_open())
		{
			//encFile.write((const char*)header, crypto_secretstream_xchacha20poly1305_HEADERBYTES);
			encFile.write((const char*)salt, crypto_pwhash_SALTBYTES);
			encFile.write((const char*)nonce, crypto_secretbox_NONCEBYTES);
			encFile.write((const char*)encryptedMessage, cryptLen);
		}
		else
		{
			throw std::runtime_error("Could not save encrypted diary file");
		}
		encFile.close();
	}

	std::string decryptEntry()
	{
		json data;
		std::ifstream encFile;
		std::string entry;

		encFile.open("example.enc", std::ofstream::binary);

		//if (sodium_init() != 0)
			//throw std::runtime_error("Could not initialize encryption (libsodium)");

		unsigned char tag;
		unsigned char key[crypto_secretstream_xchacha20poly1305_KEYBYTES];
		unsigned char header[crypto_secretstream_xchacha20poly1305_HEADERBYTES];
		unsigned char salt[crypto_pwhash_SALTBYTES];
		unsigned char nonce[crypto_pwhash_SALTBYTES];


		//encFile.read((char*)header, crypto_secretstream_xchacha20poly1305_HEADERBYTES);
		encFile.read((char*)salt, crypto_pwhash_SALTBYTES);
		encFile.read((char*)nonce, crypto_secretbox_NONCEBYTES);



		if(crypto_pwhash(key, sizeof(key), pwd, strlen(pwd), salt, crypto_pwhash_OPSLIMIT_INTERACTIVE, crypto_pwhash_MEMLIMIT_INTERACTIVE, crypto_pwhash_ALG_DEFAULT) != 0)
			throw std::runtime_error("Key derivation failed");

		encFile.seekg(0, encFile.end);
		int length = encFile.tellg();
		length -= crypto_pwhash_SALTBYTES + crypto_secretbox_NONCEBYTES; // subtract 40 bytes
		encFile.seekg(40);
		int current = encFile.tellg(); // Only for debug purpouse

		unsigned char* c1 = new unsigned char[length + 1](); // TODO: is the encrypted lenght = unencrypted lenght?
		encFile.read((char*)c1, length);

		unsigned char* m1 = new unsigned char[length]();
		unsigned long long m1Len;

		//crypto_secretstream_xchacha20poly1305_state state;
		//if (crypto_secretstream_xchacha20poly1305_init_pull(&state, header, key) != 0)
			//throw std::runtime_error("Decryption initialization failed");

		//Problem Code
		//if (crypto_secretstream_xchacha20poly1305_pull(&state, m1, &m1Len, &tag, c1, length, NULL, 0) != 0)
			//throw std::runtime_error("Decryption failed");

		if(crypto_secretbox_open_easy(m1, c1, length, nonce, key) != 0)
			throw std::runtime_error("Decryption failed");


		entry = data["diaryEntryText"];
		return entry;
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

		if (isEncrypted)
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