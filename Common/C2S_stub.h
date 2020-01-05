﻿  






// Generated by PIDL compiler.
// Do not modify this file, but modify the source .pidl file.
   
#pragma once


#include "C2S_common.h"

     
namespace C2S {


	class Stub : public ::Proud::IRmiStub
	{
	public:
               
		virtual bool Chat ( ::Proud::HostID, ::Proud::RmiContext& , const Proud::String & , const int & , const float & , const MyClass & , const Proud::CFastArray<int> & , const Proud::CFastMap<int,float> & , const Proud::ByteArray & )		{ 
			return false;
		} 

#define DECRMI_C2S_Chat bool Chat ( ::Proud::HostID remote, ::Proud::RmiContext& rmiContext , const Proud::String & a, const int & b, const float & c, const MyClass & d, const Proud::CFastArray<int> & f, const Proud::CFastMap<int,float> & g, const Proud::ByteArray & block) PN_OVERRIDE

#define DEFRMI_C2S_Chat(DerivedClass) bool DerivedClass::Chat ( ::Proud::HostID remote, ::Proud::RmiContext& rmiContext , const Proud::String & a, const int & b, const float & c, const MyClass & d, const Proud::CFastArray<int> & f, const Proud::CFastMap<int,float> & g, const Proud::ByteArray & block)
#define CALL_C2S_Chat Chat ( ::Proud::HostID remote, ::Proud::RmiContext& rmiContext , const Proud::String & a, const int & b, const float & c, const MyClass & d, const Proud::CFastArray<int> & f, const Proud::CFastMap<int,float> & g, const Proud::ByteArray & block)
#define PARAM_C2S_Chat ( ::Proud::HostID remote, ::Proud::RmiContext& rmiContext , const Proud::String & a, const int & b, const float & c, const MyClass & d, const Proud::CFastArray<int> & f, const Proud::CFastMap<int,float> & g, const Proud::ByteArray & block)
 
		virtual bool ProcessReceivedMessage(::Proud::CReceivedMessage &pa, void* hostTag) PN_OVERRIDE;
		static const PNTCHAR* RmiName_Chat;
		static const PNTCHAR* RmiName_First;
		virtual ::Proud::RmiID* GetRmiIDList() PN_OVERRIDE { return g_RmiIDList; }
		virtual int GetRmiIDListCount() PN_OVERRIDE { return g_RmiIDListCount; }
	};

#ifdef SUPPORTS_CPP11 
	
	class StubFunctional : public Stub 
	{
	public:
               
		std::function< bool ( ::Proud::HostID, ::Proud::RmiContext& , const Proud::String & , const int & , const float & , const MyClass & , const Proud::CFastArray<int> & , const Proud::CFastMap<int,float> & , const Proud::ByteArray & ) > Chat_Function;
		virtual bool Chat ( ::Proud::HostID remote, ::Proud::RmiContext& rmiContext , const Proud::String & a, const int & b, const float & c, const MyClass & d, const Proud::CFastArray<int> & f, const Proud::CFastMap<int,float> & g, const Proud::ByteArray & block) 
		{ 
			if (Chat_Function==nullptr) 
				return true; 
			return Chat_Function(remote,rmiContext, a, b, c, d, f, g, block); 
		}

	};
#endif

}


