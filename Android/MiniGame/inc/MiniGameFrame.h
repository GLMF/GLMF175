#ifndef _MINI_GAME_FRAME_H_
#define _MINI_GAME_FRAME_H_

#include <FApp.h>
#include <FBase.h>
#include <FSystem.h>
#include <FUi.h>
#include <FUiIme.h>
#include <FGraphics.h>
#include <gl.h>

class MiniGameFrame
	: public Tizen::Ui::Controls::Frame
{
public:
	MiniGameFrame(void);
	virtual ~MiniGameFrame(void);

private:
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);
};

#endif	//_MINI_GAMEFRAME_H_
