#pragma once
#include "MyWindow.h"

//����� �������� ����
class UIMainWindow : public MyWindow
{
private:
	enum EWidgetsID { // �������������� ���������
		BTN_SETTINGS = 100,
		BTN_INFO = 200,
		BTN_EXIT = 300,
		BTN_STATUS = 400,
	};

	enum EFontsID // �������������� �������
	{
		FONT_ROBOTO1 = 101,
		FONT_ROBOTO2 = 201
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
		{
			MessageBoxW(_hwnd, L"���������������� �������� ������� ���", L"����", MB_OK);
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