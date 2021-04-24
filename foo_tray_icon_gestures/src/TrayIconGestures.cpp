#include "stdafx.h"
#include "TrayIconGestures.h"
#include "Preferences.h"
#include <sstream>
#include <iomanip>

// This is true for v1.6.2
static constexpr UINT FB2KTrayIconId = 1;
static constexpr UINT FB2KTrayIconMessage = WM_USER + 7;

static constexpr UINT TrayToolTipTimeoutMs = 1000;

CTrayIconGestures::CTrayIconGestures()
	: mainWindowHandle( NULL )
	, lowLevelMouseHook( false )
{
}

CTrayIconGestures& CTrayIconGestures::Instance()
{
	static CTrayIconGestures instance;
	return instance;
}

void CTrayIconGestures::Initialize()
{
	core_api::ensure_main_thread();

	mainWindowHandle = core_api::get_main_window();
	lowLevelMouseHook.Install<CTrayIconGestures, &InstancePtr, &CTrayIconGestures::OnLowLevelMouseHook>( NULL, 0 );
	toolTip.Create( mainWindowHandle );
}

void CTrayIconGestures::Deinitialize()
{
	core_api::ensure_main_thread();

	toolTip.DestroyWindow();
	lowLevelMouseHook.Uninstall();
	mainWindowHandle = NULL;
}

void CTrayIconGestures::OnMouseWheel( const POINT& pt, const int delta )
{
	const double steps = static_cast<double>( delta ) / WHEEL_DELTA;
	const double stepPercent = static_cast<double>( Preferences::WheelStepPercent );

	auto playback = playback_control::get();

	double db = playback->get_volume();
	db = Utils::AdjustVolumeDB( db, steps * stepPercent, Preferences::UseSliderScale );
	playback->set_volume( static_cast<float>( db ) );

	fb2k::inMainThread2( [this, pt, db] () {
		std::wstringstream tip;
		tip << std::fixed << std::setprecision( 2 ) << db << " dB";
		toolTip.ShowToolTip( pt, tip.str().c_str(), TrayToolTipTimeoutMs );
		} );
}

void CTrayIconGestures::OnMouseMButtonUp( const POINT& pt )
{
	auto playback = playback_control::get();

	if(playback->is_playing()) {
		playback->toggle_pause();
	} else {
		playback->start();
	}

	fb2k::inMainThread2( [this, pt] () {
		const bool isPaused = playback_control::get()->is_paused();
		toolTip.ShowToolTip( pt, isPaused ? L"Paused" : L"Started", TrayToolTipTimeoutMs );
	} );
}

void CTrayIconGestures::OnLowLevelMouseHook( const WPARAM wParam, MSLLHOOKSTRUCT* const lParam )
{
	if(lParam == nullptr) {
		return;
	}

	switch(wParam) {
	case WM_MOUSEWHEEL: 
		if(isInsideTrayIcon( lParam->pt )) {
			OnMouseWheel( lParam->pt, wheelDeltaFromMouseData( lParam->mouseData ) );
		}
		break;
	case WM_MBUTTONUP: 
		if(isInsideTrayIcon( lParam->pt )) {
			OnMouseMButtonUp( lParam->pt );
		}
		break;
	default: 
		break;
	}
}

bool CTrayIconGestures::isInsideTrayIcon( const POINT& pt ) const
{
	RECT rect = {};
	return Utils::GetTrayIconRect( mainWindowHandle, FB2KTrayIconId, rect )
		&& Utils::IsPointInRect( pt, rect );
}

int CTrayIconGestures::wheelDeltaFromMouseData( const DWORD mouseData )
{
	static_assert( sizeof( WORD ) == sizeof( short ), "WORD must be the same size as short" );
	return static_cast<short>( HIWORD( mouseData ) );
}
