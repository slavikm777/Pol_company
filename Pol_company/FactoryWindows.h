#pragma once
#include "CoreTypes.h"
#include "UIMainWindow.h"
#include "UISettings.h"
#include "UILogin.h"
#include "UIRegister.h"


class IFactoryWindow {
public:
	virtual MyWindow* Create(EWindowID id, HINSTANCE hInstance, SColor color = SColor(0, 0, 0)) = 0;
};

class FactoryWindow : public IFactoryWindow
{
public:
	virtual MyWindow* Create(EWindowID id, HINSTANCE hInstance, SColor color = SColor(0, 0, 0)) override
	{
		switch (id)
		{
		case EUIMainWindow: return new UIMainWindow(hInstance, id, color);
		case EUISettings: return new UISettings(hInstance, id, color);
		case EUILogin: return new UILogin(hInstance, id, color);
		case EUIRegister: return new UIRegister(hInstance, id, color);
		//case EUIChangePassword: return new UIChangePassword(hInstance, id, color);
		//case EUIListOrders: return new UIListOrders(hInstance, id, color);
		//case EUINewOrder: return new UINewOrder(hInstance, id, color);
		//case EUIAddService: return new UIAddService(hInstance, id, color);
		default: return nullptr;
		}
	}
};