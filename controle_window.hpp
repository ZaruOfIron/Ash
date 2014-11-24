#ifndef ___CONTROLE_WINDOW_HPP___
#define ___CONTROLE_WINDOW_HPP___

#include <wincore.h>

class LuaControleScript;

class ControleWindow : public CWnd
{
private:
	LuaControleScript *controler_;

	enum {
		FORM_PART_WIDTH = 100,
		FORM_PART_HEIGHT = 40,
	};

	void setClientSize(int width, int height);

public:
	ControleWindow(LuaControleScript *controler);

	void OnCreate();
	void OnInitialUpdate();
	void OnDestroy();
	BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
};


#endif

