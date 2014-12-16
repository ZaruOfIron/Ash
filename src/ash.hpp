#ifndef ___ASH_HPP___
#define ___ASH_HPP___

#include "user.hpp"
#include "controle_script.hpp"
#include "view.hpp"
#include <memory>
#include <vector>
#include <wincore.h>

class Ash : private CWinApp
{
private:
	std::vector<User> users_;
	std::unique_ptr<ControleScript> controler_;
	std::unique_ptr<View> view_;
	int winner_;

	// クイズの終了確認を行う
	// 戻り値が
	enum class FINISH_STATUS {
		FIGHTING,	// まだ終了していない
		WIN_FINISH,	// 勝ち抜け者の制限到達による終了
		LOSE_FINISH	// 敗退者の制限到達による終了
	};
	FINISH_STATUS getFinishStatus() const;
public:
	Ash();

	void setScript(const std::string& filename);
	void run();

	const User& getUser(int index) const;
	bool hasFinished() const;

	void luaInitialize(int answer, int winner, const std::string& title, const std::string& subtitle, int quizId, const User& orgUser);
	void luaUpdate(const UserUpdateMessage& msg);
};


#endif

