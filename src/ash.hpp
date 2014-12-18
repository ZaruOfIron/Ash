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
	std::unique_ptr<LogWindow> log_;
	int winner_;
	std::string saveFileName_;
	std::vector<std::string> saves_;

	struct PrevMsg
	{
		User user;
		int modIndex;
		std::vector<int> info;

		PrevMsg(){}
		PrevMsg(const User& user_, int modIndex_)
			: user(user_), modIndex(modIndex_){}

		private:
		friend class boost::serialization::access;
		template<class Archive>
			void serialize(Archive& ar, const unsigned int version)
			{
				ar & user & modIndex & info;
			}

	};
	std::vector<PrevMsg> prevMsgs_;
	int nowMsgOrder_;
	std::vector<int> msgOrders_;

	struct SaveData
	{
		std::vector<User> *users;
		std::vector<PrevMsg> *prevMsgs;
		std::string luaVars;

	private:
		friend class boost::serialization::access;
		template<class Archive>
			void serialize(Archive& ar, const unsigned int version)
			{
				ar & users & prevMsgs & luaVars;
			}
	};

	void makeSaveData(SaveData& data);
	void setSaveData(const SaveData& data);

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
	void save();
	void undo();

	const User& getUser(int index) const;
	bool hasFinished() const;

	void luaInitialize(int answer, int winner, const std::string& title, const std::string& subtitle, int quizId, const User& orgUser);
	void luaUpdate(const UserUpdateMessage& msg);
};


#endif

