#ifndef _HELLO_WORLD_FORM_H_
#define _HELLO_WORLD_FORM_H_

#include <FApp.h>
#include <FBase.h>
#include <FSystem.h>
#include <FUi.h>
#include <FUiIme.h>
#include <FGraphics.h>
#include <gl.h>

class HelloWorldForm
	: public Tizen::Ui::Controls::Form
	, public Tizen::Ui::IActionEventListener
	, public Tizen::Ui::Controls::IFormBackEventListener
{
public:
	HelloWorldForm(void);
	virtual ~HelloWorldForm(void);
	bool Initialize(void);

private:
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);
	virtual void OnFormBackRequested(Tizen::Ui::Controls::Form& source);
	virtual void OnActionPerformed(const Tizen::Ui::Control& source, int actionId);

private:
	Tizen::Ui::Controls::TextBox* __pLabelPrint;
	Tizen::Ui::Controls::TextBox* __pLabelScore;
	Tizen::Base::Integer __count = 0;

protected:
	static const int IDA_BUTTON_OK = 101;
};

#endif	//_HELLO_WORLD_FORM_H_
