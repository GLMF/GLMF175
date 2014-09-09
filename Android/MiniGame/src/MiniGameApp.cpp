#include <new>
#include "MiniGameApp.h"
#include "MiniGameFrame.h"

using namespace Tizen::App;
using namespace Tizen::System;

MiniGameApp::MiniGameApp(void)
{
}

MiniGameApp::~MiniGameApp(void)
{
}

UiApp*
MiniGameApp::CreateInstance(void)
{
	// TODO: Create the application instance through the constructor.
	return new (std::nothrow) MiniGameApp();
}

bool
MiniGameApp::OnAppInitializing(AppRegistry& appRegistry)
{
	// TODO: Initialize application-specific data.
	// The permanent data and context of the application can be obtained from the application registry (appRegistry).
	//
	// If this method is successful, return true; otherwise, return false and the application is terminated.


	// Uncomment the following statement to listen to the screen on and off events:
	// PowerManager::SetScreenEventListener(*this);

	return true;
}

bool
MiniGameApp::OnAppInitialized(void)
{
	// TODO: Create the application frame.

	MiniGameFrame* pMiniGameFrame = new (std::nothrow) MiniGameFrame;
	TryReturn(pMiniGameFrame != null, false, "The memory is insufficient.");
	pMiniGameFrame->Construct();
	pMiniGameFrame->SetName(L"MiniGame");
	AddFrame(*pMiniGameFrame);

	return true;
}

bool
MiniGameApp::OnAppWillTerminate(void)
{
	// TODO: Deallocate or release resources in devices that have the END key.
	return true;
}

bool
MiniGameApp::OnAppTerminating(AppRegistry& appRegistry, bool forcedTermination)
{
	// TODO: Deallocate all resources allocated by the application.
	// The permanent data and context of the application can be saved through the application registry (appRegistry).
	return true;
}

void
MiniGameApp::OnForeground(void)
{
	// TODO: Start or resume drawing when the application is moved to the foreground.
}

void
MiniGameApp::OnBackground(void)
{
	// TODO: Stop drawing when the application is moved to the background to save the CPU and battery consumption.
}

void
MiniGameApp::OnLowMemory(void)
{
	// TODO: Free unnecessary resources or close the application.
}

void
MiniGameApp::OnBatteryLevelChanged(BatteryLevel batteryLevel)
{
	// TODO: Handle all battery level changes here.
	// Stop using multimedia features (such as camera and mp3 playback) if the battery level is CRITICAL.
}

void
MiniGameApp::OnScreenOn(void)
{
	// TODO: Retrieve the released resources or resume the operations that were paused or stopped in the OnScreenOff() event handler.
}

void
MiniGameApp::OnScreenOff(void)
{
	// TODO: Release resources (such as 3D, media, and sensors) to allow the device to enter the sleep mode 
	// to save the battery (unless you have a good reason to do otherwise).
	// Only perform quick operations in this event handler. Any lengthy operations can be risky; 
	// for example, invoking a long asynchronous method within this event handler can cause problems
	// because the device can enter the sleep mode before the callback is invoked.

}
