#include "Settings.h"
#include <iostream>

Settings* Settings::_instance_ptr = nullptr;

Settings::Settings()
{
	_mainSettings.databaseName = L"master_pol_db";
	_mainSettings.userName = L"root";
	_mainSettings.password = L"77777777";
	_mainSettings.server = L"tcp://127.0.0.1:3306";
	_userSettings.inLogin = false;
	_userSettings.login = L"";
	_userSettings.password = L"";
}

void Settings::WriteString(wofstream& outFile, const wstring& x) const
{
	// Записываем длину строки как 32-битное число
	const uint32_t size = static_cast<uint32_t>(x.size());
	outFile.write(reinterpret_cast<const wchar_t*>(&size), sizeof(uint32_t) / sizeof(wchar_t));

	if (size > 0) {
		outFile.write(x.c_str(), size);
	}
}

void Settings::ReadString(wifstream& inFile, wstring& str)
{
	uint32_t size = 0;
	inFile.read(reinterpret_cast<wchar_t*>(&size), sizeof(uint32_t) / sizeof(wchar_t));

	str.resize(size);
	if (size > 0) {
		inFile.read(&str[0], size);
	}
}

bool Settings::SaveSettings(bool debug) const
{
	std::wofstream outFile(_fileName, std::ios::binary);
	if (!outFile) {
		if (debug) std::wcerr << L"Ошибка открытия файла для записи: " << _fileName << std::endl;
		return false;
	}

	// Сохраняем все данные
	WriteString(outFile, _mainSettings.databaseName);
	WriteString(outFile, _mainSettings.userName);
	WriteString(outFile, _mainSettings.password);
	WriteString(outFile, _mainSettings.server);

	WriteString(outFile, _userSettings.login);
	WriteString(outFile, _userSettings.password);

	if (debug) std::wcout << L"Данные сохранены в " << _fileName << std::endl;
	return true;
}

bool Settings::LoadSettings(bool debug)
{
	std::wifstream inFile(_fileName, std::ios::binary);
	if (!inFile) {
		if (debug) std::wcerr << L"Ошибка открытия файла для чтения: " << _fileName << std::endl;
		return false;
	}

	// Читаем все данные
	ReadString(inFile, _mainSettings.databaseName);
	ReadString(inFile, _mainSettings.userName);
	ReadString(inFile, _mainSettings.password);
	ReadString(inFile, _mainSettings.server);

	ReadString(inFile, _userSettings.login);
	ReadString(inFile, _userSettings.password);

	if (debug) std::wcout << L"Данные загружены из " << _fileName << std::endl;
	return true;
}