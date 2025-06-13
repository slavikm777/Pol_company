#pragma once
#include "MyWindow.h"
#include "vector"

class UIRegisterPartner : public MyWindow
{
private:
	enum EWidgetsID { // Идентификаторы элементов
		BTN_REGISTER = 1,
		BTN_BACK,
		EDIT_NAME,
		EDIT_ADRES,
		EDIT_DIRECTOR,
		EDIT_NUMBER,
		EDIT_EMAIL,
		EDIT_RATING,
		EDIT_INN,
		COMBOBOX1,
		T_NAME,
		T_ADRESS,
		T_DIRECTOR,
		T_NUMBER,
		T_EMAIL,
		T_RATING,
		T_DISCOUNT,
		T_TYPE
	};

	enum EFontsID // Идентификаторы шрифтов
	{
		FONT_ROBOTO1 = 1001,
		FONT_ROBOTO2,
		FONT_ROBOTO3,
		FONT_ROBOTO4
	};

	vector<STable> _partnerType;

	eRole BoxToRole()
	{
		int itemIndex = SendMessage(_widgets[COMBOBOX1], CB_GETCURSEL, 0, 0) + 1;
		switch (itemIndex)
		{
		case 1: return eRole::Administrator;
		case 2: return eRole::Manager;
		case 3: return eRole::Master;
		case 4: return eRole::Analitic;
		default: return eRole::ConnectedDB;
		}
	}

	void Register()
	{
		string name, director, adress, number, email, rating, discount, inn;
		int indexPartner = SendMessage(_widgets[COMBOBOX1], CB_GETCURSEL, 0, 0);
		int id_partner = _partnerType[indexPartner].id;
		GetWindowTextAsString(_widgets[EDIT_NAME], name);
		GetWindowTextAsString(_widgets[EDIT_DIRECTOR], director);
		GetWindowTextAsString(_widgets[EDIT_EMAIL], email);
		GetWindowTextAsString(_widgets[EDIT_NUMBER], number);
		GetWindowTextAsString(_widgets[EDIT_ADRES], adress);
		GetWindowTextAsString(_widgets[EDIT_INN], inn);
		GetWindowTextAsString(_widgets[EDIT_RATING], rating);

		if (name.empty() || adress.empty() || director.empty() || number.empty() || email.empty() || rating.empty())
		{
			MessageBox(nullptr, L"Заполните все поля", L"Ошибка!", MB_ICONERROR);
			return;
		}

		if(_dataBase->RegisterPartner(name, director, email, number, adress, inn, rating, id_partner, true))
		{
			MessageBoxW(_hwnd, L"Партнер зарегестрирован!", L"Инфо", MB_OK);
			CreateOtherWindow(EUIListPartner, SColor::AdditionalColor());
		}
	}

public:
	UIRegisterPartner(HINSTANCE hInstance, EWindowID prevWindow) : MyWindow(hInstance, prevWindow)
	{
		CreateClass<UIRegisterPartner>(hInstance, L"RegisterationPartner", 1024, 640);
		Initialize();
	}

	UIRegisterPartner(HINSTANCE hInstance, EWindowID prevWindow, SColor color) : MyWindow(hInstance, prevWindow, color)
	{
		CreateClass<UIRegisterPartner>(hInstance, L"RegisterationPartner", 1024, 640);
		Initialize();
	}

	virtual void Show(int nCmdShow)
	{
		MyWindow::Show(nCmdShow);
		if (_settings->_userSettings.inLogin == false)
		{
			if (_dataBase->HasAdministrator() == false)
			{
				SendMessage(_widgets[COMBOBOX1], CB_SETCURSEL, 0, 0);
				SetWidgetPermissions(COMBOBOX1, WidgetState::Visible, { eRole::ConnectedDB });
				UpdateWidgetsState();
			}
			else _settings->_userSettings.role = eRole::ConnectedDB;
		}
	}

protected:
	virtual void Initialize() override
	{
		AddFont(16, L"Roboto", FW_BOLD, false, false, FONT_ROBOTO1);
		AddFont(12, L"Roboto", FW_BOLD, false, false, FONT_ROBOTO2);
		AddFont(14, L"Roboto", FW_BOLD, false, false, FONT_ROBOTO3);
		AddFont(20, L"Roboto", FW_BOLD, false, false, FONT_ROBOTO4);
		MyWindow::Initialize();
	};

	virtual void CreateWidgets() override
	{
		CreateStatic(L"static", L"Название компании -->", 10, 72, 300, 100, T_NAME, _fonts[FONT_ROBOTO3]);
		CreateStatic(L"static", L"Адрес ->", 10, 122, 300, 100, T_ADRESS, _fonts[FONT_ROBOTO2]);
		CreateStatic(L"static", L"Директор -->", 10, 172, 300, 100, T_DIRECTOR, _fonts[FONT_ROBOTO2]);
		CreateStatic(L"static", L"Телефон -->", 10, 222, 300, 100, T_NUMBER, _fonts[FONT_ROBOTO3]);
		CreateStatic(L"static", L"@mail -->", 10, 272, 300, 100, T_EMAIL, _fonts[FONT_ROBOTO2]);
		CreateStatic(L"static", L"Рейтинг -->", 10, 322, 300, 100, T_RATING, _fonts[FONT_ROBOTO2]);
		CreateStatic(L"static", L"ИНН -->", 10, 372, 300, 100, T_DISCOUNT, _fonts[FONT_ROBOTO2]);
		CreateStatic(L"static", L"Тип компании -->", 10, 425, 300, 100, T_TYPE, _fonts[FONT_ROBOTO2]);
		CreateControl(L"EDIT", L"", 240, 70, 710, 25, EDIT_NAME, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, _fonts[FONT_ROBOTO3]);
		CreateControl(L"EDIT", L"", 160, 120, 790, 25, EDIT_ADRES, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, _fonts[FONT_ROBOTO3]);
		CreateControl(L"EDIT", L"", 160, 170, 790, 25, EDIT_DIRECTOR, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, _fonts[FONT_ROBOTO3]);
		CreateControl(L"EDIT", L"", 160, 220, 790, 25, EDIT_NUMBER, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, _fonts[FONT_ROBOTO3]);
		CreateControl(L"EDIT", L"", 160, 270, 790, 25, EDIT_EMAIL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, _fonts[FONT_ROBOTO3]);
		CreateControl(L"EDIT", L"", 160, 320, 790, 25, EDIT_RATING, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, _fonts[FONT_ROBOTO3]);
		CreateControl(L"EDIT", L"", 160, 370, 790, 25, EDIT_INN, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, _fonts[FONT_ROBOTO3]);
		CreateControl(L"COMBOBOX", L"", 160, 420, 240, 200, COMBOBOX1, WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL, _fonts[FONT_ROBOTO3]);

		CreateControl(L"BUTTON", L"Назад", 20, 20, 150, 30, BTN_BACK, WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, _fonts[FONT_ROBOTO1]);
		CreateControl(L"BUTTON", L"Регистрация", 430, 500, 200, 40, BTN_REGISTER, WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, _fonts[FONT_ROBOTO4]);

	}

	virtual void UpdateWidgetSettings() override
	{
		//UpdateControlText(EDIT_LOGIN, "");
		//UpdateControlText(EDIT_PASSWORD1, "");
		//UpdateControlText(EDIT_PASSWORD2, "");
		// Добавление элементов в комбобокс
		_partnerType = _dataBase->LoadTableTitle("_partner_type");
		for (int i = 0; i < _partnerType.size(); i++)
			SendMessage(_widgets[COMBOBOX1], CB_INSERTSTRING, i, (LPARAM)StringToWide(_partnerType[i].tableLine).c_str());
		SendMessage(_widgets[COMBOBOX1], CB_SETCURSEL, 0, 0);
	}

	virtual void HandleButtonClick(int buttonId) override
	{
		switch (buttonId)
		{
		case BTN_BACK:
			CreateOtherWindow(EUIListPartner, SColor::BaseColor());
			break;
		case BTN_REGISTER:
			Register();
			break;
		}
		UpdateWidgetsState();
	}
};