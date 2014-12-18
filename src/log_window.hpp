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

	std::string askFileOpen(char *defaultExtension, char *filter, char *title);
	std::string askFileSave(char *defaultExtension, char *filter, char *title);

	BOOL OnInitDialog();
	BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	void OnSaveData();
	void OnRestoreData();
public:
	LogWindow(Ash& ash, UINT nResID)
		: CDialog(nResID), ash_(ash)
	{}
	
	void write(const std::string& msg);
};

#endif
