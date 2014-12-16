#pragma once
#ifndef ___LOG_WINDOW_HPP___
#define ___LOG_WINDOW_HPP___

#include <wincore.h>

class LogWindow : public CDialog
{
private:

	BOOL OnInitDialog();
	BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
	LogWindow(UINT nResID, CWnd *pParent = NULL)
		: CDialog(nResID, pParent)
	{}
	
	void write(const std::string& msg);
};

#endif
