#include "UIListOrders.h"
#include "FactoryWindows.h"

void UIListOrder::CreateOtherWindow(EWindowID id, SColor color)
{
	FactoryWindow factory;
	MyWindow* nextWindow = factory.Create(id, GetInstance(), color);

	UIOrderInfo* lUIPartnerInfo = dynamic_cast<UIOrderInfo*>(nextWindow);
	if (lUIPartnerInfo)
	{
		lUIPartnerInfo->SetIndexItem(_idOrderInfo);

		if (_otherWindow) {
			MyWindow* old = _otherWindow;
			_otherWindow = nullptr;
			old->Close();
		}

		if (nextWindow && nextWindow->GetHandle()) {
			ShowWindow(_hwnd, SW_HIDE);
			nextWindow->Show(SW_SHOW);
			SetForegroundWindow(nextWindow->GetHandle());
		}
	}
	else MyWindow::CreateOtherWindow(id, color);
}
