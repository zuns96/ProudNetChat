#include <iostream>
#include <ProudNetServer.h>

#include "../Common/Vars.h"
#include "../Common/C2S_common.cpp"
#include "../Common/S2C_common.cpp"
#include "../Common/C2S_stub.h"
#include "../Common/S2C_proxy.h"
#include "../Common/C2S_stub.cpp"
#include "../Common/S2C_proxy.cpp"

using namespace std;
using namespace Proud;

class C2SStub : public C2S::Stub
{
public:
	DECRMI_C2S_Chat;
};

C2SStub g_C2SStub;
S2C::Proxy g_S2CProxy;
HostID g_groupHostID = HostID_None;

DEFRMI_C2S_Chat(C2SStub)
{
	printf("[Server] chat message received : a=%s, b=%d, c=%f, d.a=%d, d.b=%.f, d.c=%lf", StringT2A(a).GetString(), b, c, d.a, d.b, d.c);
	int i = 0;
	int cnt = f.GetCount();
	for (i = 0; i < cnt; ++i)
		printf(", f[%d]=%d", i, f[i]);

	for (CFastMap<int, float>::iterator it = g.begin(); it != g.end(); ++it)
		printf(", pair(%d, %f)", it->GetFirst(), it->GetSecond());

	printf("\n");

	if (block.GetCount() != 100)
		printf("Error : ByteArray length is not equal \n");

	cnt = block.GetCount();
	for (i = 0; i < cnt; ++i)
	{
		if (i != (int)block[i])
			printf("ByteArray data is not equal. index:%d, data:%d\n", i, (int)block[i]);
	}

	// Echo chatting message which received from server to client.
	g_S2CProxy.ShowChat(remote, RmiContext::ReliableSend, a, b + 1, c + 1);
	return true;
}

int main()
{
#pragma region NetServer 클래스 선언하기
	shared_ptr<CNetServer> srv(CNetServer::Create());
#pragma endregion NetServer 클래스 선언하기

#pragma region NetServerEvent를 받기위한 람다식 정의
	srv->OnClientJoin = [](CNetClientInfo * clientInfo)
	{
		printf("Client %d connected.\n", clientInfo->m_HostID);
	};

	srv->OnClientLeave = [](CNetClientInfo * clientInfo, ErrorInfo * errorInfo, const ByteArray& comment)
	{
		printf("Client %d disconnected.\n", clientInfo->m_HostID);
	};

	srv->OnError = [](ErrorInfo * errorInfo)
	{
		printf("OnError : %s\n", StringT2A(errorInfo->ToString()).GetString());
	};

	srv->OnWarning = [](ErrorInfo * errorInfo)
	{
		printf("OnWarning : %s\n", StringT2A(errorInfo->ToString()).GetString());
	};

	srv->OnInformation = [](ErrorInfo * errorInfo)
	{
		printf("OnInformation : %s\n", StringT2A(errorInfo->ToString()).GetString());
	};

	srv->OnException = [](const Exception &e)
	{
		printf("OnException : %s\n", e.what());
	};
#pragma endregion NetServerEvent를 받기위한 람다식 정의

#pragma region RmiStub, RmiProxy 클래스 NetServer에 Attach하기
	srv->AttachStub(&g_C2SStub);
	srv->AttachProxy(&g_S2CProxy);
#pragma endregion RmiStub, RmiProxy 클래스 NetServer에 Attach하기

#pragma region 서버 시작에 필요한 Parameter 설정하기
	CStartServerParameter p1;
	p1.m_protocolVersion = g_Version;
	p1.m_tcpPorts.Add(g_ServerPort);
#pragma endregion 서버 시작에 필요한 Parameter 설정하기

#pragma region 서버를 시작하기
	try
	{
		srv->Start(p1);
	}
	catch (Exception &e)
	{
		cout << "Server start failed: " << e.what() << endl;
		return 0;
	}
#pragma endregion 서버를 시작하기

	printf("Server started. Enterable commands:\n");
	printf("1: Creates a P2P group where all clients join.\n");
	printf("2. Sends a message to P2P group members.\n");
	printf("q: Quit.\n");

	string userInput;

	while (1)
	{
		cin >> userInput;

		if (userInput == "1")
		{
			HostID list[100];
			int listCount = srv->GetClientHostIDs(list, 100);
			g_groupHostID = srv->CreateP2PGroup(list, listCount, ByteArray());
		}
		else if (userInput == "2")
		{
			g_S2CProxy.SystemChat(g_groupHostID, RmiContext::ReliableSend, _PNT("Hello~~~!"));
		}
		else if (userInput == "3")
		{
			srv->DestroyP2PGroup(g_groupHostID);
		}
		else if (userInput == "q")
		{
			break;
		}

		Sleep(10);
	}

	return 0;
}