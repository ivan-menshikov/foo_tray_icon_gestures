#pragma once

#include <Component.h>

namespace Utils
{
	bool GetTrayIconRect( HWND hWnd, UINT iconId, RECT& rect );
	bool IsPointInRect( const POINT& pt, const RECT& rect );
	double AdjustVolumeDB( double db, double percent, bool useSlider );
}

struct CNotCopyable {
	CNotCopyable() = default;
	CNotCopyable( const CNotCopyable& ) = delete;
	CNotCopyable& operator=( const CNotCopyable& ) = delete;
};

struct CNotMovable {
	CNotMovable() = default;
	CNotMovable( CNotMovable&& ) = delete;
	CNotMovable& operator=( CNotMovable&& ) = delete;
};

struct CNotCopyableNorMovable : CNotCopyable, CNotMovable {
};

template<typename TStruct>
struct CZeroStruct : TStruct {
	CZeroStruct()
	{
		::ZeroMemory( this, sizeof( TStruct ) );
	}
};

template<typename TStruct, typename TSize, TSize TStruct::* SizeMember>
struct CSizedZeroStruct : TStruct {
	CSizedZeroStruct()
	{
		::ZeroMemory( this, sizeof( TStruct ) );
		this->*SizeMember = static_cast<TSize>( sizeof( TStruct ) );
	}
};

#define logger() FB2K_console_formatter() << Component::Name << ": "

inline void log_error( const char* const func, const std::exception& e ) { logger() << "std::exception at " << func << ": " << e.what(); }
inline void log_error( const char* const func, const ATL::CAtlException& e ) { logger() << "ATL::CAtlException at " << func << ": " << format_hresult( e.m_hr ); }
inline void log_unknown_error( const char* const func ) { logger() << "unknown exception at " << func; }

#define catch_log_ignore \
	catch( const std::exception& e ) { log_error( __func__, e ); } \
	catch( const ATL::CAtlException& e ) { log_error( __func__, e ); } \
	catch( ... ) { log_unknown_error( __func__ ); }

#define catch_log_return(x) \
	catch( const std::exception& e ) { log_error( __func__, e ); return (x); } \
	catch( const ATL::CAtlException& e ) { log_error( __func__, e ); return (x); } \
	catch( ... ) { log_unknown_error( __func__ ); return (x); }

#define catch_log_return_void \
	catch( const std::exception& e ) { log_error( __func__, e ); return; } \
	catch( const ATL::CAtlException& e ) { log_error( __func__, e ); return; } \
	catch( ... ) { log_unknown_error( __func__ ); return; }
