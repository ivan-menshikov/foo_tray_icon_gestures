#pragma once

class CFreeToolTip : public CWindowImpl<CFreeToolTip, CWindow, CNullTraits> {
public:
	BEGIN_MSG_MAP_EX( CFreeToolTip )
		MSG_WM_CREATE( OnCreate )
		MSG_WM_DESTROY( OnDestroy )
		MSG_WM_TIMER( OnTimer )
	END_MSG_MAP()

	DECLARE_WND_CLASS_EX( TEXT( "{AD05DE27-872F-4660-B7BB-869F3AF52BC7}" ), 0, COLOR_HIGHLIGHT );

	void ShowToolTip( const POINT& pt, const wchar_t* message, UINT timeoutMs );

protected:
	LRESULT OnCreate( LPCREATESTRUCT );
	void OnDestroy();
	void OnTimer( WPARAM id );

private:
	CToolTipCtrl toolTip;

	UINT_PTR toolId() const { return reinterpret_cast<UINT_PTR>( toolTip.m_hWnd ); }
};
