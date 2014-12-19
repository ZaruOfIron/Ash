#include "log_window.hpp"
#include "resource.h"
#include "ash.hpp"
#include <fstream>

LogWindow::LogWindow(Ash& ash, UINT nResID)
	: CDialog(nResID), ash_(ash)
{}

std::string LogWindow::askFileOpen(const char *defaultExtention, const char *filter, const char *title)
{
	static OPENFILENAME ofn;
	static char path[MAX_PATH], file[MAX_PATH];

	::GetCurrentDirectory(MAX_PATH, path);

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = m_hWnd;
	ofn.lpstrInitialDir = path;
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = file;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_FILEMUSTEXIST;
	ofn.lpstrTitle = title;
	ofn.lpstrDefExt = defaultExtention;

	if(::GetOpenFileName(&ofn))	return std::string(file);
	return std::string("");
}

std::string LogWindow::askFileSave(const char *defaultExtention, const char *filter, const char *title)
{
	static OPENFILENAME ofn;
	static char path[MAX_PATH], file[MAX_PATH];

	::GetCurrentDirectory(MAX_PATH, path);

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = m_hWnd;
	ofn.lpstrInitialDir = path;
	ofn.lpstrFile = file;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrDefExt = defaultExtention;
	ofn.lpstrFilter = filter;
	ofn.lpstrTitle = title;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;

	if(::GetSaveFileName(&ofn))	return std::string(file);
	return std::string("");
}

BOOL LogWindow::OnInitDialog()
{
	return FALSE;
}

BOOL LogWindow::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch(LOWORD(wParam))
	{
	case ID_UNDO:
		ash_.undo();
		return TRUE;
	case ID_SAVEDATA:
		OnSaveData();
		return TRUE;
	case ID_RESTOREDATA:
		OnRestoreData();
		return TRUE;
	}

	return FALSE;
}

void LogWindow::OnSaveData()
{
	std::string file = askFileSave("asd", "asd(*.asd)\0*.asd\0All files(*.*)\0*.*\0\0", "Open Save Data");
	if(file.empty())	return;

	std::ofstream ofs(file);
	ash_.writeSaveData(ofs);
}

void LogWindow::OnRestoreData()
{
	std::string file = askFileOpen("asd", "asd(*.asd)\0*.asd\0All files(*.*)\0*.*\0\0", "Open Save Data");
	if(file.empty())	return;

	std::ifstream ifs(file);
	ash_.readSaveData(ifs);
}

