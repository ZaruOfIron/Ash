#include "copydata_view.hpp"
#include <boost/format.hpp>
#include <cstring>
#include <memory>

CopyDataView::CopyDataView(const std::string& targetTitle)
{
	hTarget_ = ::FindWindow(NULL, targetTitle.c_str());
	assert(hTarget_ != NULL);
}

bool CopyDataView::isTargetValid()
{
	return hTarget_ != NULL && ::IsWindow(hTarget_);
}

#include <iostream>
void CopyDataView::sendData(int id, const std::string& str)
{
	std::cout << id << ": " << str << std::endl;

	// const‰ñ”ð
	std::shared_ptr<char> data(new char[str.size()], std::default_delete<char[]>());
	std::memcpy(data.get(), str.c_str(), str.size());

	COPYDATASTRUCT msg;
	msg.dwData = id;
	msg.cbData = str.size();
	msg.lpData = data.get();

	::SendMessage(
		hTarget_,
		WM_COPYDATA,
		NULL,
		reinterpret_cast<LPARAM>(&msg));
}

void CopyDataView::initialize(int answerNumber, int winnerNumber, const std::string& title, const std::string& subtitle, int quizId, const User& orgUser)
{
	// Initialize Phase
	sendData(101, (boost::format("%d,%d,%s,%s,%d,%d,%d,%d") % answerNumber % winnerNumber % title.c_str() % subtitle.c_str() % quizId % orgUser.correct % orgUser.wrong % orgUser.score).str());
}

void CopyDataView::sendUserModified(int index, const User& user, int modIndex)
{
	sendData(201, (boost::format("%d,%s,%d,%d,%d,%d") % index % user.name % user.correct % user.wrong % user.score % modIndex).str());
}

void CopyDataView::sendInfo(int id)
{
	sendData(301, (boost::format("%d") % id).str());
}

void CopyDataView::sendInfo(const std::string& str)
{
	sendData(401, str);
}

