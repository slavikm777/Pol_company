#pragma once
#include "Windows.h"
#include <vector>
#include <string>
#include <codecvt>
#include <map>
#include <unordered_set>
#include "CoreTypes.h"
#include "MyButton.h"
#include "Settings.h"
#include "DBLayer.h"

using namespace std;

enum WidgetState {
	Enabled = 102, //��������� �����������
	Visible = 202, //��������� ����������
	Default = Enabled | Visible
};

// ����������� ����� �������� ����
class MyWindow {

private:
	const int minusIntensity = 50;
	HINSTANCE _hInstance = nullptr;
	struct WidgetPermissions {
		unordered_set<eRole> roles;
		WidgetState state;
	};
	SColor _color;
	HBRUSH _hBrush = nullptr;
	map<int, WidgetPermissions> _widgetPermissions;
	EWindowID _currentWindowId = EWindowID::EUIMainWindow;

	const wchar_t* GetWCharPtrFromWString(const wstring& wstr) { return wstr.c_str(); }

	template<typename T>
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		T* pThis = nullptr;

		switch (uMsg)
		{

		case WM_NCCREATE:
		{
			CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
			pThis = reinterpret_cast<T*>(pCreate->lpCreateParams);
			SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
			pThis->_hwnd = hwnd;
		} break;

		case WM_CLOSE:
		{
			DestroyWindow(hwnd);
			return 0;
		}
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		default:
			pThis = reinterpret_cast<T*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
			break;
		}

		return pThis ? pThis->HandleMessage(uMsg, wParam, lParam) : DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	HFONT CreateFontEx(int size, const wstring& name = L"Arial", int weight = FW_NORMAL, bool italic = false, bool underline = false);

protected:
	HWND _hwnd;
	MyWindow* _otherWindow = nullptr;
	map<int, HFONT> _fonts; //Map ��������� �������
	map<int, HWND> _widgets; //��� �������� ������������ � ����������� �������
	map<int, MyButton*> _customButtons;
	Settings* _settings = Settings::GetInstance(); //�������� ��������
	DataBase* _dataBase = DataBase::GetInstance(); //�������� ��������� ��

	HINSTANCE GetInstance() const { return _hInstance; }

	//������������� ��������� ����� �������
	void SetWidgetPermissions(int widgetId, WidgetState state, initializer_list<eRole> roles);

	//��������� ��������� �������� � ���� � ����������� �� ����
	virtual void UpdateWidgetsState();

	virtual void Initialize() // ������������� ����� ����������
	{
		CreateWidgets();
		//_dataBase->SetParentWindow(_hwnd);
	}
	virtual void CreateWidgets() {} //���������� ��������
	virtual void HandleButtonClick(int buttonId) {} //���������� ������
	virtual void MainWndAddMenues() {} //���������� �������
	virtual void HandleMenuCommand(int menuId) {} //���������� ������� �������
	virtual void UpdateWidgetSettings() {} // ������� ���������� ��������
	virtual bool IsMainWindow() const { return false; }

	void SwitchToWindow(MyWindow& target);

	template<typename T>
	void CreateClass(HINSTANCE hInstance, wstring ClassName, int width = 100, int height = 100)
	{
		const wchar_t* CLASS_NAME = GetWCharPtrFromWString(ClassName);
		if (!_hBrush) {
			_hBrush = CreateSolidBrush(RGB(_color.R - minusIntensity, _color.G - minusIntensity, _color.B - minusIntensity));
		}
		WNDCLASS wc = {};
		wc.lpfnWndProc = WindowProc<T>;
		wc.hInstance = hInstance;
		wc.lpszClassName = CLASS_NAME;
		wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wc.hbrBackground = _hBrush;

		RegisterClass(&wc);
		_hInstance = hInstance;
		_hwnd = CreateWindowEx(0, CLASS_NAME, CLASS_NAME,
			WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
			width, height, nullptr, nullptr, hInstance, this);
	}

	string WideToUTF8(const wchar_t* wideStr);

	//���������� ������ � map
	void AddFont(int size, const wstring& name = L"Arial", int weight = FW_NORMAL,
		bool italic = false, bool underline = false, int id = 0);

	// ������� ������ ��� ������ � �����������
	void AddMenu(HMENU hParentMenu, const wstring& name, UINT id = 0, HMENU hSubMenu = NULL);

	//�������� ��������� ����������
	HWND CreateControl(const wstring& className, const wstring& text,
		int x, int y, int w, int h, int id, DWORD styles, HFONT font = nullptr);

	//�������� ��������� ���������
	HWND CreateStatic(const wstring& className, const wstring& text,
		int x, int y, int w, int h, int id, HFONT font = nullptr);

	//���������� ����� ������
	void CreateCustomButton(const HWND& parent, const wstring& text, int x, int y, int w, int h, int id,
		SColor bgColor, SColor textColor, HFONT font, bool OutLine = true);

	MyButton* GetCustomButton(int id) { return _customButtons[id]; }
	wstring FloatToWString(float value, int precision = 2);

	//�������������� string -> wstring
	wstring StringToWide(const string& str);

	// ����� ��� ���������� ������ � ��������
	void UpdateControlText(int controlId, const string& newText);

	// ����� ��� ���������� ������ � �������� wstring
	void UpdateControlText(int controlId, const wstring& newText);

	//����� ��� ����������� �� LPSTR � string
	void GetWindowTextAsString(HWND hWnd, string& output);
	void GetWindowTextAsStringANSI(HWND hWnd, string& output);
	string WideToANSI(const wchar_t* wideStr);
	void AppendTextToEdit(HWND hEdit, const wchar_t* text);
	virtual void CreateOtherWindow(EWindowID id, SColor color = SColor(90, 75, 75)); // ������ ��� �������� ������ ����

public:
	MyWindow(HINSTANCE hInstance, EWindowID windowId, SColor color = SColor(0, 0, 0));
	virtual ~MyWindow();

	void Close();
	HWND GetHandle() const { return _hwnd; }
	EWindowID GetWindowId() const { return _currentWindowId; }

	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void Show(int nCmdShow);
};

