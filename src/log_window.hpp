#pragma once
#ifndef ___LOG_WINDOW_HPP___
#define ___LOG_WINDOW_HPP___

#include <wincore.h>
#include <controls.h>
#include <stdcontrols.h>
#include <dialog.h>

class Ash;

class LogWindow : public CDialog
{
private:
	Ash& ash_;
	CEdit logEdit_;

public:
	LogWindow(Ash& ash, UINT nResID);
	
	void write(const std::string& msg);

private:
	std::string askFileOpen(const char *defaultExtention, const char *filter, const char *title);
	std::string askFileSave(const char *defaultExtention, const char *filter, const char *title);

	BOOL OnInitDialog();
	BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	void OnSaveData();
	void OnRestoreData();
};

#endif
