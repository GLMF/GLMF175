#include "HelloWorldForm.h"
#include "AppResourceId.h"

using namespace Tizen::Base;
using namespace Tizen::App;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;

HelloWorldForm::HelloWorldForm(void)
	: __pLabelPrint (null)
{
}

HelloWorldForm::~HelloWorldForm(void)
{
}

bool
HelloWorldForm::Initialize(void)
{
	result r = Construct(IDL_FORM);
	TryReturn(r == E_SUCCESS, false, "Failed to construct form");

	return true;
}


result
HelloWorldForm::OnInitializing(void)
{
	result r = E_SUCCESS;

	// TODO: Add your initialization code here

	// Setup back event listener
	SetFormBackEventListener(this);

	// Get a button via resource ID
	Tizen::Ui::Controls::Button* pButtonOk = static_cast< Button* >(GetControl(IDC_BUTTON_OK));
	if (pButtonOk != null)
	{
		pButtonOk->SetActionId(IDA_BUTTON_OK);
		pButtonOk->AddActionEventListener(*this);
	}

	__pLabelPrint = static_cast< TextBox* >(GetControl(IDC_TEXTBOX2));
	TryReturn(__pLabelPrint != null, r = E_SYSTEM, "Panel::GetControl(IDC_TEXTBOX2) failed");

    __pLabelPrint->SetText(__count.ToString());

	return r;
}

result
HelloWorldForm::OnTerminating(void)
{
	result r = E_SUCCESS;

	// TODO: Add your termination code here

	return r;
}

void
HelloWorldForm::OnActionPerformed(const Tizen::Ui::Control& source, int actionId)
{
	switch(actionId)
	{
	case IDA_BUTTON_OK:
		__count.value++;
	    __pLabelPrint->SetText(__count.ToString());
		__pLabelPrint->Invalidate(true);
		AppLog("Vous avez appuyé sur le bouton OK\n");
		break;

	default:
		break;
	}
}

void
HelloWorldForm::OnFormBackRequested(Tizen::Ui::Controls::Form& source)
{
	UiApp* pApp = UiApp::GetInstance();
	AppAssert(pApp);
	pApp->Terminate();
}


