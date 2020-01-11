#define _CRT_SECURE_NO_WARNINGS
#include "ServerDefine.h"
#include "Server.h"

using namespace std;
using namespace Proud;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void CreatePushButton(LPCTSTR text, int x, int y, int width, int height, HWND hWnd, int id);
HWND CreateListBox(int x, int y, int width, int height, HWND hWnd, int id);
void Draw(HWND hWnd);
void Quit(HWND hWnd);

HINSTANCE g_hInst;
HWND hUserList;
HWND hLogList;

LPCTSTR lpszClass = TEXT("chat_server");

int iWidth = 400;
int iHeight = 600;

CNetServer * srv = NULL;
CFastArray<SUser> userList;

C2SStub g_C2SStub;
S2C::Proxy g_S2CProxy;
HostID g_groupHostID = HostID_None;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	srv = CNetServer::Create();

	srv->OnClientJoin = [](CNetClientInfo * clientInfo)
	{
		TCHAR buf[LOG_BUF_SIZE];
		wsprintf(buf, TEXT("Client %d connected."), clientInfo->m_HostID);
		SendMessage(hLogList, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
	};

	srv->OnClientLeave = [](CNetClientInfo * clientInfo, ErrorInfo * errorInfo, const ByteArray& comment)
	{
		TCHAR buf[LOG_BUF_SIZE];
		wsprintf(buf, TEXT("Client %d disconnected."), clientInfo->m_HostID);
		SendMessage(hLogList, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
	};

	srv->OnError = [](ErrorInfo * errorInfo)
	{
		TCHAR buf[LOG_BUF_SIZE];
		wsprintf(buf, TEXT("OnError : %s", StringT2A(errorInfo->ToString()).GetString()));
		SendMessage(hLogList, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
	};

	srv->OnWarning = [](ErrorInfo * errorInfo)
	{
		TCHAR buf[LOG_BUF_SIZE];
		wsprintf(buf, TEXT("OnWarning : %s", StringT2A(errorInfo->ToString()).GetString()));
		SendMessage(hLogList, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
	};

	srv->OnInformation = [](ErrorInfo * errorInfo)
	{
		TCHAR buf[LOG_BUF_SIZE];
		wsprintf(buf, TEXT("OnInformation : %s", StringT2A(errorInfo->ToString()).GetString()));
		SendMessage(hLogList, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
	};

	srv->OnException = [](const Exception &e)
	{
		TCHAR buf[LOG_BUF_SIZE];
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
		char buf[LOG_BUF_SIZE];
		sprintf(buf, "Server start failed : %s", e.what());
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
			Quit(hWnd);
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
		PostQuitMessage(0);
		return 0;
	case WM_CLOSE:
		Quit(hWnd);
		return 0;
	case WM_PAINT:
		Draw(hWnd);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

void Quit(HWND hWnd)
{
	srv->CloseEveryConnection();
	srv->Stop();
	delete srv;
	srv = NULL;
	DestroyWindow(hWnd);
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

DEFRMI_C2S_OnLogOn(C2SStub)
{
	TCHAR buf[LOG_BUF_SIZE];
	SUser sUser;
	sUser.SetUserData(user, remote);
	wsprintf(buf, TEXT("LogOn!! : hostID : %d, txt : %s"), sUser.m_hostID, user.m_name);
	SendMessage(hLogList, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	g_S2CProxy.SystemChat(sUser.m_hostID, RmiContext::ReliableSend, L"Login Success");
	int cnt = userList.Count;
	for (int i = 0; i < cnt; ++i)
	{
		wsprintf(buf, TEXT("%d님이 입장 했습니다."), sUser.m_hostID);
		g_S2CProxy.SystemChat(userList[i].m_hostID, RmiContext::ReliableSend, buf);
	}
	userList.Add(sUser);
	return true;
}

DEFRMI_C2S_Chat(C2SStub)
{
	TCHAR buf[LOG_BUF_SIZE];
	wsprintf(buf, TEXT("%d : %s"), remote, txt);
	SendMessage(hLogList, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	//g_S2CProxy.ShowChat(remote, RmiContext::ReliableSend, txt);
	int cnt = userList.Count;
	for (int i = 0; i < cnt; ++i)
	{
		g_S2CProxy.ShowChat(userList[i].m_hostID, RmiContext::ReliableSend, buf);
	}
	return true;
}

void SUser::SetUserData(const User &user, HostID hostID)
{
	m_name = user.m_name;
	m_hostID = hostID;
}