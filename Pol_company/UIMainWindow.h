#pragma once
#include "MyWindow.h"

//����� �������� ����
class UIMainWindow : public MyWindow
{
private:
	enum EWidgetsID { // �������������� ���������
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

	enum EFontsID // �������������� �������
	{
		FONT_ROBOTO1 = 1001,
		FONT_ROBOTO2
	};

	wstring RoleToText(eRole x)
	{
		switch (x)
		{
		case Administrator:return L"�������������";
			break;
		case Manager: return L"��������";
			break;
		case Master: return L"������";
			break;
		case Analitic: return L"��������";
			break;
		case ConnectedDB: return L"��������� � ��";
			break;
		case DisconnectDB: return L"��� ����������� � ��";
			break;
		}
	}

	void LogOut()
	{
		int result = MessageBox(nullptr, L"�����?", L"����� �� ������� ������", MB_YESNO | MB_ICONQUESTION);
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

		AppendMenu(SubMenu, MF_STRING, BTN_SETTINGS, L"��������� ����������� ��");
		AppendMenu(SubMenu, MF_STRING, BTN_INFO, L"���������� � �������");
		AppendMenu(SubMenu, MF_SEPARATOR, NULL, NULL);
		AppendMenu(SubMenu, MF_STRING, BTN_EXIT, L"�����");

		AppendMenu(RootMenu, MF_POPUP, (UINT_PTR)SubMenu, L"������");
		SetMenu(_hwnd, RootMenu);
	}

	virtual void HandleMenuCommand(int menuId) override
	{
		switch (menuId) {
		case BTN_INFO:
			MessageBoxW(_hwnd, L"���������������� �������� ������� ���", L"����", MB_OK);
			break;
		case BTN_EXIT: 
			Close(); 
			break;
		case BTN_SETTINGS:
			CreateOtherWindow(EUISettings, SColor::AdditionalColor()); // ������� � ���� ��������
			break;
		}
	}

	virtual void CreateWidgets()
	{
		CreateCustomButton(_hwnd, RoleToText(_settings->_userSettings.role), 750, 10, 250, 30, BTN_STATUS, 
			SColor::AccentuationColor(), SColor(), _fonts[FONT_ROBOTO2]);
		CreateCustomButton(_hwnd, L"�����������", 800, 50, 180, 40, BTN_LOGIN, SColor::AccentuationColor(), SColor(), _fonts[FONT_ROBOTO2]);
		CreateCustomButton(_hwnd, L"�����", 800, 50, 180, 40, BTN_LOGOUT, SColor(255, 0, 130), SColor(), _fonts[FONT_ROBOTO2]);
		CreateCustomButton(_hwnd, L"����������� �������������", 370, 150, 320, 40, BTN_USERS, SColor::AccentuationColor(), SColor(), _fonts[FONT_ROBOTO2], false);
		CreateCustomButton(_hwnd, L"������", 370, 150, 320, 40, BTN_CREATE_ORDER, SColor::AccentuationColor(), SColor(), _fonts[FONT_ROBOTO2], false);
		CreateCustomButton(_hwnd, L"��������", 370, 200, 320, 40, BTN_PARTNERS, SColor::AccentuationColor(), SColor(), _fonts[FONT_ROBOTO2], false);

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
			CreateOtherWindow(EUILogin, SColor::AdditionalColor()); // ������� � ���� �����������
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