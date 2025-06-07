#pragma once
#include "MyWindow.h"

class UILogin : public MyWindow
{
private:
	enum EWidgetsID { // Идентификаторы элементов
		BTN_REGISTER = 1,
		BTN_LOGIN,
		BTN_BACK,
		EDIT_PASSWORD,
		EDIT_LOGIN,
		STAT_TEXT1,
		STAT_TEXT2,
		CHECK_REMEMBER
	};

	enum EFontsID // Идентификаторы шрифтов
	{
		FONT_ROBOTO1 = 1001,
		FONT_ROBOTO2,
		FONT_ROBOTO3
	};

	void Ligin()
	{
		LRESULT checkState = SendMessage(_widgets[CHECK_REMEMBER], BM_GETCHECK, 0, 0);
		bool isChecked = (checkState == BST_CHECKED);
		string password, login;
		GetWindowTextAsString(_widgets[EDIT_LOGIN], login);
		GetWindowTextAsString(_widgets[EDIT_PASSWORD], password);
		if (password.empty() || login.empty())
		{
			MessageBox(nullptr, L"Заполните все поля", L"Ошибка!", MB_ICONERROR);
			return;
		}
		_settings->_userSettings.login = StringToWide(login);
		_settings->_userSettings.password = StringToWide(_dataBase->SimpleHash(password));
		if (_dataBase->InLogin())
		{
			if (isChecked)
			{
				_settings->SaveSettings();
			}
			MessageBoxW(_hwnd, L"Вход выполнен!", L"Инфо", MB_OK);
			CreateOtherWindow(EUIMainWindow);
			return;
		}
		else MessageBox(nullptr, L"Неверный логин или пароль", L"Ошибка!", MB_ICONERROR);
		return;
	}

public:

	UILogin(HINSTANCE hInstance, EWindowID prevWindow) : MyWindow(hInstance, prevWindow)
	{
		CreateClass<UILogin>(hInstance, L"Login", 600, 350);
		Initialize();
	}

	UILogin(HINSTANCE hInstance, EWindowID prevWindow, SColor color) : MyWindow(hInstance, prevWindow, color)
	{
		CreateClass<UILogin>(hInstance, L"Login", 600, 350);
		Initialize();
	}

	virtual void Show(int nCmdShow)
	{
		MyWindow::Show(nCmdShow);
		if (_settings->_userSettings.inLogin == false)
		{
			if (_dataBase->HasAdministrator() == false)
			{
				SetWidgetPermissions(BTN_REGISTER, WidgetState::Enabled, { eRole::ConnectedDB });
				//CreateOtherWindow(EUIRegister, SColor::AdditionalColor());
			}
			else _settings->_userSettings.role = eRole::ConnectedDB;
		}
		UpdateWidgetsState();
	}

protected:
	virtual void Initialize() override
	{
		AddFont(16, L"Roboto", FW_BOLD, false, false, FONT_ROBOTO1);
		AddFont(12, L"Roboto", FW_BOLD, false, false, FONT_ROBOTO2);
		AddFont(14, L"Roboto", FW_BOLD, false, false, FONT_ROBOTO3);
		MyWindow::Initialize();
	};

	virtual void CreateWidgets() override
	{
		CreateStatic(L"static", L"Логин -->", 10, 72, 300, 100, STAT_TEXT1, _fonts[FONT_ROBOTO3]);
		CreateStatic(L"static", L"Пароль -->", 10, 122, 300, 100, STAT_TEXT2, _fonts[FONT_ROBOTO3]);
		CreateControl(L"EDIT", _settings->_userSettings.login, 120, 70, 450, 25, EDIT_LOGIN, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, _fonts[FONT_ROBOTO3]);
		CreateControl(L"EDIT", _settings->_userSettings.login, 120, 120, 450, 25, EDIT_PASSWORD, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_PASSWORD, _fonts[FONT_ROBOTO3]);
		CreateControl(L"BUTTON", L"Назад", 10, 15, 150, 30, BTN_BACK, WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, _fonts[FONT_ROBOTO1]);
		CreateControl(L"BUTTON", L"Регистрация", 130, 250, 150, 30, BTN_REGISTER, WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, _fonts[FONT_ROBOTO1]);
		CreateControl(L"BUTTON", L"Войти", 300, 250, 160, 30, BTN_LOGIN, WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, _fonts[FONT_ROBOTO1]);

		// Добавляем чекбокс "Запомнить меня"
		CreateControl(L"BUTTON", L"Запомнить меня", 390, 190, 200, 40, CHECK_REMEMBER, WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, _fonts[FONT_ROBOTO3]);

		SetWidgetPermissions(BTN_REGISTER, WidgetState::Enabled, { eRole::Administrator });
		SetWidgetPermissions(EDIT_LOGIN, WidgetState::Enabled, { eRole::ConnectedDB });
		SetWidgetPermissions(EDIT_PASSWORD, WidgetState::Enabled, { eRole::ConnectedDB });
		SetWidgetPermissions(BTN_LOGIN, WidgetState::Enabled, { eRole::ConnectedDB });
		SetWidgetPermissions(CHECK_REMEMBER, WidgetState::Enabled, { eRole::ConnectedDB });
		UpdateWidgetsState(); //Применяем установленные изменения
	}

	virtual void UpdateWidgetSettings() override {
		if (_settings->_userSettings.inLogin)
		{
			UpdateControlText(EDIT_LOGIN, _settings->_userSettings.login);
			UpdateControlText(EDIT_PASSWORD, _settings->_userSettings.password);
		}
		else
		{
			UpdateControlText(EDIT_LOGIN, L"");
			UpdateControlText(EDIT_PASSWORD, L"");
		}
	}

	virtual void HandleButtonClick(int buttonId) override
	{
		switch (buttonId)
		{
		case BTN_BACK:
			CreateOtherWindow(EUIMainWindow, SColor::BaseColor());
			break;
		case BTN_REGISTER:
			CreateOtherWindow(EUIRegister, SColor::AdditionalColor());
			break;
		case BTN_LOGIN:
			Ligin();
			break;
		}
		UpdateWidgetsState();
	}
};