
// SDKExample.h 
//

#pragma once

#ifndef __AFXWIN_H__
	#error " 'stdafx.h'"
#endif

#include "resource.h"

// CSDKExampleApp:

class CSDKExampleApp : public CWinAppEx
{
public:
	CSDKExampleApp();

	public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

extern CSDKExampleApp theApp;