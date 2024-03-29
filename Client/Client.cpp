#define _CRT_SECURE_NO_WARNINGS
#include "Client.h"

using namespace std;
using namespace Proud;

CriticalSection g_critSec;
CNetClient * netClient;

DEFRMI_S2C_Recv_Rpy_Login(S2CStub)
{
	CriticalSectionLock lock(g_critSec, true);
	isLogin = true;
	user.m_name = rpy.id;
	return true;
}

DEFRMI_S2C_Recv_Rpy_Chat(S2CStub)
{
	CriticalSectionLock lock(g_critSec, true);

	TCHAR buf[512];
	wsprintf(buf, TEXT("%s"), StringT2W(rpy.msg).GetString());
	
	if (netClient->GetLocalHostID() == rpy.hostID)
		SetWindowText(hTextEdit, TEXT(""));

	SendMessage(hUserList, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
	int cnt = SendMessage(hUserList, LB_GETCOUNT, (WPARAM)NULL, (LPARAM)NULL);
	SendMessage(hUserList, LB_SETTOPINDEX, (WPARAM)(cnt - 1), (LPARAM)0);

	return true;
}

DEFRMI_S2C_Recv_Rpy_System_Chat(S2CStub)
{
	CriticalSectionLock lock(g_critSec, true);
	SendMessage(hLogList, LB_ADDSTRING, (WPARAM)0, (LPARAM)StringT2W(rpy.msg).GetString());
	int cnt = SendMessage(hLogList, LB_GETCOUNT, (WPARAM)NULL, (LPARAM)NULL);
	SendMessage(hLogList, LB_SETTOPINDEX, (WPARAM)(cnt - 1), (LPARAM)0);

	return true;
}

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
	HWND hWnd;
	MSG Message;
	netClient = CNetClient::Create();

	netClient->OnJoinServerComplete = [&](ErrorInfo * info, const ByteArray &replayFromServer)
	{
		CriticalSectionLock lock(g_critSec, true);
		ShowWindow(hWnd, nCmdShow);

		if (info->m_errorType == ErrorType_Ok)
		{
			isConnected = true;
			TCHAR info[256];
			wsprintf(info, TEXT("Login(%s:%d)"), StringA2W(c_ip), g_ServerPort);
			SetWindowText(hWnd, info);
		}
		else
		{
			MessageBox(hWnd, TEXT("서버 연결에 실패 했습니다."), TEXT("Error"), MB_OK);
		}
	};

	netClient->OnLeaveServer = [&](ErrorInfo * errorInfo)
	{
		CriticalSectionLock lock(g_critSec, true);

		isConnected = false;
		keepWorkerThread = false;

		SendMessage(hLogList, LB_ADDSTRING, (WPARAM)0, (LPARAM)TEXT("서버 연결이 끊어졌습니다."));
		int cnt = SendMessage(hLogList, LB_GETCOUNT, (WPARAM)NULL, (LPARAM)NULL);
		SendMessage(hLogList, LB_SETTOPINDEX, (WPARAM)(cnt-1), (LPARAM)0);
	};

	netClient->OnError = [](ErrorInfo * errorInfo)
	{
		TCHAR buf[512];
		wsprintf(buf, TEXT("OnError : %s"), StringT2W(errorInfo->ToString()).GetString());

		SendMessage(hLogList, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
		int cnt = SendMessage(hLogList, LB_GETCOUNT, (WPARAM)NULL, (LPARAM)NULL);
		SendMessage(hLogList, LB_SETTOPINDEX, (WPARAM)(cnt - 1), (LPARAM)0);
	};
	netClient->OnWarning = [](ErrorInfo * errorInfo)
	{
		TCHAR buf[512];
		wsprintf(buf, TEXT("OnWarning : %s"), StringT2W(errorInfo->ToString()).GetString());

		SendMessage(hLogList, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
		int cnt = SendMessage(hLogList, LB_GETCOUNT, (WPARAM)NULL, (LPARAM)NULL);
		SendMessage(hLogList, LB_SETTOPINDEX, (WPARAM)(cnt - 1), (LPARAM)0);
	};
	netClient->OnInformation = [](ErrorInfo * errorInfo)
	{
		TCHAR buf[512];
		wsprintf(buf, TEXT("OnInformation : %s"), StringT2W(errorInfo->ToString()).GetString());

		SendMessage(hLogList, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
		int cnt = SendMessage(hLogList, LB_GETCOUNT, (WPARAM)NULL, (LPARAM)NULL);
		SendMessage(hLogList, LB_SETTOPINDEX, (WPARAM)(cnt - 1), (LPARAM)0);
	};
	netClient->OnException = [](const Exception &e)
	{
		TCHAR buf[512];
		wsprintf(buf, TEXT("OnException : %s"), StringT2W(e.what()).GetString());

		SendMessage(hLogList, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
		int cnt = SendMessage(hLogList, LB_GETCOUNT, (WPARAM)NULL, (LPARAM)NULL);
		SendMessage(hLogList, LB_SETTOPINDEX, (WPARAM)(cnt - 1), (LPARAM)0);
	};

	netClient->AttachProxy(&g_C2SProxy);
	netClient->AttachStub(&g_S2CStub);

	CNetConnectionParam cp;
	cp.m_protocolVersion = g_Version;
	cp.m_serverIP = StringA2T(c_ip);
	cp.m_serverPort = g_ServerPort;

	netClient->Connect(cp);

	g_hInst = hInstance;

	WndLoginClass.cbClsExtra = 0;
	WndLoginClass.cbWndExtra = 0;
	WndLoginClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndLoginClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndLoginClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndLoginClass.hInstance = hInstance;
	WndLoginClass.lpfnWndProc = LoginWndProc;
	WndLoginClass.lpszClassName = lpszLoginClass;
	WndLoginClass.lpszMenuName = NULL;
	WndLoginClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndLoginClass);
	
	hWnd = CreateWindow(lpszLoginClass, lpszLoginClass, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, iLoginWidth, iLoginHeight, NULL, (HMENU)NULL, hInstance, NULL);
	workerThread.Start();

	while (GetMessage(&Message, NULL, 0, 0) && !isLogin)
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	UnregisterClass(lpszLoginClass, g_hInst);
	DestroyWindow(hWnd);

	if (isLogin)
	{
		WndChatClass.cbClsExtra = 0;
		WndChatClass.cbWndExtra = 0;
		WndChatClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		WndChatClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		WndChatClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		WndChatClass.hInstance = hInstance;
		WndChatClass.lpfnWndProc = ChatWndProc;
		WndChatClass.lpszClassName = lpszClass;
		WndChatClass.lpszMenuName = NULL;
		WndChatClass.style = CS_HREDRAW | CS_VREDRAW;
		RegisterClass(&WndChatClass);

		TCHAR info[256];
		wsprintf(info, TEXT("Chat(server : %s:%d, hostID : %d, id : %s)"), StringA2W(c_ip), g_ServerPort, netClient->GetLocalHostID(), user.m_name);
		hWnd = CreateWindow(lpszClass, info, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, iChatClientWidth, iChatClientHeight, NULL, (HMENU)NULL, hInstance, NULL);
		ShowWindow(hWnd, nCmdShow);

		wsprintf(info, TEXT("<SystemChat> %s"), TEXT("로그인에 성공하셨습니다."));
		SendMessage(hLogList, LB_ADDSTRING, (WPARAM)0, (LPARAM)info);

		while (GetMessage(&Message, NULL, 0, 0))
		{
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
	}

	return (int)Message.wParam;
}

LRESULT CALLBACK ChatWndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_CREATE:
		hUserList = CreateListBox(10, 30, iChatClientWidth - 35, iChatClientHeight - 390, hWnd, WINDOW_ID_CHATLIST);
		hLogList = CreateListBox(10, 270, iChatClientWidth - 35, iChatClientHeight - 390, hWnd, WINDOW_ID_LOGLIST);
		CreatePushButton(TEXT("닫기"), 10, iChatClientHeight - 85, 100, 25, hWnd, WINDOW_ID_EXITBTN, false);
		CreatePushButton(TEXT("보내기"), iChatClientWidth - 125, iChatClientHeight - 110, 100, 25, hWnd, WINDOW_ID_SENDBTN, true);
		hTextEdit = CreateTextEdit(10, iChatClientHeight - 110, 365, 20, hWnd, WINDOW_ID_TEXTEDIT);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case WINDOW_ID_EXITBTN:
			Quit(hWnd);
			break;
		case WINDOW_ID_SENDBTN:
			TCHAR input[256];
			GetWindowText(hTextEdit, input, 256);
			if (lstrlenW(input) > 0)
			{
				TCHAR text[512];
				wsprintf(text, TEXT("%s : %s"), user.m_name, input);
				
				Req_Chat req;
				req.msg = StringW2T(text).GetString();

				g_C2SProxy.Send_Req_Chat(HostID_Server, RmiContext::ReliableSend, req);
			}
			break;
		case WINDOW_ID_TEXTEDIT:
			switch (HIWORD(wParam))
			{
			case EN_UPDATE:
				break;
			case EN_CHANGE:
				TCHAR input[256];
				GetWindowText(hTextEdit, input, 256);
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

LRESULT CALLBACK LoginWndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_CREATE:
		CreatePushButton(TEXT("닫기"), 10, iLoginHeight - 85, 100, 25, hWnd, WINDOW_ID_EXITBTN, false);
		CreatePushButton(TEXT("로그인"), iLoginWidth - 125, iLoginHeight - 85, 100, 25, hWnd, WINDOW_ID_LOGINBTN, true);
		hIDEdit = CreateTextEdit(10, 30, 200, 20, hWnd, WINDOW_ID_TEXTEDIT);
		hPWEdit = CreateTextEdit(10, 70, 200, 20, hWnd, WINDOW_ID_TEXTEDIT);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case WINDOW_ID_EXITBTN:
			Quit(hWnd);
			break;
		case WINDOW_ID_LOGINBTN:
			TCHAR id[10];
			TCHAR pw[10];
			GetWindowText(hIDEdit, id, 10);
			GetWindowText(hPWEdit, pw, 10);
			if (lstrlen(id) == 0)
			{
				MessageBox(hWnd, TEXT("ID를 입력해주세요."), TEXT("Error"), MB_OK);
			}
			else if (lstrlen(pw) == 0)
			{
				MessageBox(hWnd, TEXT("비밀번호를 입력해주세요."), TEXT("Error"), MB_OK);
			}
			else
			{
				Req_Login req;
				req.id = StringW2T(id).GetString();
				req.password = StringW2T(pw).GetString();

				g_C2SProxy.Send_Req_Login(HostID_Server, RmiContext::ReliableSend, req);
			}
			break;
		default:
			break;
		}
		return 0;
	case WM_DESTROY:
		if(!isLogin)
			PostQuitMessage(0);
		return 0;
	case WM_CLOSE:
		if (isLogin)
		{
			DestroyWindow(hWnd);
		}
		else
		{
			Quit(hWnd);
		}
		return 0;
	case WM_PAINT:
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		TextOut(hdc, 10, 10, TEXT("ID"), 2);
		TextOut(hdc, 10, 50, TEXT("Password"), 8);

		EndPaint(hWnd, &ps);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

void CreatePushButton(LPCTSTR text, int x, int y, int width, int height, HWND hWnd, int id, bool isDefaultPush)
{
	int style = WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON;
	if (isDefaultPush) style |= BS_DEFPUSHBUTTON;
	CreateWindow(TEXT("button"), text, style, x, y, width, height, hWnd, (HMENU)id, g_hInst, NULL);
}

HWND CreateListBox(int x, int y, int width, int height, HWND hWnd, int id)
{
	return CreateWindow(TEXT("listbox"), NULL, WS_CHILD  | WS_VISIBLE | WS_BORDER | WS_HSCROLL | WS_VSCROLL | LBS_NOTIFY, x, y, width, height, hWnd, (HMENU)id, g_hInst, NULL);
}

HWND CreateTextEdit(int x, int y, int width, int height, HWND hWnd, int id)
{
	return CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_WANTRETURN, x, y, width, height, hWnd, (HMENU)id, g_hInst, NULL);
}

void Draw(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);

	TextOut(hdc, 10, 10, TEXT("채팅 로그"), 5);
	TextOut(hdc, 10, 250, TEXT("시스템 메시지"), 7);

	EndPaint(hWnd, &ps);
}

void Quit(HWND hWnd)
{
	keepWorkerThread = false;
	workerThread.Join();
	
	netClient->Disconnect();
	delete netClient;
	netClient = NULL;

	DestroyWindow(hWnd);
}