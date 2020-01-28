#pragma once

// 클라이언트와 서버간 주고 받는 프로토콜 버전(GUID) 및 서버 포트 정의
extern Proud::PNGUID guid;

extern Proud::Guid g_Version;
extern int g_ServerPort;

class Req_Login
{
public :
	Proud::String id;
	Proud::String password;
};

class Rpy_Login
{
public:
	Proud::String id;
};

class Req_Chat
{
public :
	Proud::String msg;
};

class Rpy_Chat
{
public:
	int hostID;
	Proud::String msg;
};

class Rpy_System_Chat
{
public:
	Proud::String msg;
};

class User
{
public:
	Proud::String m_name;
};

namespace Proud
{
	void AppendTextOut(Proud::String &a, const Req_Login &b);
	Proud::CMessage& operator >> (Proud::CMessage &a, Req_Login &b);
	Proud::CMessage& operator << (Proud::CMessage &a, const Req_Login &b);

	void AppendTextOut(Proud::String &a, const Rpy_Login &b);
	Proud::CMessage& operator >> (Proud::CMessage &a, Rpy_Login &b);
	Proud::CMessage& operator << (Proud::CMessage &a, const Rpy_Login &b);

	void AppendTextOut(Proud::String &a, const Req_Chat &b);
	Proud::CMessage& operator >> (Proud::CMessage &a, Req_Chat &b);
	Proud::CMessage& operator << (Proud::CMessage &a, const Req_Chat &b);

	void AppendTextOut(Proud::String &a, const Rpy_Chat &b);
	Proud::CMessage& operator >> (Proud::CMessage &a, Rpy_Chat &b);
	Proud::CMessage& operator << (Proud::CMessage &a, const Rpy_Chat &b);

	void AppendTextOut(Proud::String &a, const Rpy_System_Chat &b);
	Proud::CMessage& operator >> (Proud::CMessage &a, Rpy_System_Chat &b);
	Proud::CMessage& operator << (Proud::CMessage &a, const Rpy_System_Chat &b);

	void AppendTextOut(Proud::String &a, const User &b);
	Proud::CMessage& operator >> (Proud::CMessage &a, User &b);
	Proud::CMessage& operator << (Proud::CMessage &a, const User &b);
}