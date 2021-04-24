#include "stdafx.h"
#include "Preferences.h"
#include "Component.h"

static const GUID GuidBranch = { 0xafc8bbd8, 0xbf71, 0x48a4, { 0x9c, 0xc0, 0x17, 0x39, 0xe6, 0x6a, 0xec, 0x78 } };
static const GUID GuidWheelStepPercent = { 0x917229d9, 0xdff8, 0x48c9, { 0xad, 0x4f, 0x19, 0xd6, 0x88, 0xa8, 0xfd, 0xd6 } };
static const GUID GuidUseSliderScale = { 0x7ef652c5, 0x79d7, 0x431e, { 0x83, 0x5b, 0x4a, 0xc1, 0x42, 0x67, 0x2e, 0xc8 } };

static advconfig_branch_factory PreferencesBranch( Component::Name, GuidBranch, advconfig_branch::guid_branch_tools, 0 );

advconfig_signed_integer_factory Preferences::WheelStepPercent( "Wheel step percent (-100 to 100)", GuidWheelStepPercent, GuidBranch, 0, 1, -100, 100 );
advconfig_checkbox_factory Preferences::UseSliderScale( "Use slider scale", GuidUseSliderScale, GuidBranch, 0, false );