#include <iostream>
#include<ProudNetClient.h>

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

class C2CStub : public C2C::Stub
{
public :
	DECRMI_C2C_P2PChat;
};

CriticalSection g_critSec;

DEFRMI_C2C_P2PChat(C2CStub)
{
	printf("[Client] P2PChat : relayed:%s, a=%s, b=%d, c=%f\n", rmiContext.m_relayed ? "true" : "false", a.GetString(), b, c);
	return true;
}

C2C::Proxy g_C2CProxy;
C2CStub g_C2CStub;

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
	printf("[Client] ShowChat: a=%s, b=%d, c=%f\n", StringT2A(a).GetString(), b, c);
	return true;
}

DEFRMI_S2C_SystemChat(S2CStub)
{
	CriticalSectionLock lock(g_critSec, true);
	printf("[Client] ShowChat: a=%s, b=%d, c=%f\n", StringT2A(txt).GetString());
	return true;
}

S2CStub g_S2CStub;

int main()
{
	bool isConnected = false;
	volatile bool keepWorkerThread = true;
	HostID recentP2PGroupHostID = HostID_None;
	shared_ptr<CNetClient> netClient(CNetClient::Create());

	netClient->OnJoinServerComplete = [&](ErrorInfo * info, const ByteArray &replayFromServer)
	{
		CriticalSectionLock lock(g_critSec, true);

		if (info->m_errorType == ErrorType_Ok)
		{
			printf("Succeed to connect server. Allocated hostID=%d\n", netClient->GetLocalHostID());
			isConnected = true;
			MyClass myclass;
			myclass.a = 1;
			myclass.b = 1.3f;
			myclass.c = 1.4567;

			CFastArray<int> arr;
			arr.Add(4);
			arr.Add(5);

			CFastMap<int, float> map;
			map.Add(1, 5.1f);
			map.Add(2, 4.1f);

			ByteArray block;
			for (int i = 0; i < 100; ++i)
				block.Add((uint8_t)i);

			g_C2SProxy.Chat(HostID_Server,
				RmiContext::ReliableSend,
				_PNT("Hello ProudNet~!!!."), 333, 22.33f, myclass, arr, map, block);
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
			g_C2CProxy.P2PChat(memberHostID, RmiContext::ReliableSend, _PNT("Welcome!!"), 1, 1.0f);
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
	netClient->AttachProxy(&g_C2CProxy);
	netClient->AttachStub(&g_C2CStub);

	CNetConnectionParam cp;
	cp.m_protocolVersion = g_Version;
	cp.m_serverIP = _PNT("localhost");
	cp.m_serverPort = g_ServerPort;

	netClient->Connect(cp);

	Proud::Thread workerThread([&]
	{
		while (keepWorkerThread)
		{
			Proud::Sleep(10);
			netClient->FrameMove();
		}
	});
	workerThread.Start();

	cout << "a: Send a P2P message to current P2P group members except for self.\n";
	cout << "q: Exit.\n";
	while (keepWorkerThread)
	{
		string userInput;
		cin >> userInput;

		if (userInput == "q")
		{
			keepWorkerThread = false;
		}
		else if (userInput == "a")
		{
			if (isConnected)
			{
				CriticalSectionLock lock(g_critSec, true);

				RmiContext sendHow = RmiContext::ReliableSend;
				sendHow.m_enableLoopback = false;
				
				g_C2CProxy.P2PChat(recentP2PGroupHostID, sendHow, _PNT("Welcome ProudNet!!"), 1, 1);
			}
			else
			{
				cout << "Not yet connected.\n";
			}
		}
	}
	workerThread.Join();

	netClient->Disconnect();
	return 0;
}