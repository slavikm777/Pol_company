#pragma once
#include "MyWindow.h"
#include "MyGridButton.h"
#include <string.h>

//Класс окна товаров
class UIPartnerInfo : public MyWindow
{

private:
	enum EWidgetsID { // Идентификаторы элементов
		T_1 = 1,
		T_2,
		BTN_BACK,
		BTN_APPLY,
		EDIT_CHANGERAITING,
		EDIT_INFO,
		BTN_DELETE,
		EDIT_RAITING
	};

	enum EFontsID { // Идентификаторы шрифтов
		FONT_ROBOTO1 = 101,
		FONT_ROBOTO2 = 201,
		FONT_ROBOTO3 = 301
	};

	STable _currentItem;

	void LoadPartnerInfo()
	{
		vector<STable> tmp = _dataBase->LoadTablePartner(_currentItem.id);
		if (tmp.size() <= 0)
			UpdateControlText(EDIT_INFO, "Ошибка загрузки");
		else
		{
			_currentItem.tableLine = tmp[0].tableLine;
			UpdateControlText(EDIT_INFO, _currentItem.tableLine.c_str());
		}
	}

	void DeletePartner() 
	{
		int result = MessageBox(nullptr, L"Удалить партнера?", L"Удаление партнера", MB_YESNO | MB_ICONQUESTION);
		if (result == IDYES && _currentItem.id != -1)
			if(_dataBase->DeletePartner(_currentItem.id, true))
				CreateOtherWindow(EUIListPartner, SColor::BaseColor());
	}

	void UpdateRating()
	{
		string rating;
		GetWindowTextAsString(_widgets[EDIT_RAITING], rating);
		if (rating.empty() && _currentItem.id != -1)
		{
			MessageBox(nullptr, L"Заполните все поля", L"Ошибка!", MB_ICONERROR);
			return;
		}
		_dataBase->UpdateRating(rating, _currentItem.id);
		UpdateWidgetSettings();
	}

public:
	UIPartnerInfo(HINSTANCE hInstance, EWindowID prevWindow) : MyWindow(hInstance, prevWindow)
	{
		CreateClass<UIPartnerInfo>(hInstance, L"Partner", 1024, 640);
		Initialize();
	}

	UIPartnerInfo(HINSTANCE hInstance, EWindowID prevWindow, SColor color) : MyWindow(hInstance, prevWindow, color)
	{
		CreateClass<UIPartnerInfo>(hInstance, L"Partner", 1024, 640);
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
		CreateStatic(L"static", L"Рейтинг -->", 100, 332, 300, 100, T_2, _fonts[FONT_ROBOTO2]);
		CreateControl(L"EDIT", L"", 250, 330, 150, 25, EDIT_RAITING, WS_CHILD | WS_VISIBLE | ES_NUMBER, _fonts[FONT_ROBOTO3]);
	}

	virtual void HandleButtonClick(int buttonId) override
	{
		switch (buttonId)
		{
		case BTN_APPLY:
			UpdateRating();
			break;
		case BTN_BACK:
			CreateOtherWindow(EUIListPartner, SColor::BaseColor());
			break;
		case BTN_DELETE:
			DeletePartner();
			break;
		}
	}

	virtual void UpdateWidgetSettings() override
	{
		LoadPartnerInfo();
	};
};