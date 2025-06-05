#pragma once
#include "MyWindow.h"

//Класс базового окна
class UIMainWindow : public MyWindow
{
private:
	enum EWidgetsID { // Идентификаторы элементов
		BTN_SETTINGS = 100,
		BTN_INFO = 200,
		BTN_EXIT = 300,
		BTN_STATUS = 400,
	};

	enum EFontsID // Идентификаторы шрифтов
	{
		FONT_ROBOTO1 = 101,
		FONT_ROBOTO2 = 201
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

protected:
	virtual void Initialize() override
	{
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
		{
			MessageBoxW(_hwnd, L"Производственная компания «Мастер пол»", L"Инфо", MB_OK);
			break;
		}
		case BTN_EXIT: Close(); break;
		}
	}

	virtual void CreateWidgets()
	{
		CreateCustomButton(_hwnd, RoleToText(_settings->_userSettings.role), 750, 10, 250, 30, BTN_STATUS, 
			SColor(240, 10, 10), SColor(), _fonts[FONT_ROBOTO2]);
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
			GetCustomButton(BTN_STATUS)->SetBackgroundColor(SColor(0, 240, 130));
			break;
		}
		GetCustomButton(BTN_STATUS)->SetText(RoleToText(_settings->_userSettings.role));
	}

	bool IsMainWindow() const override { return true; }
};