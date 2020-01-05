#include "Server.h"

using namespace std;
using namespace Proud;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void CreatePushButton(LPCTSTR text, int x, int y, int width, int height, HWND hWnd, int id);
HWND CreateListBox(int x, int y, int width, int height, HWND hWnd, int id);
HWND CreateTextEdit(int x, int y, int width, int height, HWND hWnd, int id);
void Draw(HWND hWnd);

HINSTANCE g_hInst;
HWND hList;
HWND hEdit;

LPCTSTR lpszClass = TEXT("chat_server");

int iWidth = 400;
int iHeight = 600;

C2SStub g_C2SStub;
S2C::Proxy g_S2CProxy;
HostID g_groupHostID = HostID_None;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	shared_ptr<CNetServer> srv(CNetServer::Create());

	srv->SetEventSink(&g_C2SStub);
	srv->AttachStub(&g_C2SStub);
	srv->AttachProxy(&g_S2CProxy);

	CStartServerParameter p1;
	p1.m_protocolVersion = gProtocolVersion;
	p1.m_tcpPorts.Add(gServerPort);
	try
	{
		srv->Start(p1);
	}
	catch (Exception &e)
	{
		cout << "Server start failed: " << e.what() << endl;
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
		hList = CreateListBox(10, 30, iWidth - 35, iHeight - 390, hWnd, WINDOW_ID_USERLIST);
		hEdit = CreateTextEdit(10, 270, iWidth - 35, iHeight - 390, hWnd, WINDOW_ID_USERLIST);
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
				int i = SendMessage(hList, LB_GETCURSEL, 0, 0);
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
	return CreateWindow(TEXT("listbox"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | LBS_NOTIFY, x, y, width, height, hWnd, (HMENU)id, g_hInst, NULL);
}

HWND CreateTextEdit(int x, int y, int width, int height, HWND hWnd, int id)
{
	return CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY, x, y, width, height, hWnd, (HMENU)id, g_hInst, NULL);
}

void Draw(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);

	TextOut(hdc, 10, 10, TEXT("접속자 목록"), 6);
	TextOut(hdc, 10, 250, TEXT("메시지창"), 4);

	EndPaint(hWnd, &ps);
}

DEFRMI_C2S_RequestLogon(C2SStub)
{
	return true;
}

DEFRMI_C2S_Chat(C2SStub)
{
	return true;
}

DEFRMI_C2S_RequestP2PGroup(C2SStub)
{
	return true;
}

DEFRMI_C2S_RequestLeaveP2PGroup(C2SStub)
{
	return true;
}

void C2SStub::OnClientJoin(CNetClientInfo *clientInfo)
{

}
void C2SStub::OnClientLeave(CNetClientInfo *clientInfo, ErrorInfo *errorInfo, const ByteArray& comment)
{

}
void C2SStub::OnError(ErrorInfo *errorInfo)
{

}
void C2SStub::OnWarning(ErrorInfo *errorInfo)
{

}
void C2SStub::OnInformation(ErrorInfo *errorInfo)
{

}
void C2SStub::OnException(const Exception &e)
{

}