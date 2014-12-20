#ifndef ___ASH_HPP___
#define ___ASH_HPP___

#include "user.hpp"
#include <boost/optional.hpp>
#include <boost/serialization/vector.hpp>
#include <memory>
#include <vector>
#include <wincore.h>

class ToolWindow;
class ControleScript;
class UserUpdateMessage;
class View;

class Ash : private CWinApp
{
private:
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

	struct UndoData
	{
		std::vector<User> *users;
		int nowMsgOrder;
		std::vector<int> *msgOrders;
		std::vector<PrevMsg> *prevMsgs;
		std::string luaVars;

	private:
		friend class boost::serialization::access;
		template<class Archive>
			void serialize(Archive& ar, const unsigned int version)
			{
				ar & users & nowMsgOrder & msgOrders & prevMsgs & luaVars;
			}
	};

	struct SaveData
	{
		int quizId;
		std::vector<User> *users;
		std::vector<std::string> *saves;
		int nowMsgOrder;
		std::vector<int> *msgOrders;
		std::vector<PrevMsg> *prevMsgs;
		std::string luaVars;

	private:
		friend class boost::serialization::access;
		template<class Archive>
			void serialize(Archive& ar, const unsigned int version)
			{
				ar & quizId & users & saves & nowMsgOrder & msgOrders & prevMsgs & luaVars;
			}
	};

	std::vector<User> users_;
	std::unique_ptr<ControleScript> controler_;
	std::unique_ptr<View> view_;
	std::unique_ptr<ToolWindow> log_;
	int quizId_, winner_;
	std::vector<std::string> saves_;
	int nowMsgOrder_;
	std::vector<int> msgOrders_;
	std::vector<PrevMsg> prevMsgs_;
	boost::optional<TmpData> tmpData_;

public:
	Ash();
	~Ash();

	// call from main
	void setScript(const std::string& filename);
	void run();

	// call from tool window
	void restoreUndoData();
	void setUserNames(const std::vector<std::string>& names);

	// call from lua controle script
	void initialize(int answer, int winner, const std::string& title, const std::string& subtitle, int quizId, const User& orgUser);
	void update(const UserUpdateMessage& msg);
	void storeUndoData();

	const User& getUser(int index) const;
	bool hasFinished() const;

	void writeUndoData(std::ostream& os);
	void readUndoData(std::istream& is);
	void writeSaveData(std::ostream& os);
	void readSaveData(std::istream& is);

private:
	void makeLuaVarsData(std::string& data);
	void setLuaVarsData(const std::string& data);
	void makeUndoData(UndoData& data);
	void setUndoData(const UndoData& data);
	void makeSaveData(SaveData& data);
	void setSaveData(const SaveData& data);
	void sendAllPrevMsgs();

	void getWLCount(int& winnerCount, int& loserCount) const;
	// クイズの終了確認を行う
	// 戻り値が
	enum class FINISH_STATUS {
		FIGHTING,	// まだ終了していない
		WIN_FINISH,	// 勝ち抜け者の制限到達による終了
		LOSE_FINISH	// 敗退者の制限到達による終了
	};
	FINISH_STATUS getFinishStatus() const;
};


#endif

