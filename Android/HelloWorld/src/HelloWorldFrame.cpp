#include <new>
#include "HelloWorldFrame.h"
#include "HelloWorldForm.h"

using namespace Tizen::Base;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;

HelloWorldFrame::HelloWorldFrame(void)
{
}

HelloWorldFrame::~HelloWorldFrame(void)
{
}

result
HelloWorldFrame::OnInitializing(void)
{
	result r = E_SUCCESS;

	// Create a form
	HelloWorldForm* pHelloWorldForm = new (std::nothrow) HelloWorldForm();
	TryReturn(pHelloWorldForm != null, false, "The memory is insufficient.");
	pHelloWorldForm->Initialize();

	// Add the form to the frame
	AddControl(pHelloWorldForm);

	// Set the current form
	SetCurrentForm(pHelloWorldForm);

	// Draw the form
	pHelloWorldForm->Invalidate(true);

	// TODO: Add your frame initialization code here.

	return r;
}

result
HelloWorldFrame::OnTerminating(void)
{
	result r = E_SUCCESS;

	// TODO: Add your frame termination code here.
	return r;
}


