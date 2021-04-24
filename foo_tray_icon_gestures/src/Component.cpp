#include "stdafx.h"
#include "Component.h"

static constexpr char* const ComponentAbout =
"Volume and playback control from tray icon.\r\n"
"Preferences are available on Advanced -> Tools page.\r\n"
"\r\n"
"Author: Ivan Menshikov\r\n"
"Date: " __DATE__ " " __TIME__ "\r\n"
"\r\n"
"FOOBAR2000_SDK_VERSION: " _ATL_STRINGIZE( FOOBAR2000_SDK_VERSION ) "\r\n"
"FOOBAR2000_TARGET_VERSION: " _ATL_STRINGIZE( FOOBAR2000_TARGET_VERSION );

DECLARE_COMPONENT_VERSION( Component::Name, Component::Version, ComponentAbout );
VALIDATE_COMPONENT_FILENAME( Component::FileName );
