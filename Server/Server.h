#pragma once
#include <ProudNetServer.h>
#include "../Common/Packets.h"

#include "../Common/C2S_common.cpp"
#include "../Common/S2C_common.cpp"

#include "../Common/C2S_stub.h"
#include "../Common/C2S_stub.cpp"

#include "../Common/S2C_proxy.h"
#include "../Common/S2C_proxy.cpp"


class C2SStub : public C2S::Stub
{
public:
	DECRMI_C2S_Send_Req_Login;
	DECRMI_C2S_Send_Req_Chat;
};

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow);
LRESULT CALLBACK ChatWndProc(HWND, UINT, WPARAM, LPARAM);
void CreatePushButton(LPCTSTR text, int x, int y, int width, int height, HWND hWnd, int id);
HWND CreateListBox(int x, int y, int width, int height, HWND hWnd, int id);
void Draw(HWND hWnd);
void Quit(HWND hWnd);

HINSTANCE g_hInst;
HWND hUserList;
HWND hLogList;

LPCTSTR lpszClass = TEXT("chat_server");

int iChatClientWidth = 400;
int iChatClientHeight = 600;

C2SStub g_C2SStub;
S2C::Proxy g_S2CProxy;

int curSelectUserIdx = -1;