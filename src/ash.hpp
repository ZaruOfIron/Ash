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

	// �N�C�Y�̏I���m�F���s��
	// �߂�l��
	enum class FINISH_STATUS {
		FIGHTING,	// �܂��I�����Ă��Ȃ�
		WIN_FINISH,	// ���������҂̐������B�ɂ��I��
		LOSE_FINISH	// �s�ގ҂̐������B�ɂ��I��
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

