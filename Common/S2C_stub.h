﻿  






// Generated by PIDL compiler.
// Do not modify this file, but modify the source .pidl file.
   
#pragma once


#include "S2C_common.h"

     
namespace S2C {


	class Stub : public ::Proud::IRmiStub
	{
	public:
               
		virtual bool ShowChat ( ::Proud::HostID, ::Proud::RmiContext& , const Proud::String & , const Proud::String & )		{ 
			return false;
		} 

#define DECRMI_S2C_ShowChat bool ShowChat ( ::Proud::HostID remote, ::Proud::RmiContext& rmiContext , const Proud::String & userName, const Proud::String & text) PN_OVERRIDE

#define DEFRMI_S2C_ShowChat(DerivedClass) bool DerivedClass::ShowChat ( ::Proud::HostID remote, ::Proud::RmiContext& rmiContext , const Proud::String & userName, const Proud::String & text)
#define CALL_S2C_ShowChat ShowChat ( ::Proud::HostID remote, ::Proud::RmiContext& rmiContext , const Proud::String & userName, const Proud::String & text)
#define PARAM_S2C_ShowChat ( ::Proud::HostID remote, ::Proud::RmiContext& rmiContext , const Proud::String & userName, const Proud::String & text)
               
		virtual bool UserList_Add ( ::Proud::HostID, ::Proud::RmiContext& , const Proud::String & , const Proud::HostID & )		{ 
			return false;
		} 

#define DECRMI_S2C_UserList_Add bool UserList_Add ( ::Proud::HostID remote, ::Proud::RmiContext& rmiContext , const Proud::String & userName, const Proud::HostID & hostID) PN_OVERRIDE

#define DEFRMI_S2C_UserList_Add(DerivedClass) bool DerivedClass::UserList_Add ( ::Proud::HostID remote, ::Proud::RmiContext& rmiContext , const Proud::String & userName, const Proud::HostID & hostID)
#define CALL_S2C_UserList_Add UserList_Add ( ::Proud::HostID remote, ::Proud::RmiContext& rmiContext , const Proud::String & userName, const Proud::HostID & hostID)
#define PARAM_S2C_UserList_Add ( ::Proud::HostID remote, ::Proud::RmiContext& rmiContext , const Proud::String & userName, const Proud::HostID & hostID)
               
		virtual bool UserList_Remove ( ::Proud::HostID, ::Proud::RmiContext& , const Proud::HostID & )		{ 
			return false;
		} 

#define DECRMI_S2C_UserList_Remove bool UserList_Remove ( ::Proud::HostID remote, ::Proud::RmiContext& rmiContext , const Proud::HostID & hostID) PN_OVERRIDE

#define DEFRMI_S2C_UserList_Remove(DerivedClass) bool DerivedClass::UserList_Remove ( ::Proud::HostID remote, ::Proud::RmiContext& rmiContext , const Proud::HostID & hostID)
#define CALL_S2C_UserList_Remove UserList_Remove ( ::Proud::HostID remote, ::Proud::RmiContext& rmiContext , const Proud::HostID & hostID)
#define PARAM_S2C_UserList_Remove ( ::Proud::HostID remote, ::Proud::RmiContext& rmiContext , const Proud::HostID & hostID)
 
		virtual bool ProcessReceivedMessage(::Proud::CReceivedMessage &pa, void* hostTag) PN_OVERRIDE;
		static const PNTCHAR* RmiName_ShowChat;
		static const PNTCHAR* RmiName_UserList_Add;
		static const PNTCHAR* RmiName_UserList_Remove;
		static const PNTCHAR* RmiName_First;
		virtual ::Proud::RmiID* GetRmiIDList() PN_OVERRIDE { return g_RmiIDList; }
		virtual int GetRmiIDListCount() PN_OVERRIDE { return g_RmiIDListCount; }
	};

#ifdef SUPPORTS_CPP11 
	
	class StubFunctional : public Stub 
	{
	public:
               
		std::function< bool ( ::Proud::HostID, ::Proud::RmiContext& , const Proud::String & , const Proud::String & ) > ShowChat_Function;
		virtual bool ShowChat ( ::Proud::HostID remote, ::Proud::RmiContext& rmiContext , const Proud::String & userName, const Proud::String & text) 
		{ 
			if (ShowChat_Function==nullptr) 
				return true; 
			return ShowChat_Function(remote,rmiContext, userName, text); 
		}

               
		std::function< bool ( ::Proud::HostID, ::Proud::RmiContext& , const Proud::String & , const Proud::HostID & ) > UserList_Add_Function;
		virtual bool UserList_Add ( ::Proud::HostID remote, ::Proud::RmiContext& rmiContext , const Proud::String & userName, const Proud::HostID & hostID) 
		{ 
			if (UserList_Add_Function==nullptr) 
				return true; 
			return UserList_Add_Function(remote,rmiContext, userName, hostID); 
		}

               
		std::function< bool ( ::Proud::HostID, ::Proud::RmiContext& , const Proud::HostID & ) > UserList_Remove_Function;
		virtual bool UserList_Remove ( ::Proud::HostID remote, ::Proud::RmiContext& rmiContext , const Proud::HostID & hostID) 
		{ 
			if (UserList_Remove_Function==nullptr) 
				return true; 
			return UserList_Remove_Function(remote,rmiContext, hostID); 
		}

	};
#endif

}


