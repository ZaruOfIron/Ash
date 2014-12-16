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

	ButtonData(int id_, const std::string& caption_)
		: id(id_), caption(caption_){}
};

class ControleWindow : public CWnd
{
private:
	LuaControleScript *controler_;
	std::vector<ButtonData> userForm_, systemForm_;
	int answer_;

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
	ControleWindow(LuaControleScript *controler);

	void setAnswer(int answer);

	void registerUserButton(const ButtonData& data);
	void registerSystemButton(const ButtonData& data);

	void setUserButtonState(int index, int id, bool hasEnabled);
	void setSystemButtonState(int id, bool hasEnabled);

	LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
};


#endif

