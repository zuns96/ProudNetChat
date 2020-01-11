#pragma once
#include<ProudNetClient.h>
#include<Windows.h>
#include <iostream>

#include "../Common/Vars.h"
#include "../Common/C2S_common.cpp"
#include "../Common/C2S_proxy.h"
#include "../Common/C2S_proxy.cpp"

#include "../Common/S2C_common.cpp"
#include "../Common/S2C_proxy.h"
#include "../Common/S2C_stub.cpp"

enum WINDOW_ID : int
{
	EXIT_BUTTON = 0,
	KICK_BUTTON = 1,
	USER_LIST = 2,
	LOG_LIST = 3,
};

#define WINDOW_ID_EXITBTN WINDOW_ID::EXIT_BUTTON
#define WINDOW_ID_KICKBTN WINDOW_ID::KICK_BUTTON
#define WINDOW_ID_USERLIST WINDOW_ID::USER_LIST
#define WINDOW_ID_LOGLIST WINDOW_ID::LOG_LIST