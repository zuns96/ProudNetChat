#include "ProudNetClient.h"
#include "Packets.h"

Proud::PNGUID guid = { 0xeedf4dc9, 0xd6c4, 0x42cf, { 0xb1, 0x11, 0xe3, 0x7e, 0x6e, 0x46, 0x3e, 0xdb } };

Proud::Guid g_Version = Proud::Guid(guid);
int g_ServerPort = 33334;

namespace Proud
{
	//Req_Login
	void AppendTextOut(Proud::String &a, const Req_Login &b)
	{
		Proud::String f;
		f.Format(_PNT("¢¸Req_Login: id(%s)password(%s)"), b.id, b.password);
		a += f;
	}

	CMessage& operator >> (Proud::CMessage &a, Req_Login &b)
	{
		a >> b.id;
		a >> b.password;

		return a;
	}

	CMessage& operator << (Proud::CMessage &a, const Req_Login &b)
	{
		a << b.id;
		a << b.password;

		return a;
	}

	//Rpy_Login
	void AppendTextOut(Proud::String &a, const Rpy_Login &b)
	{
		Proud::String f;
		f.Format(_PNT("¢¸Rpy_Login: id(%s)"), b.id);
		a += f;
	}

	CMessage& operator >> (Proud::CMessage &a, Rpy_Login &b)
	{
		a >> b.id;

		return a;
	}

	CMessage& operator << (Proud::CMessage &a, const Rpy_Login &b)
	{
		a << b.id;

		return a;
	}

	// Req_Chat
	void AppendTextOut(Proud::String &a, const Req_Chat &b)
	{
		Proud::String f;
		f.Format(_PNT("¢¸Req_Chat: msg(%s)"), b.msg);
		a += f;
	}

	CMessage& operator >> (Proud::CMessage &a, Req_Chat &b)
	{
		a >> b.msg;

		return a;
	}

	CMessage& operator << (Proud::CMessage &a, const Req_Chat &b)
	{
		a << b.msg;

		return a;
	}

	// Rpy_Chat
	void AppendTextOut(Proud::String &a, const Rpy_Chat &b)
	{
		Proud::String f;
		f.Format(_PNT("¢¸Rpy_Chat: HostID(%d)msg(%s)"), b.hostID, b.msg);
		a += f;
	}

	CMessage& operator >> (Proud::CMessage &a, Rpy_Chat &b)
	{
		a >> b.hostID;
		a >> b.msg;

		return a;
	}

	CMessage& operator << (Proud::CMessage &a, const Rpy_Chat &b)
	{
		a << b.hostID;
		a << b.msg;

		return a;
	}

	//Rpy_System_Chat
	void AppendTextOut(Proud::String &a, const Rpy_System_Chat &b)
	{
		Proud::String f;
		f.Format(_PNT("¢¸Rpy_System_Chat: msg(%s)"), b.msg);
		a += f;
	}

	CMessage& operator >> (Proud::CMessage &a, Rpy_System_Chat &b)
	{
		a >> b.msg;

		return a;
	}

	CMessage& operator << (Proud::CMessage &a, const Rpy_System_Chat &b)
	{
		a << b.msg;

		return a;
	}

	void AppendTextOut(Proud::String &a, const User &b)
	{
		Proud::String f;
		f.Format(_PNT("User.name: %s\n"), b.m_name);
		a += f;
	}

	CMessage& operator>>(Proud::CMessage &a, User &b)
	{
		a >> b.m_name;

		return a;
	}

	CMessage& operator<<(Proud::CMessage &a, const User &b)
	{
		a << b.m_name;

		return a;
	}
}