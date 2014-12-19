#pragma once
#ifndef ___LOG_WINDOW_HPP___
#define ___LOG_WINDOW_HPP___

#include <wincore.h>
#include <controls.h>
#include <stdcontrols.h>
#include <dialog.h>

class Ash;

class ToolWindow : public CDialog
{
private:
	Ash& ash_;

public:
	ToolWindow(Ash& ash, UINT nResID);
	
private:
	std::string askFileOpen(const char *defaultExtention, const char *filter, const char *title);
	std::string askFileSave(const char *defaultExtention, const char *filter, const char *title);

	BOOL OnInitDialog();
	BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	void OnSaveData();
	void OnRestoreData();
};

#endif
