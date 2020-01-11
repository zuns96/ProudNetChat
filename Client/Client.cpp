#include "Client.h"

using namespace std;
using namespace Proud;

CriticalSection g_critSec;

HWND hUserList;
HWND hLogList;

C2S::Proxy g_C2SProxy;

class S2CStub : public S2C::Stub
{
public:
	DECRMI_S2C_ShowChat;
	DECRMI_S2C_SystemChat;
};

DEFRMI_S2C_ShowChat(S2CStub)
{
	TCHAR buf[512];
	wsprintf(buf, TEXT("%s"), StringT2W(txt).GetString());
	SendMessage(hUserList, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	CriticalSectionLock lock(g_critSec, true);
	return true;
}

DEFRMI_S2C_SystemChat(S2CStub)
{
	TCHAR buf[512];
	wsprintf(buf, TEXT("[Client] SystemChat: text=%s"), StringT2W(txt).GetString());
	SendMessage(hLogList, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	CriticalSectionLock lock(g_critSec, true);
	return true;
}

S2CStub g_S2CStub;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void CreatePushButton(LPCTSTR text, int x, int y, int width, int height, HWND hWnd, int id);
HWND CreateListBox(int x, int y, int width, int height, HWND hWnd, int id);
void Draw(HWND hWnd);

HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("chat_client");

int iWidth = 400;
int iHeight = 600;

bool isConnected = false;
volatile bool keepWorkerThread = true;
HostID recentP2PGroupHostID = HostID_None;
CNetClient * netClient;

Proud::Thread workerThread([&]
{
	while (keepWorkerThread)
	{
		Proud::Sleep(10);
		netClient->FrameMove();
	}
});

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	netClient = CNetClient::Create();

	netClient->OnJoinServerComplete = [&](ErrorInfo * info, const ByteArray &replayFromServer)
	{
		CriticalSectionLock lock(g_critSec, true);

		if (info->m_errorType == ErrorType_Ok)
		{
			printf("Succeed to connect server. Allocated hostID=%d\n", netClient->GetLocalHostID());
			isConnected = true;

			User user;
			user.m_name = _PNT("nickName");
			g_C2SProxy.OnLogOn(HostID_Server, RmiContext::ReliableSend, user);
		}
		else
		{
			cout << "Failed to connect to server.\n";
		}
	};

	netClient->OnLeaveServer = [&](ErrorInfo * errorInfo)
	{
		CriticalSectionLock lock(g_critSec, true);

		printf("OnLeadvServer. %s \n", StringT2A(errorInfo->m_comment).GetString());
		isConnected = false;
		keepWorkerThread = false;
	};

	netClient->OnP2PMemberJoin = [&](HostID memberHostID, HostID groupHostID, int memberCount, const ByteArray &customField)
	{
		CriticalSectionLock lock(g_critSec, true);

		printf("[Client] P2P member %d joined group %d.\n", memberHostID, groupHostID);

		if (memberHostID != netClient->GetLocalHostID())
		{
			memberHostID = memberHostID;
		}
	};

	netClient->OnP2PMemberLeave = [](HostID memberHostID, HostID groupHostID, int memberCount)
	{
		printf("[Client] P2P Member %d left group %d.\n,", memberHostID, groupHostID);
	};
	netClient->OnChangeServerUdpState = [](ErrorType reason)
	{
		printf("OnChangeServerUdpState : %d\n", (int)reason);
	};
	netClient->OnChangeP2PRelayState = [](HostID remoteHostID, ErrorType reason)
	{
		printf("OnChangeP2PRelayState : %d\n", (int)reason);
	};
	netClient->OnError = [](ErrorInfo * errorInfo)
	{
		printf("OnError : %s\n", StringT2A(errorInfo->ToString()).GetString());
	};
	netClient->OnWarning = [](ErrorInfo * errorInfo)
	{
		printf("OnWarning : %s\n", StringT2A(errorInfo->ToString()).GetString());
	};
	netClient->OnInformation = [](ErrorInfo * errorInfo)
	{
		printf("OnInformation : %s\n", StringT2A(errorInfo->ToString()).GetString());
	};
	netClient->OnException = [](const Exception &e)
	{
		printf("OnException : %s\n", (const char*)e.what());
	};

	netClient->AttachProxy(&g_C2SProxy);
	netClient->AttachStub(&g_S2CStub);

	CNetConnectionParam cp;
	cp.m_protocolVersion = g_Version;
	cp.m_serverIP = _PNT("localhost");
	cp.m_serverPort = g_ServerPort;

	netClient->Connect(cp);
	workerThread.Start();

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

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, hInstance, NULL);
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
		CreatePushButton(TEXT("보내기"), iWidth - 100 - 25, iHeight - 85, 100, 25, hWnd, WINDOW_ID_KICKBTN);
		hUserList = CreateListBox(10, 30, iWidth - 35, iHeight - 390, hWnd, WINDOW_ID_USERLIST);
		hLogList = CreateListBox(10, 270, iWidth - 35, iHeight - 390, hWnd, WINDOW_ID_LOGLIST);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case WINDOW_ID_EXITBTN:
			CloseWindow(hWnd);
			break;
		case WINDOW_ID_KICKBTN:
			g_C2SProxy.Chat(HostID_Server, RmiContext::ReliableSend, _PNT("안녕하세요."));
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
		return 0;
	case WM_CLOSE:
		keepWorkerThread = false;
		workerThread.Join();
		netClient->Disconnect();
		delete netClient;
		netClient = NULL;
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

	TextOut(hdc, 10, 10, TEXT("채팅 로그"), 5);
	TextOut(hdc, 10, 250, TEXT("시스템 메시지"), 7);

	EndPaint(hWnd, &ps);
}