#include "controle_window.hpp"
#include "lua_controle_script.hpp"
#include "resource.h"

ControleWindow::NameEdit::NameEdit(ControleWindow& window, int index)
	: window_(window), index_(index)
{}

LRESULT ControleWindow::NameEdit::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_KEYDOWN:
			if(wParam == VK_TAB)	window_.moveCursor(index_ % window_.getAnswer() + 1);
			break;
	}

	return WndProcDefault(uMsg, wParam, lParam);
}

ControleWindow::ControleWindow(LuaControleScript *controler, bool hasNameEditsEnabled)
	: controler_(controler), answer_(0), hasNameEditsEnabled_(hasNameEditsEnabled)
{}

int ControleWindow::getAnswer() const
{
	return answer_;
}

void ControleWindow::setAnswer(int answer)
{
	answer_ = answer;
}

void ControleWindow::moveCursor(int index)
{
	GetDlgItem(index << 8)->SetFocus();
}

void ControleWindow::registerUserButton(const ButtonData& data)
{
	userForm_.push_back(data);
}

void ControleWindow::registerSystemButton(const ButtonData& data)
{
	systemForm_.push_back(data);
}

LRESULT ControleWindow::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_CREATE:
		OnCreate();
		return 0;
	case WM_TIMER:
		if(wParam == SAVE_TMP_TIMER_ID){
			controler_->saveTmpFile();
			KillTimer(SAVE_TMP_TIMER_ID);
			return 0;
		}
		break;
	case WM_DESTROY:
		OnDestroy();
		return 0;
	}

	return WndProcDefault(uMsg, wParam, lParam);
}

void ControleWindow::setClientSize(int width, int height)
{
	CRect rw = GetWindowRect(), rc = GetClientRect();
	SetWindowPos(NULL, 0, 0,
		(rw.right - rw.left) - (rc.right - rc.left) + width,
		(rw.bottom - rw.top) - (rc.bottom - rc.top) + height,
		SWP_NOMOVE | SWP_NOZORDER);
}

void ControleWindow::PreCreate(CREATESTRUCT& cs)
{
	cs.style &= ~(WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX);
	cs.lpszName = "Ash";
}

void ControleWindow::OnCreate()
{
	// Iconを指定
	SetIconLarge(ID_ICON);
	SetIconSmall(ID_ICON);

	int scrWidth = 0, scrHeight = 0;
	// User Formを設置
	for(int i = 0;i < answer_;i++){
		int nx = i % FORM_PARTS_WIDTH_NUM, ny = i / FORM_PARTS_WIDTH_NUM;
		CRect rect(
			10 + nx * (FORM_PART_WIDTH + 20),
			10 + (FORM_PART_HEIGHT + 10) * (userForm_.size() + 1) * ny,
			10 + nx * (FORM_PART_WIDTH + 20) + FORM_PART_WIDTH,
			10 + (FORM_PART_HEIGHT + 10) * (userForm_.size() + 1) * (ny + 1));
		if(scrWidth < rect.right + 10)	scrWidth = rect.right + 10;
		if(scrHeight < rect.bottom + 10)	scrHeight = rect.bottom + 10;

		int index = i + 1;

		nameEdits_.push_back(std::unique_ptr<NameEdit>(new NameEdit(*this, index)));
		nameEdits_.back()->CreateEx(
			0,
			"EDIT",
			"",
			WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_LEFT,
			rect.left, rect.top, rect.Width(), FORM_PART_HEIGHT,
			GetHwnd(), reinterpret_cast<HMENU>(index << 8), NULL);
		if(!hasNameEditsEnabled_)	nameEdits_.back()->EnableWindow(FALSE);


		for(int j = 0;j < userForm_.size();j++){
			int x = rect.left, y = rect.top + (j + 1) * (FORM_PART_HEIGHT + 10);
			const auto& part = userForm_.at(j);
			::CreateWindowEx(
				0,
				"BUTTON",
				part.caption.c_str(),
				WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,
				x, y, rect.Width(), FORM_PART_HEIGHT,
				GetHwnd(),
				reinterpret_cast<HMENU>(index << 8 | part.id),
				::GetModuleHandle(NULL), NULL);
		}
	}

	// System Formを設置
	int systemY = scrHeight;
	for(int i = 0;i < systemForm_.size();i++){
		int nx = i % FORM_PARTS_WIDTH_NUM, ny = i / FORM_PARTS_WIDTH_NUM;

		CRect rect(
			10 + nx * (FORM_PART_WIDTH + 20),
			systemY + 10 + (FORM_PART_HEIGHT + 10) * ny,
			10 + nx * (FORM_PART_WIDTH + 20) + FORM_PART_WIDTH,
			systemY + 10 + (FORM_PART_HEIGHT + 10) * (ny + 1));
		if(scrHeight < rect.bottom + 10)	scrHeight = rect.bottom + 10;

		::CreateWindowEx(
			0,
			"BUTTON",
			systemForm_.at(i).caption.c_str(),
			WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,
			rect.left, rect.top, rect.Width(), FORM_PART_HEIGHT,
			GetHwnd(), reinterpret_cast<HMENU>(i + 1),
			::GetModuleHandle(NULL), NULL);
	}

	setClientSize(scrWidth, scrHeight);

	// Tmpファイル作成用のタイマを設置
	SetTimer(SAVE_TMP_TIMER_ID, SAVE_TMP_TIMER_INTERVAL, NULL);
}

void ControleWindow::OnDestroy()
{
	::PostQuitMessage(0);
}

BOOL ControleWindow::OnCommand(WPARAM wParam, LPARAM lParam)
{
	int num = LOWORD(wParam), id = (num & 0xff), index = (num >> 8);
	if(index < 0)	return FALSE;

	// Tmpファイル作成用のタイマをリセット
	KillTimer(SAVE_TMP_TIMER_ID);
	SetTimer(SAVE_TMP_TIMER_ID, SAVE_TMP_TIMER_INTERVAL, NULL);

	if(id == 0){	// Edit
		//if(HIWORD(wParam) == EN_UPDATE){
		if(HIWORD(wParam) == EN_KILLFOCUS){
			controler_->onName(index, std::string(GetDlgItem(num)->GetWindowText()));
		}
	}
	else{
		if(index == 0)	controler_->onSystemButton(id);
		else	controler_->onUserButton(index, id);
	}

	return TRUE;
}

