﻿#pragma once

namespace C2S {


	//Message ID that replies to each RMI method. 
               
    static const ::Proud::RmiID Rmi_Send_Req_Login = (::Proud::RmiID)(3000+1);
               
    static const ::Proud::RmiID Rmi_Send_Req_Chat = (::Proud::RmiID)(3000+2);

	// List that has RMI ID.
	extern ::Proud::RmiID g_RmiIDList[];
	// RmiID List Count
	extern int g_RmiIDListCount;

}


 
