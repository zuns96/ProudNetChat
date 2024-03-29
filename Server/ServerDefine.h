#pragma once
#define LOG_BUF_SIZE 512

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