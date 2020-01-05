﻿



  
// Generated by PIDL compiler.
// Do not modify this file, but modify the source .pidl file.

#pragma once


#include "C2S_common.h"

namespace C2S {


	class Proxy : public ::Proud::IRmiProxy
	{
	public:
	virtual bool Chat ( ::Proud::HostID remote, ::Proud::RmiContext& rmiContext , const Proud::String & a, const int & b, const float & c, const MyClass & d, const Proud::CFastArray<int> & f, const Proud::CFastMap<int,float> & g, const Proud::ByteArray & block) PN_SEALED; 
	virtual bool Chat ( ::Proud::HostID *remotes, int remoteCount, ::Proud::RmiContext &rmiContext, const Proud::String & a, const int & b, const float & c, const MyClass & d, const Proud::CFastArray<int> & f, const Proud::CFastMap<int,float> & g, const Proud::ByteArray & block)   PN_SEALED;  
static const PNTCHAR* RmiName_Chat;
static const PNTCHAR* RmiName_First;
		Proxy()
		{
			if(m_signature != 1)
				::Proud::ShowUserMisuseError(::Proud::ProxyBadSignatureErrorText);
		}

		virtual ::Proud::RmiID* GetRmiIDList() PN_OVERRIDE { return g_RmiIDList; } 
		virtual int GetRmiIDListCount() PN_OVERRIDE { return g_RmiIDListCount; }
	};

}



