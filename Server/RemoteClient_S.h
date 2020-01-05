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

// RefCount�� ��������� �����ϴ� ������ ����Ʈ �����ͷμ� 
// STL�� shared_ptr���� �淮ȭ�� ���Դϴ�. 
// �ڵ����� �� �޸� ������ ���Ͽ� ��ũ�η� ������� 
// CRemoteClientPtr_S�� raw pointer��ſ� ������ �մϴ�.
typedef RefCount<CRemoteClient_S> CRemoteClientPtr_S;