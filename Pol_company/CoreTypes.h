#pragma once
#include "Windows.h"

struct SColor
{
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
};

enum EWindowID
{
	EUIMainWindow = 1, //Главное окно
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