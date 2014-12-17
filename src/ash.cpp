#include "ash.hpp"
#include "lua_controle_script.hpp"
#include "copydata_view.hpp"
#include "resource.h"
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/date_time.hpp>
#include <boost/format.hpp>
#include <list>
#include <utility>
#include <sstream>

//#include <iostream>

std::string getDateTimeString()
{
	auto f = new boost::posix_time::time_facet("%y%m%d_%H%M%S");
	std::ostringstream oss;
	oss.imbue(std::locale(oss.getloc(), f));
	oss << boost::posix_time::second_clock::local_time();
	return oss.str();
}

Ash::Ash()
	: users_(), controler_(), view_(new CopyDataView("AUAUA")),
	saveFileName_((boost::format("%1%.log") % getDateTimeString()).str())
{
	log_.reset(new LogWindow(*this, ID_DIALOG));
}

void Ash::setScript(const std::string& filename)
{
	controler_.reset(new LuaControleScript(*this, filename));
	controler_->initialize();
}

void Ash::run()
{
	log_->DoModeless();
	Run();
}

void Ash::save()
{
	log_->write("Ash::save()");

	// 保存処理
	SaveData save;
	save.users = &users_;
	save.prevMsgs = &prevMsgs_;

	std::ostringstream oss;	controler_->getSaveData(oss);
	save.luaVars = oss.str();

	oss.str("");
	boost::archive::text_oarchive oa(oss);
	oa << save;
	saves_.push_back(oss.str());
}

void Ash::undo()
{
	if(saves_.size() == 0)	return;

	log_->write("Ash::undo()");

	std::istringstream iss(saves_.back());	saves_.pop_back();
	boost::archive::text_iarchive ia(iss);
	SaveData save;	ia >> save;

	users_ = *(save.users);
	delete save.users;

	prevMsgs_ = *(save.prevMsgs);
	delete save.prevMsgs;	// newed by boost::serialization
	// メッセージを送る順番を算出する
	std::list<std::pair<int, int>> order;
	for(int i = 0;i < msgOrders_.size();i++)	order.push_back(std::make_pair(i, msgOrders_.at(i)));
	order.sort([](const std::pair<int, int>& lhs, const std::pair<int, int>& rhs) { return lhs.second < rhs.second; });
	// 順番どおりに送っていく
	for(auto& item : order){
		int index = item.first;
		auto& msg = prevMsgs_.at(index);

		view_->sendUserModified(index, msg.user, msg.modIndex);
		for(int id : msg.info)	view_->sendInfo(id);
	}

	iss.str(save.luaVars);	iss.clear(std::istringstream::goodbit);
	controler_->restoreSaveData(iss);
}

const User& Ash::getUser(int index) const
{
	return users_.at(index);
}

bool Ash::hasFinished() const
{
	return getFinishStatus() != FINISH_STATUS::FIGHTING;
}

void Ash::luaInitialize(int answer, int winner, const std::string& title, const std::string& subtitle, int quizId, const User& orgUser)
{
	winner_ = winner;
	users_.resize(answer, orgUser);

	view_->initialize(answer, winner, title, subtitle, quizId);

	nowMsgOrder_ = 0;
	for(int i = 0;i < answer;i++){
		view_->sendUserModified(i, orgUser, 0x0f);
		prevMsgs_.push_back(PrevMsg(orgUser, 0x0f));
		msgOrders_.at(i) = nowMsgOrder_++;
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
	if(user.status != User::STATUS::FIGHTER)	// 対象外
		return;

	auto& prevMsg = prevMsgs_.at(msg.index);
	// 変更していく
	int& modIndex = prevMsg.modIndex;
	modIndex = 0;
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
	prevMsg.user = user;

	prevMsg.info.clear();
	for(int id : msg.info){
		// 勝ち抜け(1)と敗退(2)は記録しておく
		if(id == 1)	user.status = User::STATUS::WINNER;
		else if(id == 2)	user.status = User::STATUS::LOSER;

		view_->sendInfo(id);
		prevMsg.info.push_back(id);
	}

	// 終われば、終了処理を行う
	FINISH_STATUS status = getFinishStatus();
	if(status == FINISH_STATUS::WIN_FINISH){
		for(int i = 0;i < users_.size();i++){
			auto& user = users_.at(i);
			if(user.status != User::STATUS::FIGHTER)	continue;
			user.status = User::STATUS::LOSER;
			view_->sendUserModified(i, user, 0);
			view_->sendInfo(2);
		}
	}
	else if(status == FINISH_STATUS::LOSE_FINISH){
		for(int i = 0;i < users_.size();i++){
			auto& user = users_.at(i);
			if(user.status != User::STATUS::FIGHTER)	continue;
			user.status = User::STATUS::WINNER;
			view_->sendUserModified(i, user, 0);
			view_->sendInfo(1);
		}
	}

	// 順番を登録する
	msgOrders_.at(msg.index) = nowMsgOrder_++;
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

