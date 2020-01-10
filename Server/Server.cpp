#define _CRT_SECURE_NO_WARNINGS

#include "Server.h"

using namespace std;
using namespace Proud;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void CreatePushButton(LPCTSTR text, int x, int y, int width, int height, HWND hWnd, int id);
HWND CreateListBox(int x, int y, int width, int height, HWND hWnd, int id);
HWND CreateTextEdit(int x, int y, int width, int height, HWND hWnd, int id);
void Draw(HWND hWnd);

HINSTANCE g_hInst;
HWND hUserList;
HWND hLogList;

LPCTSTR lpszClass = TEXT("chat_server");

int iWidth = 400;
int iHeight = 600;

class C2SStub : public C2S::Stub
{
public:
	DECRMI_C2S_Chat;
};

C2SStub g_C2SStub;
S2C::Proxy g_S2CProxy;
HostID g_groupHostID = HostID_None;
CNetServer * srv = NULL;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	srv = CNetServer::Create();

	srv->OnClientJoin = [](CNetClientInfo * clientInfo)
	{
		TCHAR buf[256];
		wsprintf(buf, TEXT("Client %d connected."), clientInfo->m_HostID);
		SendMessage(hLogList, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
	};

	srv->OnClientLeave = [](CNetClientInfo * clientInfo, ErrorInfo * errorInfo, const ByteArray& comment)
	{
		TCHAR buf[256];
		wsprintf(buf, TEXT("Client %d disconnected."), clientInfo->m_HostID);
		SendMessage(hLogList, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
	};

	srv->OnError = [](ErrorInfo * errorInfo)
	{
		TCHAR buf[256];
		wsprintf(buf, TEXT("OnError : %s", StringT2A(errorInfo->ToString()).GetString()));
		SendMessage(hLogList, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
	};

	srv->OnWarning = [](ErrorInfo * errorInfo)
	{
		TCHAR buf[256];
		wsprintf(buf, TEXT("OnWarning : %s", StringT2A(errorInfo->ToString()).GetString()));
		SendMessage(hLogList, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
	};

	srv->OnInformation = [](ErrorInfo * errorInfo)
	{
		TCHAR buf[256];
		wsprintf(buf, TEXT("OnInformation : %s", StringT2A(errorInfo->ToString()).GetString()));
		SendMessage(hLogList, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
	};

	srv->OnException = [](const Exception &e)
	{
		TCHAR buf[256];
		wsprintf(buf, TEXT("OnException : %s"), e.what());
		SendMessage(hLogList, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
	};

	srv->AttachStub(&g_C2SStub);
	srv->AttachProxy(&g_S2CProxy);

	CStartServerParameter p1;
	p1.m_protocolVersion = g_Version;
	p1.m_tcpPorts.Add(g_ServerPort);

	try
	{
		srv->Start(p1);
	}
	catch (Exception &e)
	{
		TCHAR buf[256];
		wsprintf(buf, TEXT("Server start failed : %s"), e.what());
		SendMessage(hLogList, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
		return 0;
	}

	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);
	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, iWidth, iHeight, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, NULL, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_CREATE:
		CreatePushButton(TEXT("닫기"), 10, iHeight - 85, 100, 25, hWnd, WINDOW_ID_EXITBTN);
		CreatePushButton(TEXT("추방하기"), iWidth - 100 - 25, iHeight - 85, 100, 25, hWnd, WINDOW_ID_KICKBTN);
		hUserList = CreateListBox(10, 30, iWidth - 35, iHeight - 390, hWnd, WINDOW_ID_USERLIST);
		hLogList = CreateListBox(10, 270, iWidth - 35, iHeight - 390, hWnd, WINDOW_ID_LOGLIST);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case WINDOW_ID_EXITBTN:
			DestroyWindow(hWnd);
			break;
		case WINDOW_ID_KICKBTN:
			MessageBox(hWnd, TEXT("추방"), TEXT("추방하기 버튼"), MB_OK);
			break;
		case WINDOW_ID_USERLIST:
			switch (HIWORD(wParam))
			{
			case LBN_SELCHANGE:
				int i = SendMessage(hUserList, LB_GETCURSEL, 0, 0);
				break;
			}
			break;
		case WINDOW_ID_LOGLIST:
			switch (HIWORD(wParam))
			{
			case LBN_SELCHANGE:
				int i = SendMessage(hLogList, LB_GETCURSEL, 0, 0);
				break;
			}
			break;
		default:
			break;
		}
		return 0;
	case WM_DESTROY:
		srv->Stop();
		delete srv;
		srv = NULL;
		PostQuitMessage(0);
		return 0;
	case WM_PAINT:
		Draw(hWnd);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

void CreatePushButton(LPCTSTR text, int x, int y, int width, int height, HWND hWnd, int id)
{
	CreateWindow(TEXT("button"), text, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, x, y, width, height, hWnd, (HMENU)id, g_hInst, NULL);
}

HWND CreateListBox(int x, int y, int width, int height, HWND hWnd, int id)
{
	return CreateWindow(TEXT("listbox"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_HSCROLL | WS_VSCROLL | LBS_NOTIFY, x, y, width, height, hWnd, (HMENU)id, g_hInst, NULL);
}

void Draw(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);

	TextOut(hdc, 10, 10, TEXT("접속자 목록"), 6);
	TextOut(hdc, 10, 250, TEXT("메시지창"), 4);

	EndPaint(hWnd, &ps);
}

DEFRMI_C2S_Chat(C2SStub)
{
	TCHAR buf[512];
	wsprintf(buf, TEXT("message received : hostID : %d, txt : %s"), remote, txt);
	SendMessage(hLogList, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	// Echo chatting message which received from server to client.
	g_S2CProxy.ShowChat(remote, RmiContext::ReliableSend, txt);
	return true;
}