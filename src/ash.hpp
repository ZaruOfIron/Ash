#ifndef ___ASH_HPP___
#define ___ASH_HPP___

#include "user.hpp"
#include "controle_script.hpp"
#include "view.hpp"
#include "log_window.hpp"
#include <memory>
#include <vector>
#include <wincore.h>

std::string getDateTimeString();

class Ash : private CWinApp
{
private:
	std::vector<User> users_;
	std::unique_ptr<ControleScript> controler_;
	std::unique_ptr<View> view_;
	LogWindow log_;
	int winner_;
	std::string saveFileName_;

	struct SaveData
	{
		int index;
		User user;
		int modIndex;
		std::vector<int> info;
		std::string luaVars;

		SaveData()
			: index(-1){}

	private:
		friend class boost::serialization::access;
		template<class Archive>
			void serialize(Archive& ar, const unsigned int version)
			{
				ar & index & user & modIndex & info & luaVars;
			}
	};
	std::vector<SaveData> blankData_;
	std::vector<std::string> saves_;

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
	void undo();

	const User& getUser(int index) const;
	bool hasFinished() const;

	void luaInitialize(int answer, int winner, const std::string& title, const std::string& subtitle, int quizId, const User& orgUser);
	void luaUpdate(const UserUpdateMessage& msg);
};


#endif

