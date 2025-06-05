#include "MyWindow.h"
#include "FactoryWindows.h"

HFONT MyWindow::CreateFontEx(int size, const wstring& name, int weight, bool italic, bool underline)
{
	HDC hdc = GetDC(_hwnd);
	HFONT tmpFont = CreateFontW(
		-MulDiv(size, GetDeviceCaps(hdc, LOGPIXELSY), 72),
		0, 0, 0,
		weight,
		italic,
		underline,
		FALSE,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		name.c_str()
	);
	ReleaseDC(_hwnd, hdc);
	return tmpFont;
}

void MyWindow::SetWidgetPermissions(int widgetId, WidgetState state, initializer_list<eRole> roles)
{
	_widgetPermissions[widgetId] = { unordered_set<eRole>(roles), state };
}

void MyWindow::UpdateWidgetsState()
{
	bool hasAccess;
	for (const auto& widgetPair : _widgets) {
		int widgetId = widgetPair.first;
		HWND hWnd = widgetPair.second;
		auto& perms = _widgetPermissions[widgetId];
		hasAccess = perms.roles.count(_settings->_userSettings.role) > 0;

		switch (_widgetPermissions[widgetId].state)
		{
		case Enabled:
			EnableWindow(hWnd, (hasAccess && (perms.state & Enabled)) ? TRUE : FALSE);
			break;
		case Visible:
			ShowWindow(hWnd, (hasAccess && (perms.state & Visible)) ? SW_SHOW : SW_HIDE);
			break;
		}
	}
}

void MyWindow::SwitchToWindow(MyWindow& target)
{
	if (target.GetHandle()) {
		ShowWindow(_hwnd, SW_HIDE);
		target.Show(SW_SHOW);
		SetForegroundWindow(target.GetHandle());
	}
}

string MyWindow::WideToUTF8(const wchar_t* wideStr)
{
	int utf8Size = WideCharToMultiByte(CP_UTF8, 0, wideStr, -1, nullptr, 0, nullptr, nullptr);
	string result(utf8Size, 0);
	WideCharToMultiByte(CP_UTF8, 0, wideStr, -1, &result[0], utf8Size, nullptr, nullptr);
	return result.c_str();
}

void MyWindow::AddFont(int size, const wstring& name, int weight, bool italic, bool underline, int id)
{
	if (_fonts.count(id)) {
		DeleteObject(_fonts[id]);
	}
	HFONT tmpFont = CreateFontEx(size, name, weight, italic, underline);
	_fonts[id] = tmpFont;
}

void MyWindow::AddMenu(HMENU hParentMenu, const wstring& name, UINT id, HMENU hSubMenu)
{
	if (hSubMenu) {
		AppendMenuW(hParentMenu, MF_POPUP, reinterpret_cast<UINT_PTR>(hSubMenu), name.c_str());
	}
	else {
		AppendMenuW(hParentMenu, MF_STRING, id, name.c_str());
	}
}

HWND MyWindow::CreateControl(const wstring& className, const wstring& text, int x, int y, int w, int h, int id, DWORD styles, HFONT font)
{
	if (_widgets.count(id)) {
		DestroyWindow(_widgets[id]);
	}
	HWND hControl = CreateWindowExW(0,
		className.c_str(),
		text.c_str(),
		styles,
		x, y, w, h,
		_hwnd,
		reinterpret_cast<HMENU>(static_cast<INT_PTR>(id)),
		reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(_hwnd, GWLP_HINSTANCE)),
		nullptr);

	if (font)
		SendMessage(hControl, WM_SETFONT, reinterpret_cast<WPARAM>(font), TRUE);
	_widgets[id] = hControl;
	return hControl;
}

HWND MyWindow::CreateStatic(const wstring& className, const wstring& text, int x, int y, int w, int h, int id, HFONT font)
{
	HWND hStatic = CreateWindowW(className.c_str(), text.c_str(),
		WS_VISIBLE | WS_CHILD,
		x, y, w, h,
		_hwnd,
		reinterpret_cast<HMENU>(static_cast<INT_PTR>(id)),
		reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(_hwnd, GWLP_HINSTANCE)),
		nullptr);

	if (font)
		SendMessageW(hStatic, WM_SETFONT, reinterpret_cast<WPARAM>(font), TRUE);
	_widgets[id] = hStatic;
	return hStatic;
}

void MyWindow::CreateCustomButton(const HWND& parent, const wstring& text, int x, int y, int w, int h, int id, SColor bgColor, SColor textColor, HFONT font, bool OutLine)
{
	auto it = _customButtons.find(id);
	if (it != _customButtons.end()) {
		delete it->second;
		_customButtons.erase(it);
	}

	MyButton* hButton = MyButton::Create().
		parent(parent).
		text(text).
		coordSize(x, y, w, h).
		id(id).
		backGroundColor(bgColor).
		textColor(textColor).
		font(font).
		outline(OutLine).
		Build();

	_widgets[id] = hButton->GetHandle();
	_customButtons[id] = hButton;
}

wstring MyWindow::FloatToWString(float value, int precision)
{
	bool isNegative = value < 0;
	value = abs(value);

	float multiplier = pow(10, precision);
	int total = static_cast<int>(round(value * multiplier));
	int intPart = total / multiplier;
	int fracPart = total % static_cast<int>(multiplier);
	wstring result = (isNegative ? L"-" : L"") + to_wstring(intPart) + L".";

	if (fracPart == 0) {
		result += wstring(precision, L'0');
	}
	else {
		wstring fracStr = to_wstring(fracPart);
		while (fracStr.length() < precision) {
			fracStr = L"0" + fracStr;
		}
		result += fracStr;
	}
	return result;
}

wstring MyWindow::StringToWide(const string& str)
{
	if (str.empty()) return L"";
	int size = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
	if (size == 0) {
		size = MultiByteToWideChar(1251, 0, str.c_str(), -1, nullptr, 0);
		if (size == 0) return L"";
	}
	wstring wstr(size - 1, 0);
	MultiByteToWideChar(size == 0 ? 1251 : CP_UTF8, 0, str.c_str(), -1, &wstr[0], size);
	return wstr;
}

void MyWindow::UpdateControlText(int controlId, const string& newText)
{
	if (_widgets.find(controlId) != _widgets.end()) {
		wstring wtext = StringToWide(newText);
		SetWindowTextW(_widgets[controlId], wtext.c_str());
	}
}

void MyWindow::UpdateControlText(int controlId, const wstring& newText)
{
	if (_widgets.find(controlId) != _widgets.end()) {
		SetWindowTextW(_widgets[controlId], newText.c_str());
	}
}

void MyWindow::GetWindowTextAsString(HWND hWnd, string& output)
{
	int length = GetWindowTextLengthW(hWnd) + 1;
	wstring wideText(length, L'\0');
	GetWindowTextW(hWnd, &wideText[0], length);
	output = WideToUTF8(wideText.c_str());
}

void MyWindow::GetWindowTextAsStringANSI(HWND hWnd, string& output)
{
	int length = GetWindowTextLengthW(hWnd);
	wstring wideText(length + 1, L'\0');
	GetWindowTextW(hWnd, &wideText[0], length + 1);
	output = WideToANSI(wideText.c_str());
}

string MyWindow::WideToANSI(const wchar_t* wideStr)
{
	// Конвертация из WideChar (UTF-16) в ANSI (CP_ACP = активная кодовая страница консоли)
	int ansiSize = WideCharToMultiByte(1251, 0, wideStr, -1, nullptr, 0, nullptr, nullptr);
	string result(ansiSize - 1, '\0');  // -1 чтобы убрать нуль-терминатор

	WideCharToMultiByte(
		1251,                // Кодовая страница 1251 (русская)
		0,                   // Флаги
		wideStr,             // Исходная строка
		-1,                  // Длина до нуль-терминатора
		&result[0],          // Выходной буфер
		ansiSize,            // Размер буфера
		nullptr,
		nullptr
	);

	return result;
}

void MyWindow::AppendTextToEdit(HWND hEdit, const wchar_t* text)
{
	int outLength = GetWindowTextLengthW(hEdit);
	SendMessageW(hEdit, EM_SETSEL, (WPARAM)outLength, (LPARAM)outLength);
	SendMessageW(hEdit, EM_REPLACESEL, TRUE, (LPARAM)text);
}

void MyWindow::CreateOtherWindow(EWindowID id, SColor color)
{
	// Закрываем предыдущее окно если оно существует
	if (_otherWindow) {
		MyWindow* old = _otherWindow;
		_otherWindow = nullptr;
		old->Close(); // Самоудаление
	}

	FactoryWindow factory;
	_otherWindow = factory.Create(id, GetInstance(), color);

	if (_otherWindow && _otherWindow->GetHandle()) {
		ShowWindow(_hwnd, SW_HIDE);
		_otherWindow->Show(SW_SHOW);
		SetForegroundWindow(_otherWindow->GetHandle());
	}
}

MyWindow::MyWindow(HINSTANCE hInstance, EWindowID windowId, SColor color)
{
	_currentWindowId = windowId;
	_color = color;
	CreateClass<MyWindow>(hInstance, L"MyWindowsClass");
	Initialize();
}

MyWindow::~MyWindow()
{
	for (auto& pair : _customButtons) {
		delete pair.second;
	}
	_customButtons.clear();
	for (auto& pair : _fonts) {
		DeleteObject(pair.second);
	}
	_fonts.clear();
	_widgets.clear();
	_widgetPermissions.clear();
	if (_hBrush) {
		DeleteObject(_hBrush);
		_hBrush = nullptr;
	}
	if (IsWindow(_hwnd)) {
		DestroyWindow(_hwnd);
	}
}

void MyWindow::Close()
{
	if (_hwnd)
	{
		DestroyWindow(_hwnd);
		_hwnd = nullptr;
	}
}

LRESULT MyWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
		//Обработка цвета кнопок
	case WM_DRAWITEM: {
		DRAWITEMSTRUCT* pDraw = reinterpret_cast<DRAWITEMSTRUCT*>(lParam);
		if (pDraw->CtlType == ODT_BUTTON) {
			MyButton* pButton = reinterpret_cast<MyButton*>(
				GetWindowLongPtr(pDraw->hwndItem, GWLP_USERDATA)
				);
			if (pButton) {
				pButton->DrawButton(pDraw);
				return TRUE;
			}
		}
		break;
	}
	case WM_COMMAND: {
		// Обработка кнопок и меню
		if (lParam) { // Уведомление от контрола
			if (HIWORD(wParam) == BN_CLICKED) {
				HandleButtonClick(LOWORD(wParam));
			}
		}
		else { // Уведомление от меню
			HandleMenuCommand(LOWORD(wParam));
		}
		return 0;
	}
	case WM_CTLCOLORSTATIC: {
		// Установка цвета текста для статических элементов
		//HDC hdc = reinterpret_cast<HDC>(wParam);
		//SetTextColor(hdc, RGB(_color.R, _color.G, _color.B));
		return reinterpret_cast<LRESULT>(_hBrush);
	}
	case WM_DESTROY:
		if (!IsMainWindow()) {
			delete this; // Самоуничтожение объекта
		}
		return 0;
	default:
		return DefWindowProcW(_hwnd, uMsg, wParam, lParam);
	}
}

void MyWindow::Show(int nCmdShow)
{
	UpdateWidgetsState();
	UpdateWidgetSettings();
	ShowWindow(_hwnd, nCmdShow);
	UpdateWindow(_hwnd);
}
