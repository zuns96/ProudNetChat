#pragma once
extern Proud::Guid g_Version;
extern int g_ServerPort;

class User 
{
public:
	Proud::String m_name;
};

namespace Proud
{
	void AppendTextOut(Proud::String &a, const User &b);

	Proud::CMessage& operator >> (Proud::CMessage &a, User &b);

	Proud::CMessage& operator << (Proud::CMessage &a, const User &b);
}