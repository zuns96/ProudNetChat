#include<ProudNetClient.h>
#include<Windows.h>
#include <iostream>

#include "../Common/Vars.h"
#include "../Common/C2S_common.cpp"
#include "../Common/C2S_proxy.h"
#include "../Common/C2S_proxy.cpp"

#include "../Common/S2C_common.cpp"
#include "../Common/S2C_stub.h"
#include "../Common/S2C_stub.cpp"

#include "../Common/C2C_common.cpp"
#include "../Common/C2C_stub.h"
#include "../Common/C2C_stub.cpp"
#include "../Common/C2C_proxy.h"
#include "../Common/C2C_proxy.cpp"

using namespace std;
using namespace Proud;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("chat_client");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
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
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}


#pragma region Legacy
CriticalSection g_critSec;

C2S::Proxy g_C2SProxy;

class S2CStub : public S2C::Stub
{
public :
	DECRMI_S2C_ShowChat;
	DECRMI_S2C_SystemChat;
};

DEFRMI_S2C_ShowChat(S2CStub)
{
	CriticalSectionLock lock(g_critSec, true);
	printf("[Client] ShowChat: text=%s\n", StringT2A(txt).GetString());
	return true;
}

DEFRMI_S2C_SystemChat(S2CStub)
{
	CriticalSectionLock lock(g_critSec, true);
	printf("[Client] SystemChat: text=%s\n", StringT2A(txt).GetString());
	return true;
}

S2CStub g_S2CStub;

//int main()
//{
//	bool isConnected = false;
//	volatile bool keepWorkerThread = true;
//	HostID recentP2PGroupHostID = HostID_None;
//	shared_ptr<CNetClient> netClient(CNetClient::Create());
//
//	netClient->OnJoinServerComplete = [&](ErrorInfo * info, const ByteArray &replayFromServer)
//	{
//		CriticalSectionLock lock(g_critSec, true);
//
//		if (info->m_errorType == ErrorType_Ok)
//		{
//			printf("Succeed to connect server. Allocated hostID=%d\n", netClient->GetLocalHostID());
//			isConnected = true;
//
//			g_C2SProxy.Chat(HostID_Server,
//				RmiContext::ReliableSend,
//				_PNT("Hello ProudNet~!!!."));
//		}
//		else
//		{
//			cout << "Failed to connect to server.\n";
//		}
//	};
//
//	netClient->OnLeaveServer = [&](ErrorInfo * errorInfo)
//	{
//		CriticalSectionLock lock(g_critSec, true);
//
//		printf("OnLeadvServer. %s \n", StringT2A(errorInfo->m_comment).GetString());
//		isConnected = false;
//		keepWorkerThread = false;
//	};
//
//	netClient->OnP2PMemberJoin = [&](HostID memberHostID, HostID groupHostID, int memberCount, const ByteArray &customField)
//	{
//		CriticalSectionLock lock(g_critSec, true);
//
//		printf("[Client] P2P member %d joined group %d.\n", memberHostID, groupHostID);
//
//		if (memberHostID != netClient->GetLocalHostID())
//		{
//			memberHostID = memberHostID;
//		}
//	};
//
//	netClient->OnP2PMemberLeave = [](HostID memberHostID, HostID groupHostID, int memberCount)
//	{
//		printf("[Client] P2P Member %d left group %d.\n,", memberHostID, groupHostID);
//	};
//	netClient->OnChangeServerUdpState = [](ErrorType reason)
//	{
//		printf("OnChangeServerUdpState : %d\n", (int)reason);
//	};
//	netClient->OnChangeP2PRelayState = [](HostID remoteHostID, ErrorType reason)
//	{
//		printf("OnChangeP2PRelayState : %d\n", (int)reason);
//	};
//	netClient->OnError = [](ErrorInfo * errorInfo)
//	{
//		printf("OnError : %s\n", StringT2A(errorInfo->ToString()).GetString());
//	};
//	netClient->OnWarning = [](ErrorInfo * errorInfo)
//	{
//		printf("OnWarning : %s\n", StringT2A(errorInfo->ToString()).GetString());
//	};
//	netClient->OnInformation = [](ErrorInfo * errorInfo)
//	{
//		printf("OnInformation : %s\n", StringT2A(errorInfo->ToString()).GetString());
//	};
//	netClient->OnException = [](const Exception &e)
//	{
//		printf("OnException : %s\n", (const char*)e.what());
//	};
//
//	netClient->AttachProxy(&g_C2SProxy);
//	netClient->AttachStub(&g_S2CStub);
//
//	CNetConnectionParam cp;
//	cp.m_protocolVersion = g_Version;
//	cp.m_serverIP = _PNT("localhost");
//	cp.m_serverPort = g_ServerPort;
//
//	netClient->Connect(cp);
//
//	Proud::Thread workerThread([&]
//	{
//		while (keepWorkerThread)
//		{
//			Proud::Sleep(10);
//			netClient->FrameMove();
//		}
//	});
//	workerThread.Start();
//
//	cout << "a: Send a P2P message to current P2P group members except for self.\n";
//	cout << "q: Exit.\n";
//	while (keepWorkerThread)
//	{
//		string userInput;
//		cin >> userInput;
//
//		if (userInput == "q")
//		{
//			keepWorkerThread = false;
//		}
//		else if (userInput == "a")
//		{
//			if (isConnected)
//			{
//				CriticalSectionLock lock(g_critSec, true);
//				RmiContext rmi = RmiContext::ReliableSend;
//				rmi.m_enableLoopback = false;
//				g_C2SProxy.Chat(HostID_Server, rmi, _PNT("hello"));
//			}
//			else
//			{
//				cout << "Not yet connected.\n";
//			}
//		}
//	}
//	workerThread.Join();
//
//	netClient->Disconnect();
//	return 0;
//}
#pragma endregion Legacy