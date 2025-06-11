#pragma once
#include "MyWindow.h"
#include "MyGridButton.h"

#ifndef GET_X_LPARAM
#define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#endif

#ifndef GET_Y_LPARAM
#define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))
#endif

//Класс окна товаров
class UIListPartner : public MyWindow
{

private:
	enum EWidgetsID { // Идентификаторы элементов
		T_1 = 1,
		BTN_BACK,
		BTN_NEWPARTNER,
		GRID_LIST
	};

	enum EFontsID { // Идентификаторы шрифтов
		FONT_ROBOTO1 = 101,
		FONT_ROBOTO2 = 201,
		FONT_ROBOTO3 = 301
	};

	const int COLUMNS = 1;
	ScrollGridButtons* _scrollGrid = nullptr;
	vector<STable> _partners;
	int _idPartnerInfo = -1;

	void UpdateShowInfoPartners()
	{
		_partners = _dataBase->LoadTablePartner();
		if (_partners.empty()) return;

		for (int i = 0; i < _partners.size(); i++)
		{
			// Вычисляем позицию в матрице
			int row = i / COLUMNS;    // Номер строки
			int col = i % COLUMNS;    // Номер колонки

			MyButton* hButton = MyButton::Create().
				parent(_hwnd).
				text(StringToWide(_partners[i].tableLine)).
				coordSize(0, 0, 200, 100).
				id(1000+i).
				backGroundColor(SColor::AccentuationColor()).
				textColor(SColor()).
				font(_fonts[FONT_ROBOTO1]).
				outline(false).
				Build();

			_scrollGrid->AddButton(hButton, row, col);
		}
	}

public:
	UIListPartner(HINSTANCE hInstance, EWindowID prevWindow) : MyWindow(hInstance, prevWindow)
	{
		CreateClass<UIListPartner>(hInstance, L"ListPartner", 1024, 640);
		Initialize();
	}

	UIListPartner(HINSTANCE hInstance, EWindowID prevWindow, SColor color) : MyWindow(hInstance, prevWindow, color)
	{
		CreateClass<UIListPartner>(hInstance, L"ListPartner", 1024, 640);
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
			// Проверяем, находится ли курсор над нашим скроллом
			POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
			ScreenToClient(_hwnd, &pt);

			RECT scrollRect;
			GetWindowRect(_scrollGrid->GetScrollHandle(), &scrollRect);
			MapWindowPoints(HWND_DESKTOP, _hwnd, (LPPOINT)&scrollRect, 2);

			if (PtInRect(&scrollRect, pt)) {
				_scrollGrid->HandleMouseWheel(wParam);
				return 0;
			}
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
		CreateControl(L"BUTTON", L"Новый партнер", 560, 500, 200, 40, BTN_NEWPARTNER, WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, _fonts[FONT_ROBOTO2]);
		CreateControl(L"BUTTON", L"Назад", 300, 500, 200, 40, BTN_BACK, WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, _fonts[FONT_ROBOTO2]);

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
		case BTN_NEWPARTNER:
			CreateOtherWindow(EUIRegisterPartner, SColor::AdditionalColor());
			break;
		case BTN_BACK:
			CreateOtherWindow(EUIMainWindow, SColor::BaseColor());
			break;
		default:
			// Обработка кнопок в скролле
			if (buttonId >= 1000 && buttonId < 2000) {
				int buttonIndex = buttonId - 1000;
				_idPartnerInfo = _partners[buttonIndex].id;
				CreateOtherWindow(EUIPartnerInfo, SColor::AdditionalColor());
			}
			break;
		}
	}
	virtual void CreateOtherWindow(EWindowID id, SColor color = SColor(90, 75, 75))  override;
};