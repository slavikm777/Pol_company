#include "UIListParners.h"
#include "FactoryWindows.h"

void UIListPartner::CreateOtherWindow(EWindowID id, SColor color)
{
	FactoryWindow factory;
	MyWindow* nextWindow = factory.Create(id, GetInstance(), color);

	UIPartnerInfo* lUIPartnerInfo = dynamic_cast<UIPartnerInfo*>(nextWindow);
	if (lUIPartnerInfo)
	{
		lUIPartnerInfo->SetIndexItem(_idPartnerInfo);
		
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
