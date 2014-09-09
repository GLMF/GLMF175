#ifndef _MINI_GAME_FORM_H_
#define _MINI_GAME_FORM_H_

#include <FApp.h>
#include <FBase.h>
#include <FSystem.h>
#include <FUi.h>
#include <FUiIme.h>
#include <FGraphics.h>
#include <gl.h>

class MiniGameForm
	: public Tizen::Ui::Controls::Form
	, public Tizen::Ui::IActionEventListener
	, public Tizen::Ui::Controls::IFormBackEventListener
{
public:
	MiniGameForm(void);
	virtual ~MiniGameForm(void);
	bool Initialize(void);

private:
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);
	virtual void OnFormBackRequested(Tizen::Ui::Controls::Form& source);
	virtual void OnActionPerformed(const Tizen::Ui::Control& source, int actionId);

private:
	Tizen::Ui::Controls::TextBox* __pLabelQuestion;
	Tizen::Ui::Controls::TextBox* __pLabelScore;
	Tizen::Base::Integer __score = 0;
	int __reponse;

protected:
	static const int IDA_BUTTON_VRAI = 101;
	static const int IDA_BUTTON_FAUX = 102;
};

#endif	//_MINI_GAME_FORM_H_
