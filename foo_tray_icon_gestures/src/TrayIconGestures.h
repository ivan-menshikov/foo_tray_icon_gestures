#pragma once

#include "Utils.h"
#include "FreeToolTip.h"
#include "WindowsHook.h"

class CTrayIconGestures : CNotCopyableNorMovable {
public:
	static CTrayIconGestures& Instance();
	static CTrayIconGestures* InstancePtr() { return &Instance(); }

	void Initialize();
	void Deinitialize();

protected:
	void OnMouseWheel( const POINT& pt, int delta );
	void OnMouseMButtonUp( const POINT& pt );

	void OnLowLevelMouseHook( WPARAM wParam, MSLLHOOKSTRUCT* lParam );

private:
	HWND mainWindowHandle;
	CLowLevelMouseHook lowLevelMouseHook;
	CFreeToolTip toolTip;

	CTrayIconGestures();

	bool isInsideTrayIcon( const POINT& pt ) const;
	static int wheelDeltaFromMouseData( DWORD mouseData );
};
