#include "tool_window.hpp"
#include "resource.h"
#include "ash.hpp"
#include <fstream>

ToolWindow::ToolWindow(Ash& ash, UINT nResID)
	: CDialog(nResID), ash_(ash)
{}

std::string ToolWindow::askFileOpen(const char *defaultExtention, const char *filter, const char *title)
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

std::string ToolWindow::askFileSave(const char *defaultExtention, const char *filter, const char *title)
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

BOOL ToolWindow::OnInitDialog()
{
	return FALSE;
}

BOOL ToolWindow::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch(LOWORD(wParam))
	{
	case ID_UNDO:
		ash_.restoreUndoData();
		return TRUE;
	case ID_SAVEDATA:
		OnSaveData();
		return TRUE;
	case ID_RESTOREDATA:
		OnRestoreData();
		return TRUE;
	case ID_SETNAMES:
		OnSetNames();
		return TRUE;
	}

	return FALSE;
}

void ToolWindow::OnSaveData()
{
	std::string file = askFileSave("asd", "asd(*.asd)\0*.asd\0All files(*.*)\0*.*\0\0", "Open Save Data");
	if(file.empty())	return;

	std::ofstream ofs(file);
	ash_.writeSaveData(ofs);
}

void ToolWindow::OnRestoreData()
{
	std::string file = askFileOpen("asd", "asd(*.asd)\0*.asd\0All files(*.*)\0*.*\0\0", "Open Save Data");
	if(file.empty())	return;

	std::ifstream ifs(file);
	ash_.readSaveData(ifs);
}

void ToolWindow::OnSetNames()
{
	std::string file = askFileOpen("txt", "txt(*.txt)\0*.txt\0All files(*.*)\0*.*\0\0", "Open name data");
	if(file.empty())	return;

	std::ifstream ifs(file);
	std::vector<std::string> names;
	std::string inp;
	while(std::getline(ifs, inp)){
		names.push_back(inp);
	}
	ash_.setUserNames(names);
}

