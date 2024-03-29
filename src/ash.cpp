#include "ash.hpp"
#include "lua_controle_script.hpp"
#include "copydata_view.hpp"
#include "tool_window.hpp"
#include "resource.h"
#include <boost/lexical_cast.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <utility>
#include <sstream>

Ash::Ash()
	: users_(), controler_(), view_(new CopyDataView("UAUA"))
{
	log_.reset(new ToolWindow(*this, ID_DIALOG));

	char buf[MAX_PATH];	::GetCurrentDirectory(sizeof(buf), buf);
	ashPath_ = buf;

	std::ifstream ifs(ashPath_ + "\\config.txt");
	if(ifs){
		std::string inp;	std::getline(ifs, inp);
		hasNameEditsEnabled_ = static_cast<bool>(boost::lexical_cast<int>(inp));
	}
	else{
		hasNameEditsEnabled_ = true;
	}

	std::cout << "Ash::Ash()\t: finish construction" << std::endl;
}

Ash::~Ash()
{}

void Ash::setScript(const std::string& filename)
{
	controler_.reset(new LuaControleScript(*this, filename));
	std::cout << "Ash::setScript()\t: " + filename + " is being loaded" << std::endl;
	controler_->initialize(hasNameEditsEnabled_);
	std::cout << "Ash::setScript()\t: complete loading" << std::endl;
}

void Ash::run()
{
	std::cout << "Ash::run()\t: start the system" << std::endl;
	log_->DoModeless();
	Run();
}

void Ash::setUserNames(const std::vector<std::string>& names)
{
	std::cout << "Ash::setUserNames()\t: setting user names" << std::endl;
	int size = std::min(users_.size(), names.size());

	for(int i = 0;i < size;i++){
		auto& user = users_.at(i);
		user.name = names.at(i);
		std::cout << "Ash::setUserNames()\t: send UM to " << i << " (" << user.name << ", " << user.correct << ", " << user.wrong << ", " << user.score << ", 1)...   ";
		sendMsgToView(i, user, 1, std::vector<int>());
		std::cout << "done." << std::endl;
	}
	std::cout << "Ash::setUserNames()\t: setting complete" << std::endl;
}

void Ash::initialize(int answer, int winner, const std::string& title, const std::string& subtitle, int quizId, const User& orgUser)
{
	quizId_ = quizId;
	winner_ = winner;

	std::cout << "Ash::initialize()\t: send INIT" << std::endl
		<< "\t("  << answer << ", " << winner << ", " << title << ", " << subtitle << ", " << quizId << ", (" << orgUser.correct << ", " << orgUser.wrong << ", " << orgUser.score << "))" << std::endl
		<< "\t...   ";
	view_->initialize(answer, winner, title, subtitle, quizId, orgUser);
	std::cout << "done." << std::endl;
	
	users_.resize(answer, orgUser);
	nowMsgOrder_ = 0;

	for(int i = 0;i < answer;i++){
		prevMsgs_.push_back(PrevMsg(orgUser, 0x0f));
		msgOrders_.push_back(nowMsgOrder_++);
	}
}

void Ash::update(const UserUpdateMessage& msg)
{
	auto& user = users_.at(msg.index);
	if(user.status != User::STATUS::FIGHTER)	// 対象外
		return;

	// 変更していく
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

	int winnerCount, loserCount;
	getWLCount(winnerCount, loserCount);

	std::vector<int> ais;
	for(int id : msg.info){
		// 勝ち抜け(1)と敗退(2)は記録しておく
		int sendId = id;
		if(id < 0){	// id to convert
			int kind = id * -1 / 10;
			if(kind == 1){	// win
				user.status = User::STATUS::WINNER;
				winnerCount++;
				sendId = 100;
				if(id == -11)	sendId += winnerCount;	// with order
			}
			else if(kind == 2){	// lose
				user.status = User::STATUS::LOSER;
				loserCount++;
				sendId = 200;
				if(id == -21)	sendId += loserCount;	// with order
			}
			else	assert(false);
		}

		ais.push_back(sendId);
	}

	// send
	sendMsgToView(msg.index, user, modIndex, ais);

	// 終われば、終了処理を行う
	if(winnerCount >= winner_){
		std::cout << "Ash::update()\t: WIN_FINISH" << std::endl;
		for(int i = 0;i < users_.size();i++){
			auto& user = users_.at(i);
			if(user.status != User::STATUS::FIGHTER)	continue;
			user.status = User::STATUS::LOSER;

			std::cout << "Ash::update()\t: send LOSE to " << i << " (" << user.name << ", " << user.correct << ", " << user.wrong << ", " << user.score << ", " << modIndex << ")...   ";
			sendMsgToView(i, user, 0, std::vector<int>({200}));
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
			sendMsgToView(i, user, 0, std::vector<int>({100}));
			std::cout << "done." << std::endl;
		}
	}

}

const User& Ash::getUser(int index) const
{
	return users_.at(index);
}

bool Ash::hasFinished() const
{
	return getFinishStatus() != FINISH_STATUS::FIGHTING;
}

void Ash::storeUndoData()
{
	std::cout << "Ash::storeUndoData()\t: start to save" << std::endl;
	// 保存処理
	std::ostringstream oss;
	writeUndoData(oss);
	saves_.push_back(oss.str());
	std::cout << "Ash::storeUndoData()\t: complete save" << std::endl;
}

void Ash::restoreUndoData()
{
	if(saves_.size() == 0)	return;

	std::cout << "Ash::restoreUndoData()\t: start undo" << std::endl;
	std::istringstream iss(saves_.back());	saves_.pop_back();
	readUndoData(iss);
	std::cout << "Ash::restoreUndoData()\t: complete undo." << std::endl;

}

void Ash::writeSaveData(std::ostream& os)
{
	std::cout << "Ash::writeSaveData()\t: making and saving save data" << std::endl;
	SaveData data;	makeSaveData(data);
	boost::archive::text_oarchive oa(os);
	oa << data;
	std::cout << "Ash::writeSaveData()\t: complete" << std::endl;

}

void Ash::readSaveData(std::istream& is)
{
	std::cout << "Ash::readSaveData()\t: reading and setting save data" << std::endl;
	boost::archive::text_iarchive ia(is);
	SaveData data;	ia >> data;
	setSaveData(data);
	std::cout << "Ash::readSaveData()\t: done." << std::endl;
}

void Ash::writeSaveFile(const std::string& filename)
{
	std::cout << "Ash::writeSaveFile()\t: writing save file" << std::endl;

	std::ostringstream oss;	writeSaveData(oss);

	// ファイルを作る
	DWORD createFlag = ::PathFileExists(filename.c_str()) ? TRUNCATE_EXISTING : CREATE_NEW;
	HANDLE hFile = ::CreateFile(
		filename.c_str(),
		GENERIC_WRITE,
		0,
		NULL,
		createFlag,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if(hFile == INVALID_HANDLE_VALUE){
		std::cout << "Ash::writeTmpFile()\t: Can't create file" << std::endl;
		return;
	}
	std::unique_ptr<std::remove_pointer<HANDLE>::type, decltype(&::CloseHandle)> file(hFile, ::CloseHandle);

	// 書き込む
	std::string dataStr = oss.str();
	DWORD size;
	::WriteFile(file.get(), dataStr.c_str(), dataStr.size(), &size, NULL);

	std::cout << "Ash::writeSaveFile()\t: complete" << std::endl;

}

void Ash::readSaveFile(const std::string& filename)
{
	std::cout << "Ash::readSaveFile()\t: reading save file" << std::endl;

	// ファイルを開く
	HANDLE hFile = ::CreateFile(
			filename.c_str(),
			GENERIC_READ,
			0,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
	assert(hFile != INVALID_HANDLE_VALUE);
	std::unique_ptr<std::remove_pointer<HANDLE>::type, decltype(&::CloseHandle)> file(hFile, ::CloseHandle);

	// 読み込む
	DWORD size = ::GetFileSize(file.get(), NULL);
	assert(size != -1);
	std::vector<char> buffer(size + 1, '\0');
	DWORD readSize;
	::ReadFile(file.get(), buffer.data(), size, &readSize, NULL);
	buffer.at(size) = '\0';

	std::istringstream iss(std::string(buffer.begin(), buffer.end()));
	readSaveData(iss);
}

void Ash::saveTmpFile()
{
	writeSaveFile(ashPath_ + "\\.ashtmp.asd");
}

void Ash::sendMsgToView(int index, const User& user, int modIndex, const std::vector<int>& ais)
{
	auto& prevMsg = prevMsgs_.at(index);
	prevMsg.user = user;
	prevMsg.modIndex = modIndex;
	prevMsg.info = ais;

	msgOrders_.at(index) = nowMsgOrder_++;

	std::cout << "Ash::sendMsgToView()\t: sending info to " << index << std::endl;
	std::cout << "Ash::sendMsgToView()\t: UM: (" << index << ", " << user.name << ", " << user.correct << ", " << user.wrong << ", " << user.score << ", " << modIndex << ")" << std::endl;
	view_->sendUserModified(index, user, modIndex);
	if(ais.size()){
		std::cout << "Ash::sendMsgToView()\t: AI: ";
		for(int ai : ais){
			view_->sendInfo(ai);
			std::cout << " ";
		}
	}
	std::cout << std::endl
		<< "Ash::sendMsgToView()\t: complete" << std::endl;
}

void Ash::makeLuaVarsData(std::string& data)
{
	std::ostringstream oss;	controler_->getSaveData(oss);
	data = oss.str();
}

void Ash::setLuaVarsData(const std::string& data)
{
	std::istringstream iss(data);
	controler_->restoreSaveData(iss);
}

void Ash::makeUndoData(UndoData& data)
{
	data.users = &users_;
	data.nowMsgOrder = nowMsgOrder_;
	data.msgOrders = &msgOrders_;
	data.prevMsgs = &prevMsgs_;
	makeLuaVarsData(data.luaVars);
}

void Ash::setUndoData(const UndoData& data)
{
	users_ = *(data.users);
	delete data.users;

	nowMsgOrder_ = data.nowMsgOrder;

	msgOrders_ = *(data.msgOrders);
	delete data.msgOrders;

	prevMsgs_ = *(data.prevMsgs);
	delete data.prevMsgs;	// newed by boost::serialization
	sendAllPrevMsgs();

	setLuaVarsData(data.luaVars);
}

void Ash::writeUndoData(std::ostream& os)
{
	std::cout << "Ash::writeUndoData()\t: making and saving undo data...   ";
	UndoData data;	makeUndoData(data);
	boost::archive::text_oarchive oa(os);
	oa << data;
	std::cout << "done." << std::endl;
}

void Ash::readUndoData(std::istream& is)
{
	std::cout << "Ash::readUndoData()\t: reading and setting undo data" << std::endl;
	boost::archive::text_iarchive ia(is);
	UndoData data;	ia >> data;
	setUndoData(data);
	std::cout << "Ash::readSaveData()\t: done." << std::endl;
}

void Ash::makeSaveData(SaveData& data)
{
	data.quizId = quizId_;
	data.users = &users_;
	data.saves = &saves_;
	data.nowMsgOrder = nowMsgOrder_;
	data.msgOrders = &msgOrders_;
	data.prevMsgs = &prevMsgs_;
	makeLuaVarsData(data.luaVars);
}

void Ash::setSaveData(const SaveData& data)
{
	users_ = *(data.users);	delete data.users;
	saves_ = *(data.saves);	delete data.saves;
	nowMsgOrder_ = data.nowMsgOrder;
	msgOrders_ = *(data.msgOrders);	delete data.msgOrders;
	prevMsgs_ = *(data.prevMsgs);	delete data.prevMsgs;
	setLuaVarsData(data.luaVars);

	sendAllPrevMsgs();
}

void Ash::sendAllPrevMsgs()
{
	// メッセージを送る順番を算出する
	std::list<std::pair<int, int>> order;
	for(int i = 0;i < msgOrders_.size();i++)	order.push_back(std::make_pair(i, msgOrders_.at(i)));
	order.sort([](const std::pair<int, int>& lhs, const std::pair<int, int>& rhs) { return lhs.second < rhs.second; });

	// 順番どおりに送っていく
	for(auto& item : order){
		int index = item.first;
		auto& msg = prevMsgs_.at(index);

		std::cout << "Ash::sendAllPrevMsgs()\t: send UM to " << index << " (" << msg.user.name << ", " << msg.user.correct << ", " << msg.user.wrong << ", " << msg.user.score << ", 0x0f)...   ";
		view_->sendUserModified(index, msg.user, 0x0f);
		std::cout << "done." << std::endl;
		for(int id : msg.info){
			std::cout << "Ash::sendAllPrevMsgs()\t: send AI to " << index << "(" << id << ")...   ";
			view_->sendInfo(id);
			std::cout << "done." << std::endl;
		}
	}
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

