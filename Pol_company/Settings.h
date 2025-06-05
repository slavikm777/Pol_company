#pragma once
#include <fstream>
#include <string>
#include <Windows.h>
#include "CoreTypes.h"

using namespace std;

struct sMainSettings
{
	wstring databaseName, userName, password, server;
};

struct sUserSettings
{
	bool inLogin = false;
	wstring login, password;
	eRole role = eRole::DisconnectDB;
};

class Settings
{
public:
	sMainSettings _mainSettings;
	sUserSettings _userSettings;

	Settings(const Settings&) = delete;
	Settings& operator=(const Settings&) = delete;

	static void ReleaseInstance() {
		if (_instance_ptr)
			delete _instance_ptr;
		_instance_ptr = nullptr;
	}

	static Settings* GetInstance()
	{
		if (_instance_ptr == nullptr)
			_instance_ptr = new Settings();
		return _instance_ptr;
	}

private:
	static Settings* _instance_ptr;
	wstring _fileName = L"settings.bin";

	Settings();
	~Settings() = default;

	void WriteString(wofstream& outFile, const wstring& x) const;
	void ReadString(wifstream& inFile, wstring& str);

public:
	bool SaveSettings(bool debug = false) const;
	bool LoadSettings(bool debug = false);
};