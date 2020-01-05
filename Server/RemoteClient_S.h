#pragma once
using namespace Proud;

class CRemoteClient_S
{
public:
	HostID m_hostID;
	String m_userName;

	CRemoteClient_S();
	~CRemoteClient_S();
};

// RefCount는 프라우드넷이 제공하는 일종의 스마트 포인터로서 
// STL의 shared_ptr보다 경량화된 것입니다. 
// 자동적인 힙 메모리 해제를 위하여 매크로로 등록해준 
// CRemoteClientPtr_S를 raw pointer대신에 쓰도록 합니다.
typedef RefCount<CRemoteClient_S> CRemoteClientPtr_S;