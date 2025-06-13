#pragma once
#include "MyWindow.h"
#include "MyGridButton.h"

//Класс окна товаров
class UIListOrder : public MyWindow
{

private:
	enum EWidgetsID { // Идентификаторы элементов
		T_1 = 1,
		BTN_BACK,
		BTN_NEWORDER,
		GRID_LIST
	};

	enum EFontsID { // Идентификаторы шрифтов
		FONT_ROBOTO1 = 101,
		FONT_ROBOTO2 = 201,
		FONT_ROBOTO3 = 301
	};

	const int COLUMNS = 1;
	ScrollGridButtons* _scrollGrid = nullptr;
	vector<STable> _orders;
	int _idOrderInfo = -1;

	void UpdateShowInfoPartners()
	{
		_orders = _dataBase->LoadTableOrder();
		if (_orders.empty()) return;

		for (int i = 0; i < _orders.size(); i++)
		{
			// Вычисляем позицию в матрице
			int row = i / COLUMNS;    // Номер строки
			int col = i % COLUMNS;    // Номер колонки

			MyButton* hButton = MyButton::Create().
				parent(_hwnd).
				text(StringToWide(_orders[i].tableLine)).
				coordSize(0, 0, 200, 100).
				id(1000 + i).
				backGroundColor(SColor::AccentuationColor()).
				textColor(SColor()).
				font(_fonts[FONT_ROBOTO1]).
				outline(false).
				Build();

			_scrollGrid->AddButton(hButton, row, col);
		}
	}

public:
	UIListOrder(HINSTANCE hInstance, EWindowID prevWindow) : MyWindow(hInstance, prevWindow)
	{
		CreateClass<UIListOrder>(hInstance, L"ListOrder", 1024, 640);
		Initialize();
	}

	UIListOrder(HINSTANCE hInstance, EWindowID prevWindow, SColor color) : MyWindow(hInstance, prevWindow, color)
	{
		CreateClass<UIListOrder>(hInstance, L"ListOrder", 1024, 640);
		Initialize();
	}

	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override {
		switch (uMsg) {
		case WM_VSCROLL:
			if ((HWND)lParam == _scrollGrid->GetScrollHandle()) {
				_scrollGrid->HandleScroll(wParam);
				return 0;
			}
			break;
		case WM_MOUSEWHEEL:
		{
			_scrollGrid->HandleMouseWheel(wParam);
			return 0;
			break;
		}
		}
		return MyWindow::HandleMessage(uMsg, wParam, lParam);
	}

	virtual void Show(int nCmdShow)
	{
		MyWindow::Show(nCmdShow);
		UpdateShowInfoPartners();
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
		CreateControl(L"BUTTON", L"Новая заявка", 560, 500, 200, 40, BTN_NEWORDER, WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, _fonts[FONT_ROBOTO2]);
		CreateControl(L"BUTTON", L"Назад", 300, 500, 200, 40, BTN_BACK, WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, _fonts[FONT_ROBOTO2]);

		SetWidgetPermissions(BTN_NEWORDER, WidgetState::Enabled, { eRole::Manager });
		UpdateWidgetsState();

		// Создаем скролл с гридом кнопок
		_scrollGrid = new ScrollGridButtons(_hwnd, 950, 60, 20, 400, 850, 50, COLUMNS, 10);
		_scrollGrid->MoveTo(80, 60);


		/*ScrollGridButtons* _scrollGrid = ScrollGridButtons::Create().
			parent(_hwnd).
			coordSize(0, 0, 200, 100).
			coordSizeButton(80, 50, 200, 50).
			Column(COLUMNS).
			Spacing(10).
			Build();*/
	}

	virtual void HandleButtonClick(int buttonId) override
	{
		switch (buttonId)
		{
		case BTN_NEWORDER:
			CreateOtherWindow(EUINewOrder, SColor::AdditionalColor());
			break;
		case BTN_BACK:
			CreateOtherWindow(EUIMainWindow, SColor::BaseColor());
			break;
		default:
			// Обработка кнопок в скролле
			if (buttonId >= 1000 && buttonId < 2000) {
				int buttonIndex = buttonId - 1000;
				_idOrderInfo = _orders[buttonIndex].id;
				CreateOtherWindow(EUIOrderInfo, SColor::AdditionalColor());
			}
			break;
		}
	}
	virtual void CreateOtherWindow(EWindowID id, SColor color = SColor(90, 75, 75))  override;
};