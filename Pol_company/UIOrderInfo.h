#pragma once
#include "MyWindow.h"
#include "MyGridButton.h"
#include <string.h>

//Класс окна товаров
class UIOrderInfo : public MyWindow
{

private:
	enum EWidgetsID { // Идентификаторы элементов
		T_1 = 1,
		T_STATUS,
		BTN_BACK,
		BTN_APPLY,
		EDIT_CHANGERAITING,
		EDIT_INFO,
		BTN_DELETE,
		BOX_STATUS
	};

	enum EFontsID { // Идентификаторы шрифтов
		FONT_ROBOTO1 = 101,
		FONT_ROBOTO2 = 201,
		FONT_ROBOTO3 = 301
	};

	STable _currentItem;
	vector<STable> _status;

	void LoadOrderInfo()
	{
		// Добавление элементов в комбобокс
		if (_status.empty()) 
		{
			_status = _dataBase->LoadTableTitle("_status");
			for (int i = 0; i < _status.size(); i++)
				SendMessage(_widgets[BOX_STATUS], CB_INSERTSTRING, i, (LPARAM)StringToWide(_status[i].tableLine).c_str());
			SendMessage(_widgets[BOX_STATUS], CB_SETCURSEL, 0, 0);
		}

		vector<STable> tmp = _dataBase->LoadTableOrder(_currentItem.id);
		if (tmp.size() <= 0)
			UpdateControlText(EDIT_INFO, "Ошибка загрузки");
		else
		{
			_currentItem.tableLine = tmp[0].tableLine;
			UpdateControlText(EDIT_INFO, _currentItem.tableLine.c_str());
		}
	}

	void DeleteOrder()
	{
		int result = MessageBox(nullptr, L"Удалить заявку?", L"Удаление заявки", MB_YESNO | MB_ICONQUESTION);
		if (result == IDYES && _currentItem.id != -1)
			if (_dataBase->DeleteOrder(_currentItem.id, true)) 
			{
				MessageBoxW(_hwnd, L"Заявка удалена!", L"Инфо", MB_OK);
				CreateOtherWindow(EUIListOrder, SColor::BaseColor());
			}
	}

	void UpdateStatusOrder()
	{
		int index_status = SendMessage(_widgets[BOX_STATUS], CB_GETCURSEL, 0, 0);
		int status_id = _status[index_status].id;
		_dataBase->UpdateStatusOrder(status_id, _currentItem.id);
		UpdateWidgetSettings();
	}

public:
	UIOrderInfo(HINSTANCE hInstance, EWindowID prevWindow) : MyWindow(hInstance, prevWindow)
	{
		CreateClass<UIOrderInfo>(hInstance, L"Order", 1024, 640);
		Initialize();
	}

	UIOrderInfo(HINSTANCE hInstance, EWindowID prevWindow, SColor color) : MyWindow(hInstance, prevWindow, color)
	{
		CreateClass<UIOrderInfo>(hInstance, L"Order", 1024, 640);
		Initialize();
	}

	void SetIndexItem(int index)
	{
		_currentItem.id = index;
	}

protected:
	virtual void Initialize() override
	{
		AddFont(18, L"Roboto", FW_BOLD, true, false, FONT_ROBOTO1);
		AddFont(15, L"Roboto", FW_BOLD, true, false, FONT_ROBOTO2);
		AddFont(14, L"Roboto", FW_BOLD, false, false, FONT_ROBOTO3);
		CreateWidgets();
	}

	virtual void CreateWidgets()
	{
		CreateControl(L"BUTTON", L"Применить изменения", 400, 500, 250, 40, BTN_APPLY, WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, _fonts[FONT_ROBOTO3]);
		CreateControl(L"BUTTON", L"Назад", 140, 500, 150, 40, BTN_BACK, WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, _fonts[FONT_ROBOTO2]);
		CreateControl(L"BUTTON", L"Удалить", 740, 500, 150, 40, BTN_DELETE, WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, _fonts[FONT_ROBOTO2]);
		CreateControl(L"EDIT", L"Информация", 100, 30, 820, 200, EDIT_INFO,
			WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_VSCROLL | ES_NOHIDESEL | ES_READONLY, _fonts[FONT_ROBOTO2]);
		CreateStatic(L"static", L"Статус заявки -->", 100, 232, 300, 100, T_STATUS, _fonts[FONT_ROBOTO2]);
		CreateControl(L"COMBOBOX", L"", 300, 230, 250, 200, BOX_STATUS, WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL, _fonts[FONT_ROBOTO3]);

		SetWidgetPermissions(BTN_DELETE, WidgetState::Enabled, { eRole::Manager });
		SetWidgetPermissions(T_STATUS, WidgetState::Visible, { eRole::Master });
		SetWidgetPermissions(BOX_STATUS, WidgetState::Visible, { eRole::Master });
		SetWidgetPermissions(BTN_APPLY, WidgetState::Visible, { eRole::Master });
		UpdateWidgetsState();
	}

	virtual void HandleButtonClick(int buttonId) override
	{
		switch (buttonId)
		{
		case BTN_APPLY:
			UpdateStatusOrder();
			break;
		case BTN_BACK:
			CreateOtherWindow(EUIListOrder, SColor::BaseColor());
			break;
		case BTN_DELETE:
			DeleteOrder();
			break;
		}
	}

	virtual void UpdateWidgetSettings() override
	{
		LoadOrderInfo();
	};
};