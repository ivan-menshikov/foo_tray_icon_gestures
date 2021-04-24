#include "stdafx.h"
#include "FreeToolTip.h"
#include "Utils.h"

static const UINT_PTR DeactivateToolTipTimerId = 1;

void CFreeToolTip::ShowToolTip( const POINT& pt, const wchar_t* const message, const UINT timeoutMs )
{
	if(!IsWindow() || !toolTip.IsWindow()) return;

	KillTimer( DeactivateToolTipTimerId );
	SetTimer( DeactivateToolTipTimerId, timeoutMs );

	toolTip.TrackActivate( toolTip, toolId(), TRUE );
	toolTip.UpdateTipText( message, toolTip, toolId() );
	const auto size = toolTip.GetBubbleSize( CToolInfo( 0, toolTip, toolId() ) );
	toolTip.TrackPosition( pt.x, pt.y - size.cy );
}

LRESULT CFreeToolTip::OnCreate( LPCREATESTRUCT )
{
	constexpr DWORD toolTipStyle = WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP;
	constexpr DWORD toolTipStyleEx = WS_EX_TOPMOST;
	constexpr UINT toolFlags = TTF_TRACK | TTF_IDISHWND | TTF_ABSOLUTE;

	try {
		WIN32_OP( NULL != toolTip.Create( *this, NULL, nullptr, toolTipStyle, toolTipStyleEx ) );
		WIN32_OP( FALSE != toolTip.AddTool( CToolInfo( toolFlags, toolTip, toolId() ) ) );
	} catch_log_ignore;

	return 0;
}

void CFreeToolTip::OnDestroy()
{
	if(toolTip.IsWindow()) {
		toolTip.DestroyWindow();
	}
}

void CFreeToolTip::OnTimer( const WPARAM id )
{
	if(id == DeactivateToolTipTimerId) {
		KillTimer( DeactivateToolTipTimerId );
		if(toolTip.IsWindow()) {
			toolTip.TrackActivate( toolTip, toolId(), FALSE );
		}
	}
}
