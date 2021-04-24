#pragma once

#include "Component.h"
#include "Utils.h"

class CWindowsHook : public CNotCopyable {
public:
	explicit CWindowsHook( bool uninstallOnDestroy ) noexcept;
	~CWindowsHook() noexcept;

	CWindowsHook( CWindowsHook&& other ) noexcept;
	CWindowsHook& operator=( CWindowsHook&& other ) noexcept;

	bool IsInstalled() const { return handle != NULL; }
	void Install( int hookId, HOOKPROC hookProc, HINSTANCE hInstance, DWORD threadId );
	void Uninstall() noexcept;

private:
	bool uninstallOnDestroy = false;
	HHOOK handle = NULL;
};

template<typename TTarget, typename LParam>
using TWindowsHookProc = void ( TTarget::* )( WPARAM wParam, LParam lParam );

template<typename TTarget>
using TGetWindowsHookTargetProc = TTarget*(*)();

template<int HookId, typename LParam>
class CWindowsHookT : public CNotCopyable {
public:
	explicit CWindowsHookT( const bool uninstallOnDestroy ) : hook( uninstallOnDestroy ) {}

	bool IsInstalled() const { return hook.IsInstalled(); }

	template<typename TTarget, TGetWindowsHookTargetProc<TTarget> GetTargetProc, TWindowsHookProc<TTarget, LParam> HookProc>
	void Install( const HINSTANCE hInstance, const DWORD threadId )
	{
		hook.Install( HookId, &hookProc<TTarget, GetTargetProc, HookProc>, hInstance, threadId );
	}

	void Uninstall() { hook.Uninstall(); }

private:
	CWindowsHook hook;

	template<typename TTarget, TGetWindowsHookTargetProc<TTarget> GetTargetProc, TWindowsHookProc<TTarget, LParam> HookProc>
	static LRESULT CALLBACK hookProc( const int nCode, const WPARAM wParam, const LPARAM lParam )
	{
		if(nCode == HC_ACTION) {
			try {
				TTarget* const hookTarget = GetTargetProc();
				if(hookTarget != nullptr) {
					( hookTarget->*HookProc )( wParam, reinterpret_cast<LParam>( lParam ) );
				}
			} catch_log_ignore;
		}
		return ::CallNextHookEx( 0, nCode, wParam, lParam );
	}
};

using CMouseHook = CWindowsHookT<WH_MOUSE, MOUSEHOOKSTRUCTEX*>;
using CLowLevelMouseHook = CWindowsHookT<WH_MOUSE_LL, MSLLHOOKSTRUCT*>;
using CCallWndProcHook = CWindowsHookT<WH_CALLWNDPROC, CWPSTRUCT*>;
using CCallWndProcRetHook = CWindowsHookT<WH_CALLWNDPROCRET, CWPRETSTRUCT*>;
using CGetMessageHook = CWindowsHookT<WH_GETMESSAGE, MSG*>;
