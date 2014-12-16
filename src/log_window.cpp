#include "log_window.hpp"
#include "resource.h"

BOOL LogWindow::OnInitDialog()
{
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

void LogWindow::write(const std::string& msg)
{
}

