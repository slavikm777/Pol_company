#pragma once
#include "MyWindow.h"

//Класс базового окна
class UIMainWindow : public MyWindow
{
private:
	enum EWidgetsID { // Идентификаторы элементов
		BTN_SETTINGS = 100,
		BTN_INFO,
		BTN_EXIT,
		BTN_STATUS,
		BTN_LOGIN,
		BTN_LOGOUT,
		BTN_USERS,
		BTN_CREATE_ORDER,
		BTN_PARTNERS
	};

	enum EFontsID // Идентификаторы шрифтов
	{
		FONT_ROBOTO1 = 1001,
		FONT_ROBOTO2
	};

	wstring RoleToText(eRole x)
	{
		switch (x)
		{
		case Administrator:return L"Администратор";
			break;
		case Manager: return L"Менеджер";
			break;
		case Master: return L"Мастер";
			break;
		case Analitic: return L"Аналитик";
			break;
		case ConnectedDB: return L"Подключен к БД";
			break;
		case DisconnectDB: return L"Нет подключения к БД";
			break;
		}
	}

	void LogOut()
	{
		int result = MessageBox(nullptr, L"Выйти?", L"Выход из учетной записи", MB_YESNO | MB_ICONQUESTION);
		if (result == IDYES) {
			_settings->_userSettings.login = L"";
			_settings->_userSettings.password = L"";
			_settings->_userSettings.role = eRole::ConnectedDB;
			_settings->_userSettings.inLogin = false;
			_settings->SaveSettings();
		}
	}

public:

	UIMainWindow(HINSTANCE hInstance, EWindowID prevWindow) : MyWindow(hInstance, prevWindow)
	{
		CreateClass<UIMainWindow>(hInstance, L"MainWindow", 1024, 640);
		Initialize();
	}

	UIMainWindow(HINSTANCE hInstance, EWindowID prevWindow, SColor color) : MyWindow(hInstance, prevWindow, color)
	{
		CreateClass<UIMainWindow>(hInstance, L"MainWindow", 1024, 640);
		Initialize();
	}

	virtual void Show(int nCmdShow)
	{
		if (_dataBase->Connect())
		{
			if (_settings->_userSettings.role == eRole::DisconnectDB)
				_settings->_userSettings.role = eRole::ConnectedDB;
			_dataBase->InLogin();
		}
		else
		{
			_settings->_userSettings.role = eRole::DisconnectDB;
		}
		UpdateWidgetsState();
		MyWindow::Show(nCmdShow);
	}

protected:
	virtual void Initialize() override
	{
		_dataBase->SetParentWindow(_hwnd);
		AddFont(20, L"SegoeUI", FW_BOLD, true, false, FONT_ROBOTO1);
		AddFont(15, L"SegoeUI", FW_BOLD, true, false, FONT_ROBOTO2);
		MainWndAddMenues();
		CreateWidgets();
	}

	virtual void MainWndAddMenues() override
	{
		HMENU RootMenu = CreateMenu();
		HMENU SubMenu = CreateMenu();

		AppendMenu(SubMenu, MF_STRING, BTN_SETTINGS, L"Настройки подключения БД");
		AppendMenu(SubMenu, MF_STRING, BTN_INFO, L"Информация о проекте");
		AppendMenu(SubMenu, MF_SEPARATOR, NULL, NULL);
		AppendMenu(SubMenu, MF_STRING, BTN_EXIT, L"Выход");

		AppendMenu(RootMenu, MF_POPUP, (UINT_PTR)SubMenu, L"Помощь");
		SetMenu(_hwnd, RootMenu);
	}

	virtual void HandleMenuCommand(int menuId) override
	{
		switch (menuId) {
		case BTN_INFO:
			MessageBoxW(_hwnd, L"Производственная компания «Мастер пол»", L"Инфо", MB_OK);
			break;
		case BTN_EXIT: 
			Close(); 
			break;
		case BTN_SETTINGS:
			CreateOtherWindow(EUISettings, SColor::AdditionalColor()); // Переход в окно настроек
			break;
		}
	}

	virtual void CreateWidgets()
	{
		CreateCustomButton(_hwnd, RoleToText(_settings->_userSettings.role), 750, 10, 250, 30, BTN_STATUS, 
			SColor::AccentuationColor(), SColor(), _fonts[FONT_ROBOTO2]);
		CreateCustomButton(_hwnd, L"Авторизация", 800, 50, 180, 40, BTN_LOGIN, SColor::AccentuationColor(), SColor(), _fonts[FONT_ROBOTO2]);
		CreateCustomButton(_hwnd, L"Выйти", 800, 50, 180, 40, BTN_LOGOUT, SColor(255, 0, 130), SColor(), _fonts[FONT_ROBOTO2]);
		CreateCustomButton(_hwnd, L"Авторизация пользователей", 370, 150, 320, 40, BTN_USERS, SColor::AccentuationColor(), SColor(), _fonts[FONT_ROBOTO2], false);
		CreateCustomButton(_hwnd, L"Заявки", 370, 150, 320, 40, BTN_CREATE_ORDER, SColor::AccentuationColor(), SColor(), _fonts[FONT_ROBOTO2], false);
		CreateCustomButton(_hwnd, L"Партнеры", 370, 200, 320, 40, BTN_PARTNERS, SColor::AccentuationColor(), SColor(), _fonts[FONT_ROBOTO2], false);

		SetWidgetPermissions(BTN_STATUS, WidgetState::Enabled, { eRole::Disabled });
		SetWidgetPermissions(BTN_LOGIN, WidgetState::Visible, { eRole::ConnectedDB });
		SetWidgetPermissions(BTN_LOGOUT, WidgetState::Visible, { eRole::Administrator, eRole::Analitic, eRole::Manager, eRole::Master });
		SetWidgetPermissions(BTN_USERS, WidgetState::Visible, { eRole::Administrator });
		SetWidgetPermissions(BTN_CREATE_ORDER, WidgetState::Visible, { eRole::Manager, eRole::Master });
		SetWidgetPermissions(BTN_PARTNERS, WidgetState::Visible, { eRole::Manager });
	}

	virtual void UpdateWidgetsState() override
	{
		MyWindow::UpdateWidgetsState();
		if (GetCustomButton(BTN_STATUS) == nullptr) return;
		switch (_settings->_userSettings.role)
		{
		case DisconnectDB:
			GetCustomButton(BTN_STATUS)->SetBackgroundColor(SColor(240, 10, 10));
			break;
		default:
			GetCustomButton(BTN_STATUS)->SetBackgroundColor(SColor::AccentuationColor());
			break;
		}
		GetCustomButton(BTN_STATUS)->SetText(RoleToText(_settings->_userSettings.role));
	}

	virtual void HandleButtonClick(int buttonId) override
	{
		switch (buttonId) {
		case BTN_LOGIN:
			CreateOtherWindow(EUILogin, SColor::AdditionalColor()); // Переход в окно авторизации
			break;
		case BTN_LOGOUT:
			LogOut();
			break;
		case BTN_USERS:
			CreateOtherWindow(EUILogin, SColor::AdditionalColor());
			break;
		case BTN_PARTNERS:
			CreateOtherWindow(EUIListPartner, SColor::BaseColor());
			break;
		case BTN_CREATE_ORDER:
			CreateOtherWindow(EUIListOrder, SColor::BaseColor());
			break;
		}
		UpdateWidgetsState();
	}

	bool IsMainWindow() const override { return true; }
};