#ifndef ___CONTROLE_WINDOW_HPP___
#define ___CONTROLE_WINDOW_HPP___

#include <wincore.h>
#include <memory>
#include <string>
#include <vector>

class LuaControleScript;
class ControleWindow;

struct ButtonData
{
	int id;
	std::string caption;

	ButtonData(int id_, const std::string& caption_);
};

class ControleWindowFrame : private CWinApp
{
private:
	LuaControleScript *controler_;
	std::unique_ptr<ControleWindow> window_;
	std::vector<ButtonData> userForm_, systemForm_;

public:
	friend ControleWindow;

	ControleWindowFrame(LuaControleScript *controler_);

	void registerUserButton(const ButtonData& data);
	void registerSystemButton(const ButtonData& data);
	
	void run(int answer);
};

class ControleWindow : public CWnd
{
private:
	ControleWindowFrame *frame_;

	enum {
		FORM_PART_WIDTH = 100,
		FORM_PART_HEIGHT = 40,
	};

	void setClientSize(int width, int height);

	// message crackers
	void OnCreate();
	//void OnInitialUpdate();
	void OnDestroy();
	BOOL OnCommand(WPARAM wParam, LPARAM lParam);

public:
	ControleWindow(ControleWindowFrame *frame);
	
	LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
};


#endif

