#pragma once
using namespace Proud;

extern Guid gProtocolVersion;
extern int gServerPort;

namespace Proud
{ 
	inline void AppendTextOut(String &a, const HostIDArray &b)
	{
		String f;
		f.Format(L"<Proud::HostIDArray>");
		a += f;
	}
}