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

		FORM_PARTS_WIDTH_NUM = 4,
	};
	
public:
	ControleWindow(LuaControleScript *controler);

	void setAnswer(int answer);

	void registerUserButton(const ButtonData& data);
	void registerSystemButton(const ButtonData& data);

	LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	void setClientSize(int width, int height);

	// message crackers
	void OnCreate();
	//void OnInitialUpdate();
	void OnDestroy();
	BOOL OnCommand(WPARAM wParam, LPARAM lParam);

};


#endif

