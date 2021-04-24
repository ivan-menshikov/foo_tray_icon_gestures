#include "stdafx.h"
#include "Utils.h"

using CNotifyIconIdentifier = CSizedZeroStruct<NOTIFYICONIDENTIFIER, DWORD, &NOTIFYICONIDENTIFIER::cbSize>;

bool Utils::GetTrayIconRect( const HWND hWnd, const UINT iconId, RECT& rect )
{
	CNotifyIconIdentifier id;
	id.hWnd = hWnd;
	id.uID = iconId;
	return S_OK == ::Shell_NotifyIconGetRect( &id, &rect );
}

bool Utils::IsPointInRect( const POINT& pt, const RECT& rect )
{
	return rect.left <= pt.x && pt.x <= rect.right
		&& rect.top <= pt.y && pt.y <= rect.bottom;
}

double Utils::AdjustVolumeDB( const double db, const double percent, const bool useSlider )
{
	if(useSlider) {
		using namespace VolumeMap;
		return SliderToDB( pfc::clip_t( DBToSlider( db ) + percent / 100.0, 0.0, 1.0 ) );
	} else {
		return pfc::clip_t( db + percent, -100.0, 0.0 );
	}
}
