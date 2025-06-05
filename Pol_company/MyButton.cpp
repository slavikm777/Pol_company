#include "MyButton.h"

void MyButton::SetBackgroundColor(const SColor& newColor)
{
	_bgColor = newColor;
	UpdateBrush();
	InvalidateRect(_hWnd, NULL, TRUE);
}

void MyButton::SetColorText(const SColor& newColor)
{
	_textColor = newColor;
	UpdateBrush();
	InvalidateRect(_hWnd, NULL, TRUE);
}

void MyButton::SetText(const wstring& newText)
{
	_text = newText;
	Redraw();
}

void MyButton::DrawButton(DRAWITEMSTRUCT* pDraw)
{
	if (!pDraw || !pDraw->hDC || !pDraw->hwndItem) return;

	HDC hdc = pDraw->hDC;
	RECT rc = pDraw->rcItem;

	HFONT hOldFont = nullptr;
	if (_hFont) {
		hOldFont = static_cast<HFONT>(SelectObject(hdc, _hFont));
	}

	HBRUSH hBgBrush = CreateSolidBrush(RGB(_bgColor.R, _bgColor.G, _bgColor.B));
	FillRect(hdc, &rc, hBgBrush);
	DeleteObject(hBgBrush);
	hBgBrush = nullptr;

	SetTextColor(hdc, RGB(_textColor.R, _textColor.G, _textColor.B));
	SetBkMode(hdc, TRANSPARENT);

	if (!_text.empty()) {
		DrawTextW(hdc, _text.c_str(), -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}

	if (_outLine)
	{
		FrameRect(hdc, &rc, (HBRUSH)GetStockObject(BLACK_BRUSH));
		if (pDraw->itemState & ODS_SELECTED) {
			InflateRect(&rc, -1, -1);
			DrawTextW(hdc, _text.c_str(), -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
	}

	if (_hFont && hOldFont) {
		SelectObject(hdc, hOldFont);
	}
}

void MyButton::Redraw()
{
	InvalidateRect(_hWnd, nullptr, TRUE);
	UpdateWindow(_hWnd);
}

void MyButton::UpdateBrush()
{
	if (_hBrush) DeleteObject(_hBrush);
	_hBrush = CreateSolidBrush(RGB(_bgColor.R, _bgColor.G, _bgColor.B));
}

void MyButton::CreateButton()
{
	if (_hWnd) DestroyWindow(_hWnd);
	_hWnd = CreateWindowExW(0,
		L"BUTTON",
		_text.c_str(),
		WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
		_x, _y, _width, _height,
		_parent,
		reinterpret_cast<HMENU>(static_cast<INT_PTR>(_id)),
		GetModuleHandle(nullptr),
		nullptr);

	SetWindowLongPtrW(_hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
}
