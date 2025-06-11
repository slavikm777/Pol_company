#pragma once
#include "Windows.h"
#include <string>

struct SColor
{
	//#FFFFFF - 255,255,255 - �������� ���
	//#F4E8D3 - 244,232,211 - �������������� ���
	//#67BA80 - 103,186,128 - �������������� ��������
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

	static const SColor BaseColor() //�������� ���
	{
		return SColor(255, 255, 255);
	}

	static const SColor AdditionalColor() //�������������� ���
	{
		return SColor(244, 232, 211);
	}

	static const SColor AccentuationColor() //�������������� ��������
	{
		return SColor(103, 186, 128);
	}
};

enum EWindowID
{
	EUIMainWindow = 1, //������� ����
	EUISettings, //���� ��������
	EUILogin, //���� �����������
	EUIRegister, // ���� �����������
	EUIChangePassword, // ���� ����� ������
	EUIRegisterPartner, //���� ����������� ���������
	EUIListPartner, // ���� ������ ���������
	EUIPartnerInfo // ���� ���������� ��������
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