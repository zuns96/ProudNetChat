#pragma once
#include <ProudNetServer.h>
#include <windows.h>
#include "RemoteClient_S.h"
#include "../Common/Vars.h"
#include "../Common/C2S_common.cpp"
#include "../Common/S2C_common.cpp"
#include "../Common/C2S_stub.h"
#include "../Common/C2S_stub.cpp"
#include "../Common/S2C_proxy.h"
#include "../Common/S2C_proxy.cpp"

class C2SStub : public INetServerEvent, public C2S::Stub
{
public:
	DECRMI_C2S_RequestLogon;
	DECRMI_C2S_Chat;
	DECRMI_C2S_RequestP2PGroup;
	DECRMI_C2S_RequestLeaveP2PGroup;

	virtual void OnClientJoin(CNetClientInfo *clientInfo)PN_OVERRIDE;
	virtual void OnClientLeave(CNetClientInfo *clientInfo, ErrorInfo *errorInfo, const ByteArray& comment)PN_OVERRIDE;

	virtual bool OnConnectionRequest(AddrPort clientAddr, ByteArray &userDataFromClient, ByteArray &reply) PN_OVERRIDE {
		return true;
	}
	virtual void OnP2PGroupJoinMemberAckComplete(HostID groupHostID, HostID memberHostID, ErrorType result) PN_OVERRIDE {}
	virtual void OnUserWorkerThreadBegin() PN_OVERRIDE {}
	virtual void OnUserWorkerThreadEnd() PN_OVERRIDE {}
	virtual void OnError(ErrorInfo *errorInfo) PN_OVERRIDE;
	virtual void OnWarning(ErrorInfo *errorInfo)PN_OVERRIDE;
	virtual void OnInformation(ErrorInfo *errorInfo)PN_OVERRIDE;
	virtual void OnException(const Exception &e) PN_OVERRIDE;
	virtual void OnUnhandledException() {}
	virtual void OnNoRmiProcessed(RmiID rmiID) PN_OVERRIDE {}
};

enum WINDOW_ID : int
{
	EXIT_BUTTON = 0,
	KICK_BUTTON = 1,
	USER_LIST = 2,
	TEXT_EDIT = 3,
};

#define WINDOW_ID_EXITBTN WINDOW_ID::EXIT_BUTTON
#define WINDOW_ID_KICKBTN WINDOW_ID::KICK_BUTTON
#define WINDOW_ID_USERLIST WINDOW_ID::USER_LIST
#define WINDOW_ID_TEXTEDIT WINDOW_ID::TEXT_EDIT