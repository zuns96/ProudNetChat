﻿




// Generated by PIDL compiler.
// Do not modify this file, but modify the source .pidl file.


#include "C2C_proxy.h"

namespace C2C {


        
	bool Proxy::P2PChat ( ::Proud::HostID remote, ::Proud::RmiContext& rmiContext , const Proud::HostID & p2pGroupID, const Proud::String & text)	{
		::Proud::CMessage __msg;
__msg.UseInternalBuffer();
__msg.SetSimplePacketMode(m_core->IsSimplePacketMode());

::Proud::RmiID __msgid=(::Proud::RmiID)Rmi_P2PChat;
__msg.Write(__msgid); 
	
__msg << p2pGroupID;
__msg << text;
		
		return RmiSend(&remote,1,rmiContext,__msg,
			RmiName_P2PChat, (::Proud::RmiID)Rmi_P2PChat);
	}

	bool Proxy::P2PChat ( ::Proud::HostID *remotes, int remoteCount, ::Proud::RmiContext &rmiContext, const Proud::HostID & p2pGroupID, const Proud::String & text)  	{
		::Proud::CMessage __msg;
__msg.UseInternalBuffer();
__msg.SetSimplePacketMode(m_core->IsSimplePacketMode());

::Proud::RmiID __msgid=(::Proud::RmiID)Rmi_P2PChat;
__msg.Write(__msgid); 
	
__msg << p2pGroupID;
__msg << text;
		
		return RmiSend(remotes,remoteCount,rmiContext,__msg,
			RmiName_P2PChat, (::Proud::RmiID)Rmi_P2PChat);
	}
        
	bool Proxy::P2P_BigData ( ::Proud::HostID remote, ::Proud::RmiContext& rmiContext , const Proud::ByteArray & data, const Proud::HostID & p2pGroupID)	{
		::Proud::CMessage __msg;
__msg.UseInternalBuffer();
__msg.SetSimplePacketMode(m_core->IsSimplePacketMode());

::Proud::RmiID __msgid=(::Proud::RmiID)Rmi_P2P_BigData;
__msg.Write(__msgid); 
	
__msg << data;
__msg << p2pGroupID;
		
		return RmiSend(&remote,1,rmiContext,__msg,
			RmiName_P2P_BigData, (::Proud::RmiID)Rmi_P2P_BigData);
	}

	bool Proxy::P2P_BigData ( ::Proud::HostID *remotes, int remoteCount, ::Proud::RmiContext &rmiContext, const Proud::ByteArray & data, const Proud::HostID & p2pGroupID)  	{
		::Proud::CMessage __msg;
__msg.UseInternalBuffer();
__msg.SetSimplePacketMode(m_core->IsSimplePacketMode());

::Proud::RmiID __msgid=(::Proud::RmiID)Rmi_P2P_BigData;
__msg.Write(__msgid); 
	
__msg << data;
__msg << p2pGroupID;
		
		return RmiSend(remotes,remoteCount,rmiContext,__msg,
			RmiName_P2P_BigData, (::Proud::RmiID)Rmi_P2P_BigData);
	}
        
	bool Proxy::P2P_BigDataAck ( ::Proud::HostID remote, ::Proud::RmiContext& rmiContext , const Proud::HostID & p2pGroupID)	{
		::Proud::CMessage __msg;
__msg.UseInternalBuffer();
__msg.SetSimplePacketMode(m_core->IsSimplePacketMode());

::Proud::RmiID __msgid=(::Proud::RmiID)Rmi_P2P_BigDataAck;
__msg.Write(__msgid); 
	
__msg << p2pGroupID;
		
		return RmiSend(&remote,1,rmiContext,__msg,
			RmiName_P2P_BigDataAck, (::Proud::RmiID)Rmi_P2P_BigDataAck);
	}

	bool Proxy::P2P_BigDataAck ( ::Proud::HostID *remotes, int remoteCount, ::Proud::RmiContext &rmiContext, const Proud::HostID & p2pGroupID)  	{
		::Proud::CMessage __msg;
__msg.UseInternalBuffer();
__msg.SetSimplePacketMode(m_core->IsSimplePacketMode());

::Proud::RmiID __msgid=(::Proud::RmiID)Rmi_P2P_BigDataAck;
__msg.Write(__msgid); 
	
__msg << p2pGroupID;
		
		return RmiSend(remotes,remoteCount,rmiContext,__msg,
			RmiName_P2P_BigDataAck, (::Proud::RmiID)Rmi_P2P_BigDataAck);
	}
#ifdef USE_RMI_NAME_STRING
const PNTCHAR* Proxy::RmiName_P2PChat =_PNT("P2PChat");
#else
const PNTCHAR* Proxy::RmiName_P2PChat =_PNT("");
#endif
#ifdef USE_RMI_NAME_STRING
const PNTCHAR* Proxy::RmiName_P2P_BigData =_PNT("P2P_BigData");
#else
const PNTCHAR* Proxy::RmiName_P2P_BigData =_PNT("");
#endif
#ifdef USE_RMI_NAME_STRING
const PNTCHAR* Proxy::RmiName_P2P_BigDataAck =_PNT("P2P_BigDataAck");
#else
const PNTCHAR* Proxy::RmiName_P2P_BigDataAck =_PNT("");
#endif
const PNTCHAR* Proxy::RmiName_First = RmiName_P2PChat;

}



