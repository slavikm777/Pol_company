// Pol_company.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
#include <iostream>
#include "FactoryWindows.h"
using namespace std;

//#FFFFFF - 255,255,255 - Основной фон
//#F4E8D3 - 244,232,211 - Дополнительный фон
//#67BA80 - 103,186,128 - Акцентирование внимания

class Application
{
public:
	int Run(HINSTANCE hInstance, int nCmdShow)
	{
		MyWindow* _mainWindow;
		Settings* _settings = Settings::GetInstance(); //Сущность настроек

		FactoryWindow _factory;
		_mainWindow = _factory.Create(EUIMainWindow, hInstance, SColor(244, 232, 211));

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

