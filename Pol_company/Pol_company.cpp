// Pol_company.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
#include <iostream>
#include "Settings.h"
#include "FactoryWindows.h"
using namespace std;

class Application
{
public:
	int Run(HINSTANCE hInstance, int nCmdShow)
	{
		MyWindow* _mainWindow;
		Settings* _settings = Settings::GetInstance(); //Сущность настроек

		FactoryWindow _factory;
		if (_settings->LoadSettings())
			_mainWindow = _factory.Create(EUIMainWindow, hInstance, SColor::BaseColor());
		else
			_mainWindow = _factory.Create(EUISettings, hInstance, SColor::AdditionalColor());

		if (!_mainWindow->GetHandle()) return 1;

		//Показываем окно
		_mainWindow->Show(nCmdShow);

		//Цикл обработки сообщений
		MSG msg;
		while (GetMessage(&msg, nullptr, 0, 0)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		_settings->ReleaseInstance();
		delete _mainWindow;
		return (int)msg.wParam;
	}
};

// Точка входа
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	Application app;
	return app.Run(hInstance, nCmdShow);
}


int main()
{
    cout << "Hello World!\n";
}

