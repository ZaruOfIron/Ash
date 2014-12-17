#pragma once
#ifndef ___LOG_WINDOW_HPP___
#define ___LOG_WINDOW_HPP___

#include <wincore.h>
#include <controls.h>
#include <stdcontrols.h>
#include <dialog.h>

class LogWindow : public CDialog
{
private:
	CEdit logEdit_;

	BOOL OnInitDialog();
	BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
	LogWindow(UINT nResID)
		: CDialog(nResID)
	{}
	
	void write(const std::string& msg);
};

#endif
