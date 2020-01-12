#pragma once
#include<ProudNetClient.h>
#include<Windows.h>
#include <iostream>
#include <string>

#include "../Common/Vars.h"
#include "../Common/C2S_common.cpp"
#include "../Common/C2S_proxy.h"
#include "../Common/C2S_proxy.cpp"

#include "../Common/S2C_common.cpp"
#include "../Common/S2C_proxy.h"
#include "../Common/S2C_stub.cpp"

enum WINDOW_ID : int
{
	LOGIN_BUTTON = 0,
	EXIT_BUTTON,
	KICK_BUTTON,
	CHAT_LIST,
	LOG_LIST,
	TEXT_EDIT,
	ID_EDIT,
	PW_EDIT,
};

#define WINDOW_ID_LOGINBTN WINDOW_ID::LOGIN_BUTTON
#define WINDOW_ID_EXITBTN WINDOW_ID::EXIT_BUTTON
#define WINDOW_ID_KICKBTN WINDOW_ID::KICK_BUTTON
#define WINDOW_ID_CHATLIST WINDOW_ID::CHAT_LIST
#define WINDOW_ID_LOGLIST WINDOW_ID::LOG_LIST
#define WINDOW_ID_TEXTEDIT WINDOW_ID::TEXT_EDIT
#define WINDOW_ID_IDEDIT WINDOW_ID::ID_EDIT
#define WINDOW_ID_PWEDIT WINDOW_ID::PW_EDIT