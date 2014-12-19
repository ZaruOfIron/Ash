#include "ash.hpp"
#include "lua_controle_script.hpp"
#include "copydata_view.hpp"
#include "tool_window.hpp"
#include "resource.h"
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <iostream>
#include <list>
#include <utility>
#include <sstream>

Ash::Ash()
	: users_(), controler_(), view_(new CopyDataView("AUAUA"))
{
	log_.reset(new ToolWindow(*this, ID_DIALOG));
	std::cout << "Ash::Ash()\t: finish construction" << std::endl;
}

Ash::~Ash()
{}

void Ash::setScript(const std::string& filename)
{
	controler_.reset(new LuaControleScript(*this, filename));
	std::cout << "Ash::setScript()\t: " + filename + "is being loaded" << std::endl;
	controler_->initialize();
	std::cout << "Ash::setScript()\t: complete loading" << std::endl;
}

void Ash::run()
{
	std::cout << "Ash::run()\t: start the system" << std::endl;
	log_->DoModeless();
	Run();
}

void Ash::writeSaveData(std::ostream& os)
{
	std::cout << "Ash::writeSaveData()\t: making and saving save data...   ";
	SaveData data;	makeSaveData(data);
	boost::archive::text_oarchive oa(os);
	oa << data;
	std::cout << "done." << std::endl;
}

void Ash::readSaveData(std::istream& is)
{
	std::cout << "Ash::readSaveData()\t: reading and setting save data" << std::endl;
	boost::archive::text_iarchive ia(is);
	SaveData data;	ia >> data;
	setSaveData(data);
	std::cout << "Ash::readSaveData()\t: done." << std::endl;
}

void Ash::undo()
{
	if(saves_.size() == 0)	return;

	std::cout << "Ash::undo()\t: start undo" << std::endl;
	std::istringstream iss(saves_.back());	saves_.pop_back();
	readSaveData(iss);
	std::cout << "Ash::undo()\t: complete undo." << std::endl;
}

void Ash::initialize(int answer, int winner, const std::string& title, const std::string& subtitle, int quizId, const User& orgUser)
{
	winner_ = winner;
	users_.resize(answer, orgUser);

	std::cout << "Ash::initialize()\t: send INIT" << std::endl
		<< "\t("  << answer << ", " << winner << ", " << title << ", " << subtitle << ", " << quizId << ", (" << orgUser.correct << ", " << orgUser.wrong << ", " << orgUser.score << "))" << std::endl
		<< "\t...   ";
	view_->initialize(answer, winner, title, subtitle, quizId, orgUser);
	std::cout << "done." << std::endl;

	nowMsgOrder_ = 0;
	for(int i = 0;i < answer;i++){
		//view_->sendUserModified(i, orgUser, 0x0f);
		prevMsgs_.push_back(PrevMsg(orgUser, 0x0f));
		msgOrders_.push_back(nowMsgOrder_++);
	}
}

void Ash::update(const UserUpdateMessage& msg)
{
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

	std::cout << "Ash::update()\t: send UM to " << msg.index << " (" << user.name << ", " << user.correct << ", " << user.wrong << ", " << user.score << ", " << modIndex << ")...   ";
	view_->sendUserModified(msg.index, user, modIndex);
	std::cout << "done." << std::endl;

	prevMsg.user = user;

	// 順番を登録する
	msgOrders_.at(msg.index) = nowMsgOrder_++;

	int winnerCount, loserCount;
	getWLCount(winnerCount, loserCount);

	prevMsg.info.clear();
	for(int id : msg.info){
		// 勝ち抜け(1)と敗退(2)は記録しておく
		int sendId = id;
		if(id == 1){
			user.status = User::STATUS::WINNER;
			sendId = 100 + ++winnerCount;
		}
		else if(id == 2){
			user.status = User::STATUS::LOSER;
			sendId = 200 + ++loserCount;
		}

		std::cout << "Ash::update()\t: send AI to " << msg.index << " (" << sendId << ")...   ";
		view_->sendInfo(sendId);
		std::cout << "done." << std::endl;
		prevMsg.info.push_back(sendId);
	}

	// 終われば、終了処理を行う
	if(winnerCount >= winner_){
		std::cout << "Ash::update()\t: WIN_FINISH" << std::endl;
		for(int i = 0;i < users_.size();i++){
			auto& user = users_.at(i);
			if(user.status != User::STATUS::FIGHTER)	continue;
			user.status = User::STATUS::LOSER;

			std::cout << "Ash::update()\t: send LOSE to " << i << " (" << user.name << ", " << user.correct << ", " << user.wrong << ", " << user.score << ", " << modIndex << ")...   ";
			view_->sendUserModified(i, user, 0);
			view_->sendInfo(200);
			std::cout << "done." << std::endl;
		}
	}
	else if(loserCount >= users_.size() - winner_){
		std::cout << "Ash::update()\t: LOSE_FINISH" << std::endl;
		for(int i = 0;i < users_.size();i++){
			auto& user = users_.at(i);
			if(user.status != User::STATUS::FIGHTER)	continue;
			user.status = User::STATUS::WINNER;

			std::cout << "Ash::update()\t: send WIN to " << i << " (" << user.name << ", " << user.correct << ", " << user.wrong << ", " << user.score << ", " << modIndex << ")...   ";
			view_->sendUserModified(i, user, 0);
			view_->sendInfo(100);
			std::cout << "done." << std::endl;
		}
	}

}

void Ash::save()
{
	std::cout << "Ash::save()\t: start to save" << std::endl;
	// 保存処理
	std::ostringstream oss;
	writeSaveData(oss);
	saves_.push_back(oss.str());
	std::cout << "Ash::save()\t: complete save" << std::endl;
}

const User& Ash::getUser(int index) const
{
	return users_.at(index);
}

bool Ash::hasFinished() const
{
	return getFinishStatus() != FINISH_STATUS::FIGHTING;
}

void Ash::makeSaveData(SaveData& data)
{
	data.users = &users_;
	data.prevMsgs = &prevMsgs_;
	data.msgOrders = &msgOrders_;

	std::ostringstream oss;	controler_->getSaveData(oss);
	data.luaVars = oss.str();
}

void Ash::setSaveData(const SaveData& data)
{
	users_ = *(data.users);
	delete data.users;

	msgOrders_ = *(data.msgOrders);
	delete data.msgOrders;

	prevMsgs_ = *(data.prevMsgs);
	delete data.prevMsgs;	// newed by boost::serialization
	// メッセージを送る順番を算出する
	std::list<std::pair<int, int>> order;
	for(int i = 0;i < msgOrders_.size();i++)	order.push_back(std::make_pair(i, msgOrders_.at(i)));
	order.sort([](const std::pair<int, int>& lhs, const std::pair<int, int>& rhs) { return lhs.second < rhs.second; });
	// 順番どおりに送っていく
	for(auto& item : order){
		int index = item.first;
		auto& msg = prevMsgs_.at(index);

		std::cout << "Ash::setSaveData()\t: send UM to " << index << " (" << msg.user.name << ", " << msg.user.correct << ", " << msg.user.wrong << ", " << msg.user.score << ", 0x0f)...   ";
		view_->sendUserModified(index, msg.user, 0x0f);
		std::cout << "done." << std::endl;
		for(int id : msg.info){
			std::cout << "Ash::setSaveData()\t: send AI to " << index << "(" << id << ")...   ";
			view_->sendInfo(id);
			std::cout << "done." << std::endl;
		}
	}

	std::istringstream iss(data.luaVars);
	controler_->restoreSaveData(iss);
}

void Ash::getWLCount(int& winnerCount, int& loserCount) const
{
	winnerCount = loserCount = 0;

	for(const auto& user : users_){
		if(user.status == User::STATUS::WINNER)	winnerCount++;
		else if(user.status == User::STATUS::LOSER)	loserCount++;
	}
}

Ash::FINISH_STATUS Ash::getFinishStatus() const
{
	int numWinner = 0, numLoser = 0;
	getWLCount(numWinner, numLoser);

	if(numWinner >= winner_)	return FINISH_STATUS::WIN_FINISH;
	else if(numLoser >= users_.size() - winner_)	return FINISH_STATUS::LOSE_FINISH;
	return FINISH_STATUS::FIGHTING;
}

