#ifndef ___CONTROLE_WINDOW_HPP___
#define ___CONTROLE_WINDOW_HPP___

#include <wincore.h>
#include <stdcontrols.h>
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
	class NameEdit : public CEdit
	{
	private:
		ControleWindow& window_;
		int index_;

	public:
		NameEdit(ControleWindow& window, int index);

	private:
		LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	};

	LuaControleScript *controler_;
	std::vector<ButtonData> userForm_, systemForm_;
	int answer_;
	std::vector<std::unique_ptr<NameEdit>> nameEdits_;
	bool hasNameEditsEnabled_;

	enum {
		FORM_PART_WIDTH = 100,
		FORM_PART_HEIGHT = 40,

		FORM_PARTS_WIDTH_NUM = 4,

		SAVE_TMP_TIMER_ID = 100,
		SAVE_TMP_TIMER_INTERVAL = 10 * 1000,
	};
	
public:
	ControleWindow(LuaControleScript *controler, bool hasNameEditsEnabled);

	int getAnswer() const;
	void setAnswer(int answer);

	void moveCursor(int index);

	void registerUserButton(const ButtonData& data);
	void registerSystemButton(const ButtonData& data);

	LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	void setClientSize(int width, int height);

	// message crackers
	void PreCreate(CREATESTRUCT& cs);
	void OnCreate();
	//void OnInitialUpdate();
	void OnDestroy();
	BOOL OnCommand(WPARAM wParam, LPARAM lParam);

};


#endif

