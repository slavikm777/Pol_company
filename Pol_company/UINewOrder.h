#pragma once
#include "MyWindow.h"
#include "vector"

class UINewOrder : public MyWindow
{
private:
	enum EWidgetsID { // Идентификаторы элементов
		BTN_CREATE = 1,
		BTN_BACK,
		COMBOBOX_PARTNER,
		COMBOBOX_PRODUCT,
		T_PARTNER,
		T_PRODUCT,
		T_COUNT,
		EDIT_COUNT
	};

	enum EFontsID // Идентификаторы шрифтов
	{
		FONT_ROBOTO1 = 1001,
		FONT_ROBOTO2,
		FONT_ROBOTO3,
		FONT_ROBOTO4
	};

	vector<STable> _partners;
	vector<STable> _products;

	void Create()
	{
		string totalAmount;
		int index_partner = SendMessage(_widgets[COMBOBOX_PARTNER], CB_GETCURSEL, 0, 0);
		int index_product = SendMessage(_widgets[COMBOBOX_PRODUCT], CB_GETCURSEL, 0, 0);
		int partner_id = _partners[index_partner].id;
		int product_id = _products[index_product].id;
		GetWindowTextAsString(_widgets[EDIT_COUNT], totalAmount);

		if (totalAmount.empty())
		{
			MessageBox(nullptr, L"Заполните все поля", L"Ошибка!", MB_ICONERROR);
			return;
		}
		int user_id = _dataBase->GetIdUser(WideToUTF8(_settings->_userSettings.login.c_str()));
		
		if(_dataBase->CreateOrder(partner_id, user_id, product_id, 1, totalAmount, true))
		{
			MessageBoxW(_hwnd, L"Заявка оформлена!", L"Инфо", MB_OK);
			CreateOtherWindow(EUIListOrder, SColor::AdditionalColor());
		}
	}

public:
	UINewOrder(HINSTANCE hInstance, EWindowID prevWindow) : MyWindow(hInstance, prevWindow)
	{
		CreateClass<UINewOrder>(hInstance, L"NewOrder", 1024, 640);
		Initialize();
	}

	UINewOrder(HINSTANCE hInstance, EWindowID prevWindow, SColor color) : MyWindow(hInstance, prevWindow, color)
	{
		CreateClass<UINewOrder>(hInstance, L"NewOrder", 1024, 640);
		Initialize();
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
		CreateStatic(L"static", L"Кому -->", 10, 72, 300, 100, T_PARTNER, _fonts[FONT_ROBOTO3]);
		CreateStatic(L"static", L"Продукция ->", 10, 122, 300, 100, T_PRODUCT, _fonts[FONT_ROBOTO2]);
		CreateStatic(L"static", L"Количество -->", 10, 172, 300, 100, T_COUNT, _fonts[FONT_ROBOTO2]);
		CreateControl(L"EDIT", L"", 160, 170, 100, 25, EDIT_COUNT, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, _fonts[FONT_ROBOTO3]);
		CreateControl(L"COMBOBOX", L"", 160, 72, 790, 200, COMBOBOX_PARTNER, WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL, _fonts[FONT_ROBOTO3]);
		CreateControl(L"COMBOBOX", L"", 160, 122, 790, 200, COMBOBOX_PRODUCT, WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL, _fonts[FONT_ROBOTO3]);

		CreateControl(L"BUTTON", L"Назад", 20, 20, 150, 30, BTN_BACK, WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, _fonts[FONT_ROBOTO1]);
		CreateControl(L"BUTTON", L"Создать", 430, 500, 200, 40, BTN_CREATE, WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, _fonts[FONT_ROBOTO4]);

	}

	virtual void UpdateWidgetSettings() override
	{
		// Добавление элементов в комбобокс
		_partners = _dataBase->LoadTableTitle("_partners");
		for (int i = 0; i < _partners.size(); i++)
			SendMessage(_widgets[COMBOBOX_PARTNER], CB_INSERTSTRING, i, (LPARAM)StringToWide(_partners[i].tableLine).c_str());
		SendMessage(_widgets[COMBOBOX_PARTNER], CB_SETCURSEL, 0, 0);

		_products = _dataBase->LoadTableTitle("_product");
		for (int i = 0; i < _products.size(); i++)
			SendMessage(_widgets[COMBOBOX_PRODUCT], CB_INSERTSTRING, i, (LPARAM)StringToWide(_products[i].tableLine).c_str());
		SendMessage(_widgets[COMBOBOX_PRODUCT], CB_SETCURSEL, 0, 0);
	}

	virtual void HandleButtonClick(int buttonId) override
	{
		switch (buttonId)
		{
		case BTN_BACK:
			CreateOtherWindow(EUIListOrder, SColor::AdditionalColor());
			break;
		case BTN_CREATE:
			Create();
			break;
		}
		UpdateWidgetsState();
	}
};