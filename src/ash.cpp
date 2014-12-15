#include "ash.hpp"
#include "lua_controle_script.hpp"
#include "copydata_view.hpp"

//#include <iostream>

Ash::Ash()
	: users_(), controler_(), view_(new CopyDataView("AUAUA"))
{
}

void Ash::setScript(const std::string& filename)
{
	controler_.reset(new LuaControleScript(*this, filename));
	controler_->initialize();
}

void Ash::run()
{
	controler_->run();
}

const User& Ash::getUser(int index)
{
	return users_.at(index);
}

void Ash::luaInitialize(int answer, int winner, const std::string& title, const std::string& subtitle, int quizId, const User& orgUser)
{
	winner_ = winner;
	users_.resize(answer, orgUser);

	view_->initialize(answer, winner, title, subtitle, quizId);

	for(int i = 0;i < answer;i++){
		view_->sendUserModified(i, orgUser, 0x0f);
	}
}

void Ash::luaUpdate(const UserUpdateMessage& msg)
{
/*	std::cout << "Ash::luaUpdate() : index = " << msg.index << std::endl;
	if(msg.name)	std::cout << "\tname    = " << *(msg.name) << std::endl;
	if(msg.correct)	std::cout << "\tcorrect = " << *(msg.correct) << std::endl;
	if(msg.wrong)	std::cout << "\twrong   = " << *(msg.wrong) << std::endl;
	if(msg.score)	std::cout << "\tscore   = " << *(msg.score) << std::endl;
	std::cout << "\t";
	for(int id : msg.info)	std::cout << id << " ";
	std::cout << std::endl;
	*/

	auto& user = users_.at(msg.index);
	int modIndex = 0;
	if(msg.name){
		user.name = *(msg.name);
		modIndex |= 1 << 0;
	}
	if(msg.correct){
		user.correct = *(msg.correct);
		modIndex |= 1 << 1;
	}
	if(msg.wrong){
		user.wrong = *(msg.wrong);
		modIndex |= 1 << 2;
	}
	if(msg.score){
		user.score = *(msg.score);
		modIndex |= 1 << 3;
	}

	view_->sendUserModified(msg.index, user, modIndex);

	for(int id : msg.info){
		// 勝ち抜け(1)と敗退(2)は記録しておく
		if(id == 1)	user.status = User::STATUS::WINNER;
		else if(id == 2)	user.status == User::STATUS::LOSER;

		view_->sendInfo(id);
	}

	// 終われば、終了処理を行う
	FINISH_STATUS status = getFinishStatus();
	if(status == FINISH_STATUS::WIN_FINISH){
		for(int i = 0;i < users_.size();i++){
			auto& user = users_.at(i);
			if(user.status == User::STATUS::FIGHTER)	user.status = User::STATUS::LOSER;
			view_->sendUserModified(i, user, 0);
			view_->sendInfo(2);
		}
	}
	else if(status == FINISH_STATUS::LOSE_FINISH){
		for(int i = 0;i < users_.size();i++){
			auto& user = users_.at(i);
			if(user.status == User::STATUS::FIGHTER)	user.status = User::STATUS::WINNER;
			view_->sendUserModified(i, user, 0);
			view_->sendInfo(1);
		}
	}
}

Ash::FINISH_STATUS Ash::getFinishStatus() const
{
	int numWinner = 0, numLoser = 0;
	for(auto& user : users_){
		if(user.status == User::STATUS::WINNER)	numWinner++;
		else if(user.status == User::STATUS::LOSER)	numLoser++;
	}

	if(numWinner >= winner_)	return FINISH_STATUS::WIN_FINISH;
	else if(numLoser >= users_.size() - winner_)	return FINISH_STATUS::LOSE_FINISH;
	return FINISH_STATUS::FIGHTING;
}

