#include "MyWindow.h"

//Класс окна настроек
class UISettings : public MyWindow
{
private:
	enum EWidgetsID { // Идентификаторы элементов
		BTN_SAVE = 1,
		BTN_CONNECT,
		BTN_BACK,
		EDIT_DBNAME,
		EDIT_DBUSERNAME,
		EDIT_DBPASSWORD,
		EDIT_DBSERVER,
		STAT_TEXT1,
		STAT_TEXT2,
		STAT_TEXT3,
		STAT_TEXT4,
	};

	enum EFontsID // Идентификаторы шрифтов
	{
		FONT_ROBOTO1 = 1001,
		FONT_ROBOTO2,
		FONT_ROBOTO3
	};

	void SaveSettings()
	{
		string dbName, username, passw, server;
		GetWindowTextAsString(_widgets[EDIT_DBNAME], dbName);
		GetWindowTextAsString(_widgets[EDIT_DBUSERNAME], username);
		GetWindowTextAsString(_widgets[EDIT_DBPASSWORD], passw);
		GetWindowTextAsString(_widgets[EDIT_DBSERVER], server);
		_settings->_mainSettings.databaseName = StringToWide(dbName);
		_settings->_mainSettings.password = StringToWide(passw);
		_settings->_mainSettings.userName = StringToWide(username);
		_settings->_mainSettings.server = StringToWide(server);
		if (_settings->SaveSettings())
		{
			MessageBoxW(_hwnd, L"Настройки сохранены!", L"Инфо", MB_OK);
		}
		else
		{
			MessageBoxW(_hwnd, L"Ошибка сохранения! Файл сохранения открыт другим процессом.", L"Инфо", MB_OK);
		}
	}

	void ConnectDB()
	{
		string dbName, username, passw, server;
		GetWindowTextAsString(_widgets[EDIT_DBNAME], dbName);
		GetWindowTextAsString(_widgets[EDIT_DBUSERNAME], username);
		GetWindowTextAsString(_widgets[EDIT_DBPASSWORD], passw);
		GetWindowTextAsString(_widgets[EDIT_DBSERVER], server);
		_settings->_mainSettings.databaseName = StringToWide(dbName);
		_settings->_mainSettings.password = StringToWide(passw);
		_settings->_mainSettings.userName = StringToWide(username);
		_settings->_mainSettings.server = StringToWide(server);
		if (_dataBase->Connect(true))
		{
			_settings->_userSettings.role = eRole::ConnectedDB;
		}
		else
		{
			_settings->_userSettings.role = eRole::DisconnectDB;
		}
		UpdateWidgetsState();
	}

public:
	UISettings(HINSTANCE hInstance, EWindowID prevWindow) : MyWindow(hInstance, prevWindow)
	{
		CreateClass<UISettings>(hInstance, L"Settings", 840, 540);
		Initialize();
	}

	UISettings(HINSTANCE hInstance, EWindowID prevWindow, SColor color) : MyWindow(hInstance, prevWindow, color)
	{
		CreateClass<UISettings>(hInstance, L"Settings", 840, 540);
		Initialize();
	}

protected:
	virtual void Initialize() override
	{
		//_dataBase->SetParentWindow(_hwnd);
		AddFont(16, L"Roboto", FW_BOLD, false, false, FONT_ROBOTO1);
		AddFont(12, L"Roboto", FW_BOLD, false, false, FONT_ROBOTO2);
		AddFont(14, L"Roboto", FW_BOLD, false, false, FONT_ROBOTO3);
		MyWindow::Initialize();
	};
	virtual void CreateWidgets() override
	{
		CreateStatic(L"static", L"Введите название БД --->", 20, 93, 300, 100, STAT_TEXT1, _fonts[FONT_ROBOTO2]);
		CreateControl(L"EDIT", _settings->_mainSettings.databaseName, 280, 90, 500, 25, EDIT_DBNAME, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, _fonts[FONT_ROBOTO1]);

		CreateStatic(L"static", L"Имя пользователя для БД --->", 20, 153, 300, 100, STAT_TEXT2, _fonts[FONT_ROBOTO2]);
		CreateControl(L"EDIT", _settings->_mainSettings.userName, 280, 150, 500, 25, EDIT_DBUSERNAME, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, _fonts[FONT_ROBOTO1]);

		CreateStatic(L"static", L"Пароль для БД --->", 20, 213, 300, 100, STAT_TEXT3, _fonts[FONT_ROBOTO2]);
		CreateControl(L"EDIT", _settings->_mainSettings.password, 280, 210, 500, 25, EDIT_DBPASSWORD, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_PASSWORD, _fonts[FONT_ROBOTO1]);

		CreateStatic(L"static", L"Адрес сервера --->", 20, 273, 300, 100, STAT_TEXT4, _fonts[FONT_ROBOTO2]);
		CreateControl(L"EDIT", _settings->_mainSettings.server, 280, 270, 500, 25, EDIT_DBSERVER, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, _fonts[FONT_ROBOTO1]);

		CreateControl(L"BUTTON", L"Главное окно", 20, 30, 150, 30, BTN_BACK, WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, _fonts[FONT_ROBOTO1]);
		CreateControl(L"BUTTON", L"Сохранить", 200, 350, 150, 30, BTN_SAVE, WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, _fonts[FONT_ROBOTO1]);
		CreateControl(L"BUTTON", L"Проверить подключение к БД", 360, 350, 300, 30, BTN_CONNECT, WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, _fonts[FONT_ROBOTO3]);

		//Устанавливаем роли для каждого элемента
		SetWidgetPermissions(BTN_BACK, WidgetState::Enabled, { eRole::ConnectedDB, eRole::Administrator, eRole::Manager, eRole::Analitic, eRole::Master});
		SetWidgetPermissions(EDIT_DBNAME, WidgetState::Enabled, { eRole::DisconnectDB, eRole::Administrator });
		SetWidgetPermissions(EDIT_DBUSERNAME, WidgetState::Enabled, { eRole::DisconnectDB, eRole::Administrator });
		SetWidgetPermissions(EDIT_DBPASSWORD, WidgetState::Enabled, { eRole::DisconnectDB, eRole::Administrator });
		SetWidgetPermissions(EDIT_DBSERVER, WidgetState::Enabled, { eRole::DisconnectDB, eRole::Administrator });
		SetWidgetPermissions(BTN_SAVE, WidgetState::Enabled, { eRole::Administrator });
		UpdateWidgetsState(); //Применяем установленные изменения
	}

	virtual void HandleButtonClick(int buttonId) override
	{
		switch (buttonId) {
		case BTN_SAVE:
			SaveSettings();
			break;
		case BTN_CONNECT:
			ConnectDB();
			break;
		case BTN_BACK:
			CreateOtherWindow(EUIMainWindow, SColor::BaseColor()); // Переход в главное окно
			break;
		}
	}

	virtual void UpdateWidgetSettings() override
	{
		UpdateControlText(EDIT_DBNAME, _settings->_mainSettings.databaseName);
		UpdateControlText(EDIT_DBUSERNAME, _settings->_mainSettings.userName);
		UpdateControlText(EDIT_DBPASSWORD, _settings->_mainSettings.password);
		UpdateControlText(EDIT_DBSERVER, _settings->_mainSettings.server);
	};
};