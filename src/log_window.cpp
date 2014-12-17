#include "log_window.hpp"
#include "resource.h"

BOOL LogWindow::OnInitDialog()
{
	AttachItem(ID_LOGEDIT, logEdit_);

	return FALSE;
}

BOOL LogWindow::OnCommand(WPARAM wParam, LPARAM lParam)
{
	/*
	switch(LOWORD(wParam))
	{
	}
	*/

	return FALSE;
}

#include <iostream>
void LogWindow::write(const std::string& msg)
{
	if(!logEdit_.IsWindow())	return;

	logEdit_.ReplaceSel(msg.c_str(), FALSE);
	logEdit_.ReplaceSel("\r\n", FALSE);
}

