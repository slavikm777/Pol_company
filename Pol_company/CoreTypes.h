#pragma once
#include "Windows.h"
#include <string>

struct SColor
{
	//#FFFFFF - 255,255,255 - Основной фон
	//#F4E8D3 - 244,232,211 - Дополнительный фон
	//#67BA80 - 103,186,128 - Акцентирование внимания
	BYTE R, G, B;

	SColor()
	{
		R = 0;
		G = 0;
		B = 0;
	}

	SColor(int red, int green, int blue)
		: R(red), G(green), B(blue) {
	}

	static const SColor BaseColor() //Основной фон
	{
		return SColor(255, 255, 255);
	}

	static const SColor AdditionalColor() //Дополнительный фон
	{
		return SColor(244, 232, 211);
	}

	static const SColor AccentuationColor() //Акцентирование внимания
	{
		return SColor(103, 186, 128);
	}
};

enum EWindowID
{
	EUIMainWindow = 1, //Главное окно
	EUISettings, //Окно настроек
	EUILogin, //Окно авторизации
	EUIRegister, // Окно регистрации
	EUIChangePassword, // Окно смены пароля
	EUIRegisterPartner, //Окно регистрации партнеров
	EUIListPartner, // Окно списка партнеров
	EUIPartnerInfo // Окно информации партнера
};

enum eRole
{
	Administrator = 1,
	Manager = 2,
	Master = 3,
	Analitic = 4,
	ConnectedDB = 100,
	DisconnectDB = 101,
	Disabled = 102
};