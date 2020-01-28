#pragma once
#include <ProudNetClient.h>
#include <Windows.h>
#include <iostream>
#include <string>

#include "../Common/Packets.h"

#include "../Common/C2S_common.cpp"
#include "../Common/C2S_proxy.h"
#include "../Common/C2S_proxy.cpp"

#include "../Common/S2C_common.cpp"
#include "../Common/S2C_proxy.h"
#include "../Common/S2C_stub.cpp"

#pragma region enum
enum WINDOW_ID : int
{
	LOGIN_BUTTON = 0,
	EXIT_BUTTON,
	SEND_BUTTON,
	CHAT_LIST,
	LOG_LIST,
	TEXT_EDIT,
	ID_EDIT,
	PW_EDIT,
};

#define WINDOW_ID_LOGINBTN WINDOW_ID::LOGIN_BUTTON
#define WINDOW_ID_EXITBTN WINDOW_ID::EXIT_BUTTON
#define WINDOW_ID_SENDBTN WINDOW_ID::SEND_BUTTON
#define WINDOW_ID_CHATLIST WINDOW_ID::CHAT_LIST
#define WINDOW_ID_LOGLIST WINDOW_ID::LOG_LIST
#define WINDOW_ID_TEXTEDIT WINDOW_ID::TEXT_EDIT
#define WINDOW_ID_IDEDIT WINDOW_ID::ID_EDIT
#define WINDOW_ID_PWEDIT WINDOW_ID::PW_EDIT
#pragma endregion enum

class S2CStub : public S2C::Stub
{
public:
	DECRMI_S2C_Recv_Rpy_Login;
	DECRMI_S2C_Recv_Rpy_Chat;
	DECRMI_S2C_Recv_Rpy_System_Chat;
};

const char * c_ip = "127.0.0.1";

HWND hIDEdit;
HWND hPWEdit;
HWND hUserList;
HWND hLogList;
HWND hTextEdit;

User user;
bool isLogin = false;
C2S::Proxy g_C2SProxy;

bool isConnected = false;
volatile bool keepWorkerThread = true;

S2CStub g_S2CStub;
HINSTANCE g_hInst;

WNDCLASS WndLoginClass;
LPCTSTR lpszLoginClass = TEXT("Login");

WNDCLASS WndChatClass;
LPCTSTR lpszClass = TEXT("chat_client");

int iLoginWidth = 400;
int iLoginHeight = 200;

int iChatClientWidth = 500;
int iChatClientHeight = 600;

extern Proud::Thread workerThread;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow);
LRESULT CALLBACK ChatWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK LoginWndProc(HWND, UINT, WPARAM, LPARAM);
void CreatePushButton(LPCTSTR text, int x, int y, int width, int height, HWND hWnd, int id, bool isDefaultPush);
HWND CreateListBox(int x, int y, int width, int height, HWND hWnd, int id);
HWND CreateTextEdit(int x, int y, int width, int height, HWND hWnd, int id);
void Draw(HWND hWnd);
void Quit(HWND hWnd);