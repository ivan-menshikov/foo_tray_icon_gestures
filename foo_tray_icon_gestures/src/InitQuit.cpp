#include "stdafx.h"
#include "TrayIconGestures.h"

class CInitQuit : public initquit {
public:
	void on_init() override
	{
		try {
			CTrayIconGestures::Instance().Initialize();
		} catch_log_ignore; 
	}

	void on_quit() override
	{
		try {
			CTrayIconGestures::Instance().Deinitialize();
		} catch_log_ignore;
	}
};

static service_factory_single_t<CInitQuit> InitQuit;
