#include <new>
#include "MiniGameFrame.h"
#include "MiniGameForm.h"

using namespace Tizen::Base;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;

MiniGameFrame::MiniGameFrame(void)
{
}

MiniGameFrame::~MiniGameFrame(void)
{
}

result
MiniGameFrame::OnInitializing(void)
{
	result r = E_SUCCESS;

	// Create a form
	MiniGameForm* pMiniGameForm = new (std::nothrow) MiniGameForm();
	TryReturn(pMiniGameForm != null, false, "The memory is insufficient.");
	pMiniGameForm->Initialize();

	// Add the form to the frame
	AddControl(pMiniGameForm);

	// Set the current form
	SetCurrentForm(pMiniGameForm);

	// Draw the form
	pMiniGameForm->Invalidate(true);

	// TODO: Add your frame initialization code here.

	return r;
}

result
MiniGameFrame::OnTerminating(void)
{
	result r = E_SUCCESS;

	// TODO: Add your frame termination code here.
	return r;
}


