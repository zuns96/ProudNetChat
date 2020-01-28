#define _CRT_SECURE_NO_WARNINGS
#include "ServerDefine.h"
#include "Server.h"

using namespace std;
using namespace Proud;

CNetServer * srv = NULL;
HostID g_groupHostID = HostID_None;

class SUser : public User
{
public:
	Proud::HostID m_hostID;

	void SetUserData(const String &name, HostID hostID)
	{
		m_name = name;
		m_hostID = hostID;
	}
};

CFastArray<SUser> userList;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	srv = CNetServer::Create();

	srv->OnClientJoin = [](CNetClientInfo * clientInfo)
	{
		TCHAR buf[LOG_BUF_SIZE];
		wsprintf(buf, TEXT("Client %d connected."), clientInfo->m_HostID);
		SendMessage(hLogList, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
		int cnt = SendMessage(hLogList, LB_GETCOUNT, (WPARAM)NULL, (LPARAM)NULL);
		SendMessage(hLogList, LB_SETTOPINDEX, (WPARAM)(cnt - 1), (LPARAM)0);
	};

	srv->OnClientLeave = [](CNetClientInfo * clientInfo, ErrorInfo * errorInfo, const ByteArray& comment)
	{
		TCHAR buf[LOG_BUF_SIZE];
		int hostID = clientInfo->m_HostID;
		wsprintf(buf, TEXT("Client %d disconnected."), hostID);

		SendMessage(hLogList, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
		int cnt = SendMessage(hLogList, LB_GETCOUNT, (WPARAM)NULL, (LPARAM)NULL);
		SendMessage(hLogList, LB_SETTOPINDEX, (WPARAM)(cnt - 1), (LPARAM)0);

		int userCnt = userList.Count;
		for (int i = 0; i < userCnt; ++i)
		{
			if (userList[i].m_hostID == hostID)
			{
				SendMessage(hUserList, LB_DELETESTRING, (WPARAM)i, (LPARAM)0);
				wsprintf(buf, TEXT("%s님이 퇴장 하셨습니다."), userList[i].m_name);
				for (int j = 0; j < userCnt; ++j)
				{
					if (i != j)
					{
						Rpy_System_Chat rpy;
						rpy.msg = buf;
						g_S2CProxy.Recv_Rpy_System_Chat(userList[j].m_hostID, RmiContext::ReliableSend, rpy);
					}
				}
				userList.RemoveAt(i);
				break;
			}
		}
	};

	srv->OnError = [](ErrorInfo * errorInfo)
	{
		TCHAR buf[LOG_BUF_SIZE];
		wsprintf(buf, TEXT("OnError : %s", StringT2A(errorInfo->ToString()).GetString()));
		SendMessage(hLogList, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
		int cnt = SendMessage(hLogList, LB_GETCOUNT, (WPARAM)NULL, (LPARAM)NULL);
		SendMessage(hLogList, LB_SETTOPINDEX, (WPARAM)(cnt - 1), (LPARAM)0);
	};

	srv->OnWarning = [](ErrorInfo * errorInfo)
	{
		TCHAR buf[LOG_BUF_SIZE];
		wsprintf(buf, TEXT("OnWarning : %s", StringT2A(errorInfo->ToString()).GetString()));
		SendMessage(hLogList, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
		int cnt = SendMessage(hLogList, LB_GETCOUNT, (WPARAM)NULL, (LPARAM)NULL);
		SendMessage(hLogList, LB_SETTOPINDEX, (WPARAM)(cnt - 1), (LPARAM)0);
	};

	srv->OnInformation = [](ErrorInfo * errorInfo)
	{
		TCHAR buf[LOG_BUF_SIZE];
		wsprintf(buf, TEXT("OnInformation : %s", StringT2A(errorInfo->ToString()).GetString()));
		SendMessage(hLogList, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
		int cnt = SendMessage(hLogList, LB_GETCOUNT, (WPARAM)NULL, (LPARAM)NULL);
		SendMessage(hLogList, LB_SETTOPINDEX, (WPARAM)(cnt - 1), (LPARAM)0);
	};

	srv->OnException = [](const Exception &e)
	{
		TCHAR buf[LOG_BUF_SIZE];
		wsprintf(buf, TEXT("OnException : %s"), e.what());
		SendMessage(hLogList, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
		int cnt = SendMessage(hLogList, LB_GETCOUNT, (WPARAM)NULL, (LPARAM)NULL);
		SendMessage(hLogList, LB_SETTOPINDEX, (WPARAM)(cnt - 1), (LPARAM)0);
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
	WndClass.lpfnWndProc = ChatWndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);
	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, iChatClientWidth, iChatClientHeight, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, NULL, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	return (int)Message.wParam;
}

LRESULT CALLBACK ChatWndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_CREATE:
		CreatePushButton(TEXT("닫기"), 10, iChatClientHeight - 85, 100, 25, hWnd, WINDOW_ID_EXITBTN);
		CreatePushButton(TEXT("추방하기"), iChatClientWidth - 100 - 25, iChatClientHeight - 85, 100, 25, hWnd, WINDOW_ID_KICKBTN);
		hUserList = CreateListBox(10, 30, iChatClientWidth - 35, iChatClientHeight - 390, hWnd, WINDOW_ID_CHATLIST);
		hLogList = CreateListBox(10, 270, iChatClientWidth - 35, iChatClientHeight - 390, hWnd, WINDOW_ID_LOGLIST);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case WINDOW_ID_EXITBTN:
			Quit(hWnd);
			break;
		case WINDOW_ID_KICKBTN:
			if (curSelectUserIdx >=0 && curSelectUserIdx < userList.Count)
			{
				TCHAR buf[LOG_BUF_SIZE];
				SUser user = userList[curSelectUserIdx];
				wsprintf(buf, TEXT("[%d]%s님을 추방하시겠습니까?"), user.m_hostID, user.m_name);
				if (MessageBox(hWnd, buf, TEXT("추방"), MB_OKCANCEL) == IDOK)
				{
					curSelectUserIdx = -1;
					srv->CloseConnection(user.m_hostID);
				}
			}
			break;
		case WINDOW_ID_CHATLIST:
			switch (HIWORD(wParam))
			{
			case LBN_SELCHANGE:
				curSelectUserIdx = SendMessage(hUserList, LB_GETCURSEL, 0, 0);
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

DEFRMI_C2S_Send_Req_Login(C2SStub)
{
	TCHAR buf[LOG_BUF_SIZE];
	SUser sUser;
	sUser.SetUserData(req.id, remote);
	wsprintf(buf, TEXT("[LogOn!!] hostID : %d, id : %s"), sUser.m_hostID, sUser.m_name);
	SendMessage(hLogList, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
	int cnt = SendMessage(hLogList, LB_GETCOUNT, (WPARAM)NULL, (LPARAM)NULL);
	SendMessage(hLogList, LB_SETTOPINDEX, (WPARAM)(cnt - 1), (LPARAM)0);

	Rpy_Login rpy;
	rpy.id = req.id;

	g_S2CProxy.Recv_Rpy_Login(sUser.m_hostID, RmiContext::ReliableSend, rpy);
	int userCnt = userList.Count;
	for (int i = 0; i < userCnt; ++i)
	{
		wsprintf(buf, TEXT("%s님이 입장 하셨습니다."), sUser.m_name);
		
		Rpy_System_Chat rpy;
		rpy.msg = buf;

		g_S2CProxy.Recv_Rpy_System_Chat(userList[i].m_hostID, RmiContext::ReliableSend, rpy);
	}
	userList.Add(sUser);
	wsprintf(buf, TEXT("[%d] : %s"), sUser.m_hostID, sUser.m_name);
	SendMessage(hUserList, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
	return true;
}

DEFRMI_C2S_Send_Req_Chat(C2SStub)
{
	TCHAR buf[LOG_BUF_SIZE];
	wsprintf(buf, TEXT("[Chat]{%d}%s"), remote, req.msg);
	SendMessage(hLogList, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
	int cnt = SendMessage(hLogList, LB_GETCOUNT, (WPARAM)NULL, (LPARAM)NULL);
	SendMessage(hLogList, LB_SETTOPINDEX, (WPARAM)(cnt - 1), (LPARAM)0);

	int userCnt = userList.Count;
	for (int i = 0; i < userCnt; ++i)
	{
		Rpy_Chat rpy;
		rpy.hostID = remote;
		rpy.msg = req.msg;
		g_S2CProxy.Recv_Rpy_Chat(userList[i].m_hostID, RmiContext::ReliableSend, rpy);
	}
	return true;
}