﻿  






// Generated by PIDL compiler.
// Do not modify this file, but modify the source .pidl file.
   
#pragma once


#include "C2C_common.h"

     
namespace C2C {


	class Stub : public ::Proud::IRmiStub
	{
	public:
               
		virtual bool P2PChat ( ::Proud::HostID, ::Proud::RmiContext& , const Proud::String & , const int & , const float & )		{ 
			return false;
		} 

#define DECRMI_C2C_P2PChat bool P2PChat ( ::Proud::HostID remote, ::Proud::RmiContext& rmiContext , const Proud::String & a, const int & b, const float & c) PN_OVERRIDE

#define DEFRMI_C2C_P2PChat(DerivedClass) bool DerivedClass::P2PChat ( ::Proud::HostID remote, ::Proud::RmiContext& rmiContext , const Proud::String & a, const int & b, const float & c)
#define CALL_C2C_P2PChat P2PChat ( ::Proud::HostID remote, ::Proud::RmiContext& rmiContext , const Proud::String & a, const int & b, const float & c)
#define PARAM_C2C_P2PChat ( ::Proud::HostID remote, ::Proud::RmiContext& rmiContext , const Proud::String & a, const int & b, const float & c)
 
		virtual bool ProcessReceivedMessage(::Proud::CReceivedMessage &pa, void* hostTag) PN_OVERRIDE;
		static const PNTCHAR* RmiName_P2PChat;
		static const PNTCHAR* RmiName_First;
		virtual ::Proud::RmiID* GetRmiIDList() PN_OVERRIDE { return g_RmiIDList; }
		virtual int GetRmiIDListCount() PN_OVERRIDE { return g_RmiIDListCount; }
	};

#ifdef SUPPORTS_CPP11 
	
	class StubFunctional : public Stub 
	{
	public:
               
		std::function< bool ( ::Proud::HostID, ::Proud::RmiContext& , const Proud::String & , const int & , const float & ) > P2PChat_Function;
		virtual bool P2PChat ( ::Proud::HostID remote, ::Proud::RmiContext& rmiContext , const Proud::String & a, const int & b, const float & c) 
		{ 
			if (P2PChat_Function==nullptr) 
				return true; 
			return P2PChat_Function(remote,rmiContext, a, b, c); 
		}

	};
#endif

}


