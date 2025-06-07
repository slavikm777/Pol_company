#pragma once
#include "MyWindow.h"
#include "vector"

class UIRegister : public MyWindow
{
private:
	enum EWidgetsID { // Идентификаторы элементов
		BTN_REGISTER = 1,
		BTN_BACK,
		EDIT_LOGIN,
		EDIT_PASSWORD1,
		EDIT_PASSWORD2,
		STAT_TEXT1,
		STAT_TEXT2,
		STAT_TEXT3,
		EDIT_FIO,
		DATE_BIRTH,
		PASPORT,
		BANK,
		STAT_TEXT4,
		STAT_TEXT5,
		STAT_TEXT6,
		STAT_TEXT7,
		COMBOBOX1
	};

	enum EFontsID // Идентификаторы шрифтов
	{
		FONT_ROBOTO1 = 1001,
		FONT_ROBOTO2,
		FONT_ROBOTO3,
		FONT_ROBOTO4
	};

	vector<string> _roles;

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
		string login, password, password2, fio, birthday, pasport, bank;
		eRole registerRole = BoxToRole();
		GetWindowTextAsString(_widgets[EDIT_LOGIN], login);
		GetWindowTextAsString(_widgets[EDIT_PASSWORD1], password);
		GetWindowTextAsString(_widgets[EDIT_PASSWORD2], password2);
		GetWindowTextAsString(_widgets[EDIT_FIO], fio);
		GetWindowTextAsString(_widgets[DATE_BIRTH], birthday);
		GetWindowTextAsString(_widgets[PASPORT], pasport);
		GetWindowTextAsString(_widgets[BANK], bank);
		if (password.empty() || login.empty() || password2.empty() || fio.empty() || birthday.empty() || pasport.empty() || bank.empty())
		{
			MessageBox(nullptr, L"Заполните все поля", L"Ошибка!", MB_ICONERROR);
			return;
		}

		if (password != password2)
		{
			MessageBox(nullptr, L"Пароли не совпадают", L"Ошибка!", MB_ICONERROR);
			return;
		}

		if (_dataBase->HasAdministrator())
			if (_dataBase->RegisterUser(login, password, registerRole, fio, birthday, pasport, bank, true))
			{
				MessageBoxW(_hwnd, L"Пользователь зарегестрирован!", L"Инфо", MB_OK);
				CreateOtherWindow(EUILogin, SColor::AdditionalColor());
			}
			else MessageBox(nullptr, L"Пользователь не зарегестрирован!", L"Ошибка!", MB_ICONERROR);
		else
		{
			if (BoxToRole() == eRole::Administrator)
			{
				if (_dataBase->RegisterUser(login, password, registerRole, fio, birthday, pasport, bank, true))
				{
					MessageBoxW(_hwnd, L"Пользователь зарегестрирован!", L"Инфо", MB_OK);
					CreateOtherWindow(EUILogin, SColor::AdditionalColor());
				}
			}
			else MessageBox(nullptr, L"Создайте пользователя с привелегиями администратора", L"Ошибка!", MB_ICONERROR);
		}
	}

public:
	UIRegister(HINSTANCE hInstance, EWindowID prevWindow) : MyWindow(hInstance, prevWindow)
	{
		CreateClass<UIRegister>(hInstance, L"Registeration", 1024, 640);
		Initialize();
	}

	UIRegister(HINSTANCE hInstance, EWindowID prevWindow, SColor color) : MyWindow(hInstance, prevWindow, color)
	{
		CreateClass<UIRegister>(hInstance, L"Registeration", 1024, 640);
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
		CreateStatic(L"static", L"Логин -->", 10, 72, 300, 100, STAT_TEXT1, _fonts[FONT_ROBOTO3]);
		CreateStatic(L"static", L"Новый пароль ->", 10, 122, 300, 100, STAT_TEXT2, _fonts[FONT_ROBOTO2]);
		CreateStatic(L"static", L"Новый пароль -->", 10, 172, 300, 100, STAT_TEXT3, _fonts[FONT_ROBOTO2]);
		CreateStatic(L"static", L"ФИО -->", 10, 222, 300, 100, STAT_TEXT4, _fonts[FONT_ROBOTO3]);
		CreateStatic(L"static", L"Дата рождения -->", 10, 272, 300, 100, STAT_TEXT5, _fonts[FONT_ROBOTO2]);
		CreateStatic(L"static", L"Номер паспорта -->", 10, 322, 300, 100, STAT_TEXT6, _fonts[FONT_ROBOTO2]);
		CreateStatic(L"static", L"Реквезиты банка -->", 10, 372, 300, 100, STAT_TEXT7, _fonts[FONT_ROBOTO2]);
		CreateControl(L"EDIT", L"", 120, 70, 830, 25, EDIT_LOGIN, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, _fonts[FONT_ROBOTO3]);
		CreateControl(L"EDIT", L"", 160, 120, 790, 25, EDIT_PASSWORD1, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_PASSWORD, _fonts[FONT_ROBOTO3]);
		CreateControl(L"EDIT", L"", 160, 170, 790, 25, EDIT_PASSWORD2, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_PASSWORD, _fonts[FONT_ROBOTO3]);
		CreateControl(L"EDIT", L"", 120, 220, 830, 25, EDIT_FIO, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, _fonts[FONT_ROBOTO3]);
		CreateControl(L"EDIT", L"", 190, 270, 350, 25, DATE_BIRTH, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, _fonts[FONT_ROBOTO3]);
		CreateControl(L"EDIT", L"", 190, 320, 350, 25, PASPORT, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, _fonts[FONT_ROBOTO3]);
		CreateControl(L"EDIT", L"", 190, 370, 350, 25, BANK, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, _fonts[FONT_ROBOTO3]);

		CreateControl(L"BUTTON", L"Назад", 20, 20, 150, 30, BTN_BACK, WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, _fonts[FONT_ROBOTO1]);
		CreateControl(L"BUTTON", L"Регистрация", 430, 500, 200, 40, BTN_REGISTER, WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, _fonts[FONT_ROBOTO4]);

		CreateControl(L"COMBOBOX", L"", 700, 450, 240, 200, COMBOBOX1, WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL, _fonts[FONT_ROBOTO2]);
		SetWidgetPermissions(COMBOBOX1, WidgetState::Visible, { eRole::Administrator });
	}

	virtual void UpdateWidgetSettings() override
	{
		UpdateControlText(EDIT_LOGIN, "");
		UpdateControlText(EDIT_PASSWORD1, "");
		UpdateControlText(EDIT_PASSWORD2, "");
		// Добавление элементов в комбобокс
		_roles = _dataBase->LoadTitle("_role");
		for (int i = 0; i < _roles.size(); i++)
			SendMessage(_widgets[COMBOBOX1], CB_INSERTSTRING, i, (LPARAM)StringToWide(_roles[i]).c_str());
		SendMessage(_widgets[COMBOBOX1], CB_SETCURSEL, 1, 0);
	}

	virtual void HandleButtonClick(int buttonId) override
	{
		switch (buttonId)
		{
		case BTN_BACK:
			CreateOtherWindow(EUILogin, SColor::AdditionalColor());
			break;
		case BTN_REGISTER:
			Register();
			break;
		}
		UpdateWidgetsState();
	}
};