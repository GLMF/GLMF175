#include "MiniGameForm.h"
#include "AppResourceId.h"

using namespace Tizen::Base;
using namespace Tizen::App;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;

MiniGameForm::MiniGameForm(void)
    : __pLabelQuestion (null),
      __pLabelScore (null)
{
}

MiniGameForm::~MiniGameForm(void)
{
}

bool
MiniGameForm::Initialize(void)
{
	result r = Construct(IDL_FORM);
	TryReturn(r == E_SUCCESS, false, "Failed to construct form");

	return true;
}
result
MiniGameForm::OnInitializing(void)
{
	result r = E_SUCCESS;

	// TODO: Add your initialization code here

	// Setup back event listener
	SetFormBackEventListener(this);

	// Get a button via resource ID
	Tizen::Ui::Controls::Button* pButtonVrai = static_cast< Button* >(GetControl(IDC_BUTTON_VRAI));
	if (pButtonVrai != null)
	{
		pButtonVrai->SetActionId(IDA_BUTTON_VRAI);
		pButtonVrai->AddActionEventListener(*this);
	}

	Tizen::Ui::Controls::Button* pButtonFaux = static_cast< Button* >(GetControl(IDC_BUTTON_FAUX));
	if (pButtonFaux != null)
	{
		pButtonFaux->SetActionId(IDA_BUTTON_FAUX);
		pButtonFaux->AddActionEventListener(*this);
	}

	__pLabelQuestion = static_cast< TextBox* >(GetControl(IDC_TEXTBOX_QUESTION));
	TryReturn(__pLabelQuestion != null, r = E_SYSTEM, "Panel::GetControl(IDC_TEXTBOX_QUESTION) failed");

    __pLabelQuestion->SetText(L"Vous êtes prêt");
    __reponse = -1;

	__pLabelScore = static_cast< TextBox* >(GetControl(IDC_TEXTBOX_SCORE));
	TryReturn(__pLabelScore != null, r = E_SYSTEM, "Panel::GetControl(IDC_TEXTBOX_SCORE) failed");

    __pLabelScore->SetText(__score.ToString());

	return r;
}

result
MiniGameForm::OnTerminating(void)
{
	result r = E_SUCCESS;

	// TODO: Add your termination code here

	return r;
}

void
MiniGameForm::OnActionPerformed(const Tizen::Ui::Control& source, int actionId)
{
	bool ok = false;
	switch(actionId)
	{
	case IDA_BUTTON_VRAI:
		if (__reponse == 0)
		{
			__score.value++;
            ok = true;
		}
		else if (__reponse == -1)
		{
			ok = true;
		}
		else
		{
			__score.value = __score.value - 2;
		}
		AppLog("Vrai Button is clicked!\n");
		break;
	case IDA_BUTTON_FAUX:
		if (__reponse == 1)
		{
			__score.value++;
			ok = true;
		}
		else
		{
			__score.value = __score.value - 2;
		}
		AppLog("Faux Button is clicked!\n");
		break;
	default:
		break;
	}
    __pLabelScore->SetText(__score.ToString());

    // Nouvelle question
    if (ok)
    {
    	Integer a, b;
    	int operateur;
    	String op;

    	a.value = 1 + (rand() % 10);
    	b.value = 1 + (rand() % 10);
    	__reponse = 1;

    	operateur = 1 + (rand() % 3);

    	switch(operateur)
    	{
    	case 1:
    		op = L" > ";
    		if (a.value > b.value)
    		{
    		    __reponse = 0;
    		}
    		break;
    	case 2:
    		op = L" < ";
    		if (a.value < b.value)
    		{
    		    __reponse = 0;
    		}
    		break;
    	default:
    		op = L" == ";
    		if (a.value == b.value)
    		{
    		    __reponse = 0;
    		}
    		break;
    	}
        __pLabelQuestion->SetText(a.ToString() + op + b.ToString());
    }

	Invalidate(true);
}

void
MiniGameForm::OnFormBackRequested(Tizen::Ui::Controls::Form& source)
{
	UiApp* pApp = UiApp::GetInstance();
	AppAssert(pApp);
	pApp->Terminate();
}


