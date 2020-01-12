#pragma once
#include <ProudNetServer.h>
#include "../Common/Vars.h"

#include "../Common/C2S_common.cpp"
#include "../Common/S2C_common.cpp"

#include "../Common/C2S_stub.h"
#include "../Common/C2S_stub.cpp"

#include "../Common/S2C_proxy.h"
#include "../Common/S2C_proxy.cpp"

class C2SStub : public C2S::Stub
{
public:
	DECRMI_C2S_OnLogOn;
	DECRMI_C2S_Chat;
};

class SUser : public User
{
public :
	Proud::HostID m_hostID;

	void SetUserData(const Proud::String &id, Proud::HostID hostID);
};

enum WINDOW_ID : int
{
	EXIT_BUTTON = 0,
	KICK_BUTTON = 1,
	CHAT_LIST = 2,
	LOG_LIST = 3,
};

#define WINDOW_ID_EXITBTN WINDOW_ID::EXIT_BUTTON
#define WINDOW_ID_KICKBTN WINDOW_ID::KICK_BUTTON
#define WINDOW_ID_CHATLIST WINDOW_ID::CHAT_LIST
#define WINDOW_ID_LOGLIST WINDOW_ID::LOG_LIST