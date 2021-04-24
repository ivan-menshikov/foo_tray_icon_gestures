#include "stdafx.h"
#include "WindowsHook.h"

CWindowsHook::CWindowsHook( const bool _uninstallOnDestroy ) noexcept
	: uninstallOnDestroy( _uninstallOnDestroy )
	, handle( NULL )
{
}

CWindowsHook::~CWindowsHook() noexcept
{
	if(uninstallOnDestroy) {
		Uninstall();
	}
}

CWindowsHook::CWindowsHook( CWindowsHook&& other ) noexcept
	: uninstallOnDestroy( std::exchange( other.uninstallOnDestroy, false ) )
	, handle( std::exchange( other.handle, static_cast<HHOOK>( NULL ) ) )
{
}

CWindowsHook& CWindowsHook::operator=( CWindowsHook&& other ) noexcept
{
	if(this != &other) {
		uninstallOnDestroy = std::exchange( other.uninstallOnDestroy, false );
		handle = std::exchange( other.handle, static_cast<HHOOK>( NULL ) );
	}
	return *this;
}

void CWindowsHook::Install( const int hookId, const HOOKPROC hookProc, const HINSTANCE hInstance, const DWORD threadId )
{
	Uninstall();
	handle = ::SetWindowsHookExW( hookId, hookProc, hInstance, threadId );
	WIN32_OP( handle != NULL );
}

void CWindowsHook::Uninstall() noexcept
{
	if(handle == NULL) return;
	::UnhookWindowsHookEx( handle );
	handle = NULL;
}
